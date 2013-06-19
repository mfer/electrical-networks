# -*- coding: utf-8 -*-
import sys

saida = None

buflist = []
corlist = []
cor_count = 0
arcos_count = 0

if len(sys.argv) < 2:
    print 'Uso: python %s <entrada> [saida]' % sys.argv[0]
elif len(sys.argv) == 3:
    if sys.argv[1] == sys.argv[2]:
        print 'Os arquivos de entrada e saída não podem ser os mesmos. Abortando.'
        sys.exit(1)
    saida = open(sys.argv[2], 'w')

try:
    entrada = open(sys.argv[1])
except:
    print 'Não foi possível abrir o arquivo de entrada.'
    sys.exit(3)

for linha in entrada:
    l = linha.strip()
    if l.startswith('a'):
        temp = linha.split()        
        #print temp[-5]+' '+temp[-4]
        #print int(temp[-5]) < int(temp[-4])
        if int(temp[-5]) < int(temp[-4]):
					  arcos_count = arcos_count + 1
					  buflist.append(temp[-5]+' '+temp[-4]+' '+temp[-1]+'\n')

    elif l.startswith('p'):
        temp = linha.split()
        num_nos = int(temp[-2])
        #num_arcos = int(temp[-1])#/2 #'nossa' entrada duplicava os arcos
        #buflist.append(temp[-2] + ' ' + str(num_arcos) + '\n')
        
    elif l.startswith('n'):
        temp = linha.split()
        cor_count = cor_count + 1
        corlist.append(temp[-2] + ' ' + temp[-1] + '\n')


buflist.insert(0,str(num_nos)+' '+str(arcos_count)+'\n')
corlist.insert(0,str(cor_count)+'\n')
buflist = buflist + corlist

if saida is None:
    sys.stdout.writelines(buflist)
else:
    saida.writelines(buflist)
    
