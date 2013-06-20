# -*- coding: utf-8 -*-
import sys

def mult_lines(entrada, mult):
    """
    Entrada:
    Um descritor de arquivo aberto para leitura e um inteiro maior que 1

    Saída:
    Uma lista com as novas linhas do arquivo de saída.
    """

    buflist = []
    for linha in entrada:
        l = linha.strip()
        if l.startswith('a'):

            for arco in xrange(mult):
                temp = linha.split()
                cost = float(temp[-1])*(float(arco+1)/mult)
                cap = float(temp[-2])/mult
                buflist.append(\
                    ' '.join(temp[:-2]) + ' %d %d\n' % (round(cap), round(cost))) # Arredonda pra cima
            #buflist.append(' '.join(temp[:-1]) + ' %d\n' % cap) # Arredonda pra baixo


        elif l.startswith('p'):
            temp = linha.split()
            num_arcos = int(temp[-1])*mult        
            buflist.append(' '.join(temp[:-1]) + ' %d\n' % num_arcos)
        else:
            buflist.append(linha)
    return buflist

def main():
    saida = None
    
    buflist = []

    if len(sys.argv) < 3:
        print 'Uso: python %s <multiplicador> <entrada> [saida]' % sys.argv[0]
        sys.exit(0)

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


    buflist = mult_lines(mult)

    if saida is None:
        sys.stdout.writelines(buflist)
    else:
        saida.writelines(buflist)
    
if __name__ == "__main__":
    main()
