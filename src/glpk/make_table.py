# -*- coding: utf-8 -*-
import sys
import os
import tempfile
import time

from mult_arc import mult_lines

OKALG = 'okalg'

def processa_okalg_output(entrada):
    """
    Processa uma lista com as linhas de entrada e retorna uma lista
    com arcos processados.
    """
    arcs = []
    for linha in entrada:
        if linha.strip().startswith('arc'):
            t = linha.strip()[3:].split()
            source = t[0].split('->')[0].strip(':')
            dest = t[0].split('->')[1].strip(':')
            x = t[3].strip(';')
            l = t[6]
            arcs.append((source, dest, x,l))
    return arcs


def main():

    okalg_path = os.path.dirname(os.path.realpath(__file__)) + '/' + OKALG
    to_process = [1]



    if len(sys.argv) < 3:
        print 'Uso: python %s <entrada> <divisoes separadas por virgula.' % sys.argv[0]
        print 'Exemplo: python %s arquivo.in 1,2,4,8' % sys.argv[0]
        sys.exit(0)

    to_process = [int(i) for i in sys.argv[2].split(',')]


    try:
        with open(sys.argv[1]) as entrada:
            okalg_lines = entrada.readlines()            
    except:
        print 'Não foi possível abrir o arquivo de entrada.'
        sys.exit(3)

    orig_okalg = mult_lines(okalg_lines, 1)
    temp_file = tempfile.NamedTemporaryFile()
    temp_file.writelines(orig_okalg)
    temp_file.flush()

    orig_saida_okalg = os.popen(okalg_path + ' %s' % temp_file.name).readlines()
    temp_file.close()
    del temp_file

    orig_costs = processa_okalg_output(orig_saida_okalg)

    arcos = [(i[0],i[1]) for i in orig_costs]
    mult_costs = []


    for multiplicador in to_process:
        new_okalg_lines = mult_lines(okalg_lines, int(multiplicador))
        temp_file = tempfile.NamedTemporaryFile()
        temp_file.writelines(new_okalg_lines)
        temp_file.flush()
        t_inicio = time.time()
        saida_okalg = os.popen(okalg_path + ' %s' % temp_file.name).readlines()
        t_fim = time.time()
        arcs_costs = processa_okalg_output(saida_okalg)
        costs = []
        counter = 0
        for index in xrange(len(orig_costs)):
            costs.append(0)

            for dummy2 in xrange(int(multiplicador)):							  
                costs[index] += int(arcs_costs[counter][2])
                counter += 1

        #     print'%s &' % costs[index],
        
        costs.insert(0,multiplicador)
        costs.append((t_fim - t_inicio)*1000)
        mult_costs.append(costs)
        # for source, dest, cost, l in arcs_costs:
        #     costs.append(cost)

        # print costs
    costs_mult = [[row[i] for row in mult_costs] for i in range(len(mult_costs[0]))]
    mult_costs = []

    counter = 0
    new_arcs = []

    mult_costs.append(costs_mult[0])
    for index in xrange(1, len(costs_mult)-1 ):
        if any(costs_mult[index]):
            new_arcs.append(arcos[index])
            mult_costs.append(costs_mult[index])
    mult_costs.append(costs_mult[-1])

    costs_mult = [[row[i] for row in mult_costs] for i in range(len(mult_costs[0]))]
    
    print '\\scalebox{0.5} {'
    print '\\begin {tabular}{%s}' % ('|c' * (len(costs_mult[0])) + '|')
    print '\\hline'
    print 'Segmento',
    print ' & '.join([('%s->%s' % (s[0], s[1])) for s in new_arcs]) + ' & tempo (ms)\\\\ \\hline'
    for linha in costs_mult:
        print ' & '.join([str(s) for s in linha]) + '\\\\ \\hline'

    print 
    print '\\\\ \\hline'



    # print
    # print '\\end{tabular}'         
    # print '}'
         #     print 
             

    #f_entrada = open(sys.argv[1])

    
    # while True:
    #     try:
    #         linha = raw_input()
    #     except:
    #         break
    #     input_list.append(linha)

    # arcs = processa_okalg_output(input_list)
    # print input_list

    # for x in arcs:
    #     print x

if __name__ == '__main__':
    main()

        
