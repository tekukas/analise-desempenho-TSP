import networkx as nx

def geraGrafo(arquivo_tsp):
	# Inicializa o grafo
	grafo = nx.Graph()

	with open(arquivo_tsp, 'r') as arquivo:
		linhas = arquivo.readlines()

		# Variável para controlar se estamos na seção NODE_COORD_SECTION
		em_secao_coord = False

		for linha in linhas:
			# Remove espaços em branco no início e no final da linha
			linha = linha.strip()

			if linha.startswith("DIMENSION"):
				dimensao = int(linha.split(":")[1])
			elif linha.startswith("NODE_COORD_SECTION"):
				em_secao_coord = True
			elif em_secao_coord and linha.startswith("EOF"):
				em_secao_coord = False
			elif em_secao_coord:
				# Divide a linha em partes e extrair informações de coordenadas
				partes = linha.split()
				cidade = int(partes[0])
				coordenada_x = float(partes[1])
				coordenada_y = float(partes[2])

				grafo.add_node(cidade, pos=(coordenada_x, coordenada_y))
				
				# Adiciona aresta entre todos os nós já adicionados com peso = distância euclidiana
				for outra_cidade in grafo.nodes():
					if outra_cidade == cidade:
						continue

					# Extrair coordenadas do outro nó
					coordenada_x_outro, coordenada_y_outro = grafo.nodes[outra_cidade]["pos"]

					# Calcular distância euclidiana
					distancia = ((coordenada_x - coordenada_x_outro)**2 + (coordenada_y - coordenada_y_outro)**2)**0.5

					# Adicionar aresta com peso = distância euclidiana
					grafo.add_edge(cidade, outra_cidade, weight=distancia)

	return grafo