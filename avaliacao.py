from aproximativos import christofides, twice
from primitivas import geraGrafo
import time
from memory_profiler import memory_usage
from functools import partial
from multiprocessing import freeze_support
import signal

def main():
	testaTodos = True
	numTestes = 1
	arquivoTestes = 'lista-datasets.txt'
	arquivoSaida = 'saida.csv'

	with open(arquivoTestes, 'r') as arquivo:
		linhas = arquivo.readlines()

	linhas = linhas[1:] # Remove a primeira linha (cabeçalho)

	with open(arquivoSaida, 'w') as saida: # abre o arquivo de saida (se nao existir, cria)
		saida.write('Arquivo;Algoritmo;Tempo;Custo;Aproximacao;Memoria\n')

	if testaTodos:
		numTestes = len(linhas)

	for linha in linhas[:numTestes]:
		
		linha = linha.split() # arquivo, dimensao, custo otimo
		arquivo = 'datasets/' + linha[0] + '.tsp'

		try:
			grafo = geraGrafo(arquivo)
		except:
			print('Erro em geraGrafo: Arquivo nao encontrado')
			continue

		with open(arquivoSaida, 'a') as saida:
			saida.write(linha[0] + ';')
			saida.write('Christofides;')

		signal.signal(signal.SIGALRM, lambda x, y: exit())
		signal.alarm(30*60)

		inicioCristo = time.time()
		custoCristo = christofides(grafo)
		fimCristo = time.time()

		signal.alarm(0)

		memTAT, distTAT = memory_usage(partial(christofides, grafo), retval=True, interval=1.0, max_usage=True)
		
		with open(arquivoSaida, 'a') as saida:
			saida.write('{};'.format(round(fimCristo-inicioCristo, 5))) # tempo
			saida.write('{};'.format(round(custoCristo, 2))) # custo
			saida.write('{};'.format(round(custoCristo/float(linha[2]), 2))) # aproximacao
			saida.write('{}'.format(round(memTAT, 2))) # memoria
			saida.write('\n')

			saida.write(linha[0] + ';')
			saida.write('Twice around the tree;')	
		
		signal.signal(signal.SIGALRM, lambda x, y: exit())
		signal.alarm(30*60)
		
		inicioTwice = time.time()
		custoTwice = twice(grafo)
		fimTwice = time.time()
		
		signal.alarm(0)

		memTAT, distTAT = memory_usage(partial(twice, grafo), retval=True, interval=1.0, max_usage=True)
		
		with open(arquivoSaida, 'a') as saida:
			saida.write('{};'.format(round(fimTwice-inicioTwice, 5))) # tempo
			saida.write('{};'.format(round(custoTwice, 2))) # custo
			saida.write('{};'.format(round(custoTwice/float(linha[2]), 2))) # aproximacao
			saida.write('{}'.format(round(memTAT, 2))) # memoria
			saida.write('\n')

if __name__ == '__main__':
	freeze_support()
	# seta um timeout de 30 minutos para o programa

	main()
	