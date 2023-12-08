# Avaliação do desempenho das soluções para o Problema do Caixeiro Viajante (PCV)
Este artigo avalia o desempenho de três implementações de algoritmos para computação de rotas no problema NP-difícil do Caixeiro Viajante. Serão analisadas duas soluções aproximadas e uma exata, avaliando o tempo e espaço usado por cada uma, além da qualidade da solução.

Os algoritmos aproximativos são "Twice around the tree" e "Christofides", e o código está presente no arquivo [aproximativos.py](aproximativos.py).

O algoritmo Branch and Bound está presente no arquivo [bnb.py](bnb.py)

O arquivo [primitivas.py](primitivas.py) contém funções auxiliares para os algoritmos, como criação de grafos a partir dos arquivos ```.tsp```.

Por fum, o arquivo [lista-datasets.txt](lista-datasets.txt) contém todos os datasets da pasta nomeados e a solução exata de cada um, para análise do fator aproximativo.