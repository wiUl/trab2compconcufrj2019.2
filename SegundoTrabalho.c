#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int n_threads_leitoras, n_threads_escritoras;//armazenam o numero de threads leitoras e o numero de threads escritoras
int nescritas, nleituras;//armazenam quantas leituras e quantas escritas as threads leitoras e escritoras irão realizar
int escritora_bloq=0, leitora_bloq=0;//armazenam quantas threads escritoras e quantas threads leitoras estão bloqueadas esperando para poderem executar
int escrevendo=0, lendo=0;//armazena a quantidade de threads lendo e escrevendo em um determinado instante
int valor=-1;//recurso compartilhado entre as threads leitoras e as escritoras e começa como -1
int sinal = 1, sinal2 = 0;//variáveis utilizadas para remover a inanição do programa
FILE *Log_saida; //variável que rerpesenta o arquivo no qual o log será escrito

pthread_mutex_t mutex;//váriavel de exclusão mútua
pthread_cond_t cond_escrita, cond_leitura;//variáveis de condição das threads de leitura e escrita



void *Escritora(void *arg){
    int id = *((int*)arg);
    for(int contador=0; contador<nescritas; contador++){
        //checa o estado do programa e decide se a thread deve continuar ou esperar
        pthread_mutex_lock(&mutex);
        while(escrevendo>0 || lendo>0 || sinal2){
            escritora_bloq++;
            fprintf(Log_saida, "Escritora_bloq(1, %d, %d)\n", escritora_bloq, valor);
            fprintf(Log_saida, "Escrita_bloqueada(%d, %d, %d, %d, %d)\n", id, escrevendo, lendo, sinal2, valor);
            pthread_cond_wait(&cond_escrita, &mutex);
        }
        escrevendo++;
        fprintf(Log_saida, "Escrevendo(1, %d, %d)\n", escrevendo, valor);
        pthread_mutex_unlock(&mutex);
        
        //efetua a escrita
        valor = id;
        fprintf(Log_saida, "Set_valor(%d)\n", valor);
        
        //finaliza a escrita
        pthread_mutex_lock(&mutex);
        escrevendo--;
        fprintf(Log_saida, "Escrevendo(-1, %d, %d)\n", escrevendo, valor);
        if(leitora_bloq>0){
            leitora_bloq--;
            fprintf(Log_saida, "Leitora_bloq(-1, %d, %d)\n", leitora_bloq, valor);
            sinal = 0;
            fprintf(Log_saida, "Sinal(0, %d)\n", valor);
            sinal2 = 1;
            fprintf(Log_saida, "Sinal2(1, %d)\n", valor);
            pthread_cond_signal(&cond_leitura);
            pthread_mutex_unlock(&mutex);
            
        }else if(escritora_bloq>0){
            escritora_bloq--;
            fprintf(Log_saida, "Escritora_bloq(-1, %d, %d)\n", escritora_bloq, valor);
            pthread_cond_signal(&cond_escrita);
            pthread_mutex_unlock(&mutex);
            
        }
        else{
            pthread_mutex_unlock(&mutex);
        }
    }
    free(arg);
    pthread_exit(NULL);
}

void *Leitora(void *arg){
    //checa o estado atual do programa e verifica se a thread pode seguir ou deve esperar e inicia o arquivo em que a saída será impressa
    char id_str[10];
    char nome_arq[50];
    int id = *((int*)arg);
    int k = snprintf(id_str, 10, "%d", id);
    //itoa(id, id_str, 10);
    strcpy(nome_arq, id_str);
    strcat(nome_arq, ".txt");
    FILE *arquivo;
    arquivo = fopen(nome_arq, "w");
    for(int contador=0; contador<nleituras; contador++){
        pthread_mutex_lock(&mutex);
        while(escrevendo>0 || sinal){
            leitora_bloq++;
            fprintf(Log_saida, "Leitora_bloq(1, %d, %d)\n", leitora_bloq, valor);
            fprintf(Log_saida, "Leitura_bloqueada(%d, %d, %d, %d)\n", id, escrevendo, sinal, valor);
            pthread_cond_wait(&cond_leitura, &mutex);
        }
        lendo++;
        fprintf(Log_saida, "Lendo(1, %d, %d)\n", lendo, valor);
        pthread_mutex_unlock(&mutex);

        //efetua a leitura da variável compartilhada e a armazena no arquivo de saída
        fprintf(arquivo, "%d\n", valor);

        //finaliza a leitura
        pthread_mutex_lock(&mutex);
        lendo--;
        fprintf(Log_saida, "Lendo(-1, %d, %d)\n", lendo, valor);
        if(escritora_bloq>0){
            escritora_bloq--;
            fprintf(Log_saida, "Escritora_bloq(-1, %d, %d)\n", escritora_bloq, valor);
            sinal = 1;
            fprintf(Log_saida, "Sinal(1, %d)\n", valor);
            sinal2 = 0;
            fprintf(Log_saida, "Sinal2(0, %d)\n", valor);
            pthread_cond_signal(&cond_escrita);
            pthread_mutex_unlock(&mutex);
        }else if(leitora_bloq>0){
            leitora_bloq--;
            fprintf(Log_saida, "Leitora_bloq(-1, %d, %d)\n", leitora_bloq, valor);
            pthread_cond_signal(&cond_leitura);
            pthread_mutex_unlock(&mutex);
        }else{
            pthread_mutex_unlock(&mutex);
        }
    }
    //fehca o arquivo de saida e finaliza a thread
    fclose(arquivo);
    free(arg);
    pthread_exit(NULL);
}

int main(int args, char *argv[]){
   if(args != 6){
       printf("numeros de argumentos invalidos\n");
       return 0;
   }
    n_threads_leitoras = atoi(argv[1]);
    n_threads_escritoras = atoi(argv[2]);
    nleituras = atoi(argv[3]);
    nescritas = atoi(argv[4]);
    Log_saida = fopen(argv[5], "w");
    int *arg;
    fprintf(Log_saida, "Set_ntleitoras(%d)\nSet_ntescritoras(%d)\nSet_nleituras(%d)\nSet_nescritas(%d)\n", n_threads_leitoras, n_threads_escritoras, nleituras, nescritas);
    pthread_t leitoras_id[n_threads_leitoras], escritoras_id[n_threads_escritoras];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_leitura,NULL);
    pthread_cond_init(&cond_escrita, NULL);
    for(int i=0; i<n_threads_leitoras; i++){
        arg = (int*)malloc(sizeof(int));
        *arg = i;
        if(pthread_create(&leitoras_id[i], NULL, Leitora, (void*)arg)){
            printf("erro\n");
            return 0;
        }
    }
    for(int i=0; i<n_threads_escritoras; i++){
        arg = (int*)malloc(sizeof(int));
        *arg = i;
        if(pthread_create(&escritoras_id[i], NULL, Escritora, (void*)arg)){
            printf("erro\n");
            return 0;
        }
    }


    for(int i=0; i<n_threads_escritoras; i++){
        pthread_join(escritoras_id[i], NULL);
    }
    for(int i=0; i<n_threads_leitoras; i++){
        pthread_join(leitoras_id[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_leitura);
    pthread_cond_destroy(&cond_escrita);
    fclose(Log_saida);
    return 0;
}
