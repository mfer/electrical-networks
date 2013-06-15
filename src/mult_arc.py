# -*- coding: utf-8 -*-
import sys

saida = None

buflist = []

if len(sys.argv) < 3:
    print 'Uso: python %s <multiplicador> <entrada> [saida]' % sys.argv[0]
elif len(sys.argv) == 4:
    if sys.argv[2] == sys.argv[3]:
        print 'Os arquivos de entrada e saída não podem ser os mesmos. Abortando.'
        sys.exit(1)
    saida = open(sys.argv[3], 'w')

try:
    mult = int(sys.argv[1])
except:
    print 'O parâmetro <multiplicador> precisa ser um número inteiro.'
    sys.exit(2)

try:
    entrada = open(sys.argv[2])
except:
    print 'Não foi possível abrir o arquivo de entrada.'
    sys.exit(3)

for linha in entrada:
    l = linha.strip()
    if l.startswith('a'):

        for arco in xrange(mult):
            temp = linha.split()
            cap = float(temp[-1])/mult
            buflist.append(' '.join(temp[:-1]) + ' %d\n' % round(cap)) # Arredonda pra cima
            #buflist.append(' '.join(temp[:-1]) + ' %d\n' % cap) # Arredonda pra baixo


    elif l.startswith('p'):
        temp = linha.split()
        num_arcos = int(temp[-1])*mult        
        buflist.append(' '.join(temp[:-1]) + ' %d\n' % num_arcos)
    else:
        buflist.append(linha)

if saida is None:
    sys.stdout.writelines(buflist)
else:
    saida.writelines(buflist)
    
