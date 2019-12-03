nome_arquivo = input()

arquivo = open(nome_arquivo, 'r')

n_threads_leitoras = -1
n_threads_escritoras = -1
nleituras = -1
nescritas = -1
valor = -1 
escritora_bloq = 0
leitora_bloq=0
lendo = 0
escrevendo = 0
sinal = 1
sinal2 = 0
leituras_totais = -1
escritas_totais = -1

def Set_ntleitoras(i):
    global n_threads_leitoras
    n_threads_leitoras = i

def Set_ntescritoras(i):
    global n_threads_escritoras
    n_threads_escritoras = i

def Set_nleituras(i):
    global nleituras
    global n_threads_leitoras
    global leituras_totais
    nleituras = i
    leituras_totais = nleituras * n_threads_leitoras

def Set_nescritas(i):
    global nescritas
    global n_threads_escritoras
    global escritas_totais
    nescritas = i
    escritas_totais = nescritas * n_threads_escritoras

def Escrevendo(i,esc, valoratual):
    global escrevendo
    global valor
    global escritas_totais
    global n_threads_leitoras
    global leitora_bloq
    if(escrevendo+i == esc):
        escrevendo+=i
    else:
        print("A variável escrevendo não possui o valor esperado")
        exit()
    if(valoratual != valor):
        print("A variável valor não possui o valor esperado")
        exit()
    if(i < 0):
        escritas_totais -= 1
            
        
def Lendo(i,lend,valoratual):
    global lendo
    global valor
    global leituras_totais
    if(lendo+i == lend):
        lendo+=i
    else:
        print("A variável lendo não possui o valor esperado")
        exit()
    if(valoratual != valor):
        print("A variável valor não possui o valor esperado")
        exit()
    if(i < 0):
        leituras_totais -= 1
            


def Escritora_bloq(i,esc_bloq,valoratual):
    global valor
    global escritora_bloq
    if(escritora_bloq+i == esc_bloq):
        escritora_bloq+=i
    else:
        print("A variável escritora_bloq não possui o valor esperado")
        exit()
    if(valoratual != valor):
        print("A variável valor não possui o valor esperado")
        exit()
        

def Leitora_bloq(i,lend_bloq,valoratual):
    global valor
    global leitora_bloq
    if(leitora_bloq+i == lend_bloq):
        leitora_bloq+=i
    else:
        print("A variável leitora_bloq não possui o valor esperado")
        exit()
    if(valoratual != valor):
        print("A variável valor não possui o valor esperado")
        exit()        


def Sinal(i,valoratual):
    global valor
    global sinal
    sinal = i
    if(valoratual != valor):
        print("A variável valor não possui o valor esperado")
        exit()    


def Sinal2(i,valoratual):
    global valor
    global sinal2
    sinal2 = i
    if(valoratual != valor):
        print("A variável valor não possui o valor esperado")
        exit() 


def Set_valor(valoratual):
    global valor
    valor = valoratual
    if(valoratual != valor):
        print("A variável valor não possui o valor esperado")
        exit()
        

def Escrita_bloqueada(id,esc,lend,sin2,valoratual):
    global valor
    global escrevendo
    global lendo
    global sinal2
    if(escrevendo != esc):
        print("A variavel escrevendo não possui o valor esperado")
        exit()
    if(lendo != lend):
        print("A variavel lendo não possui o valor esperado")
        exit()
    if(sinal2 != sin2):
        print("A variavel sinal2 não possui o valor esperado")
        exit()    
    if(valoratual != valor):
        print("A variável valor não possui o valor esperado")
        exit()


def Leitura_bloqueada(id,esc,sin,valoratual):
    global valor
    global escrevendo
    global sinal
    if(escrevendo != esc):
        print("A variavel escrevendo não possui o valor esperado")
        exit()
    if(sinal != sin):
        print("A variavel sinal não possui o valor esperado")
        exit()
    if(valoratual != valor):
        print("A variável valor não possui o valor esperado")
        exit()


string = arquivo.readlines()
j = 0
while(j <len(string)):
    if(escrevendo > 0 and lendo > 0):
        print("Condição de corrida")
        exit()
    if(escrevendo > 1):
        print("Condição de corrida")
        exit()
    A = string[j]
    exec(A)
    j+=1

print("Programa executou sem problemas")
arquivo.close