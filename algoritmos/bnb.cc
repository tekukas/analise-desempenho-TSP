#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <fstream>
#include <tuple>
#include <cctype>
#include <stack>

using namespace std;

/// @brief Função auxiliar para extrair números de uma string
/// @param str String a ser analisada
/// @return Vetor com os números encontrados
vector<int> extrairNumeros(const string& str) {
    vector<int> numeros;
    string numeroAtual;
    bool pontoEncontrado = false;

    for (char caractere : str) {
        if (isdigit(caractere)) {
            // Se o caractere é um dígito, adiciona ao número atual
            numeroAtual += caractere;
        } else if (caractere == '.' && !pontoEncontrado) {
            // Se o caractere é um ponto decimal e ainda não encontramos um, adiciona ao número atual
            pontoEncontrado = true;
            numeroAtual += caractere;
        } else if (!numeroAtual.empty()) {
            // Se não é um dígito e já temos um número atual, converte e adiciona ao vetor
            numeros.push_back(stoi(numeroAtual));
            numeroAtual.clear();  // Limpa o número atual para começar um novo
            pontoEncontrado = false;  // Reinicia a flag do ponto decimal
        }
    }

    // Adiciona o último número, se houver
    if (!numeroAtual.empty()) {
        numeros.push_back(stoi(numeroAtual));
    }

    return numeros;
}

/// @brief Função para gerar o grafo a partir de um arquivo .tsp
/// @param nomeArquivo Nome do arquivo .tsp a ser lido
/// @return Grafo completo ponderado e não direcionado respeitando a desigualdade triangular em forma de matriz de adjacência
vector<vector<double>> geraGrafo(string nomeArquivo){
	ifstream arquivo(nomeArquivo);
	string linha;
	vector<vector<double>> grafo;
	vector<tuple<int, int>> vertices;
	int n;

	if (arquivo.is_open()){
		bool coords = false;
		while (getline(arquivo, linha)){
			if (linha == "EOF") break;

			if (linha.find("DIMENSION") == 0){
				n = extrairNumeros(linha)[0];
				grafo = vector<vector<double>>(n, vector<double>(n, numeric_limits<double>::infinity()));
			}
			else if (coords){
				vector<int> coordenadas = extrairNumeros(linha);
				vertices.push_back(make_tuple(coordenadas[1], coordenadas[2]));
			}
			else if (linha == "NODE_COORD_SECTION") coords = true;
		}
		arquivo.close();
	}

	for (int i = 0; i < vertices.size(); i++){
		for (int j = 0; j < vertices.size(); j++){
			if (i != j){
				int x1 = get<0>(vertices[i]);
				int y1 = get<1>(vertices[i]);
				int x2 = get<0>(vertices[j]);
				int y2 = get<1>(vertices[j]);
				double dist = sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
				grafo[i][j] = dist;
				grafo[j][i] = dist;
			}
		}
	}

	return grafo;
}

/// @brief Classe que representa um nó da árvore de busca, uma solução parcial do problema
class Node{
public:
	double bound;		// Limite inferior do custo do caminho
	int level;			// Nível do nó na árvore de busca
	double cost;		// Custo do caminho
	vector<int> path;	// Caminho do nó

	Node(double bound, int level, double cost, vector<int> path) : bound(bound), level(level), cost(cost), path(path) {}

	bool operator<(const Node& other) const{
		return bound > other.bound;
	}
};

/// @brief Calcula o limite inferior (lowe bound) do custo do caminho
/// @param grafo Grafo completo ponderado e não direcionado respeitando a desigualdade triangular
/// @param caminho Caminho parcial
/// @return Limite inferior do custo do caminho
double bound(vector<vector<double>>& grafo, vector<int> caminho){
	int n = grafo.size();
	double custo = 0;

	// Calcula o custo definido do caminho (multiplicado por 2 pois o grafo é não direcionado)
	for (int i = 0; i < caminho.size() - 1; i++){
		custo += grafo[caminho[i]][caminho[i+1]] * 2;
	}

	// Se o caminho já contém todos os vértices, adiciona o peso da aresta entre o último e o primeiro vértice e retorna
	if (caminho.size() == n){
		custo += grafo[caminho[n-1]][caminho[0]] * 2;
		return ceil(custo/2);
	}

	// Adiciona o peso da menor aresta incidente no primeiro e último vértices do caminho
	if (caminho.size() > 1 && caminho.size() < n){
		double menorArestaPrimeiro = numeric_limits<double>::infinity();
		double menorArestaUltimo = numeric_limits<double>::infinity();

		for (int i = 0; i < n; i++){
			if (find(caminho.begin(), caminho.end(), i) == caminho.end()) {
				if (grafo[caminho[0]][i] < menorArestaPrimeiro) menorArestaPrimeiro = grafo[caminho[0]][i];
				if (grafo[caminho[caminho.size()-1]][i] < menorArestaUltimo) menorArestaUltimo = grafo[caminho[caminho.size()-1]][i];
			}
		}

		custo += menorArestaPrimeiro + menorArestaUltimo;
	}

	// Adiciona o peso das duas menores arestas incidentes em cada vértice não visitado
	for (int i = 0; i < n; i++){
		if (find(caminho.begin(), caminho.end(), i) == caminho.end() || caminho.size() == 1) {
			vector<double> vizinhos = grafo[i];
			sort(vizinhos.begin(), vizinhos.end());
			custo += vizinhos[0] + vizinhos[1];
		}
	}

	return ceil(custo/2);

}

/// @brief Executa o algoritmo Branch and Bound utilizando busca em largura adaptada (best-first search)
/// @param grafo Grafo completo ponderado e não direcionado respeitando a desigualdade triangular
/// @param n Número de vértices do grafo
/// @return Custo do menor caminho encontrado
double bnb_bfs(vector<vector<double>>& grafo, int n){
	Node raiz(bound(grafo, {0}), 0, 0, {0});
	priority_queue<Node> fila;
	fila.push(raiz);

	double melhorCusto = numeric_limits<double>::infinity();
	vector<int> melhorCaminho;

	while (!fila.empty()){
		Node node = fila.top();
		fila.pop();

		if (node.level > n-1){
			if (node.cost < melhorCusto){
				melhorCusto = node.cost;
				melhorCaminho = node.path;
			}
		}
		else if (node.bound < melhorCusto){
			if (node.level < n-1){
				for (int k = 0; k < n; k++){
					double custo = grafo[node.path[node.path.size()-1]][k];
					if (find(node.path.begin(), node.path.end(), k) == node.path.end() && custo != numeric_limits<double>::infinity()){
						vector<int> novoCaminho = node.path;
						novoCaminho.push_back(k);
						Node filho(bound(grafo, novoCaminho), node.level + 1, node.cost + custo, novoCaminho);
						fila.push(filho);
					}
				}
			}
			else if (grafo[node.path[node.path.size()-1]][0] != numeric_limits<double>::infinity() && node.path.size() == n){
				vector<int> novoCaminho = node.path;
				novoCaminho.push_back(0);
				Node filho(bound(grafo, novoCaminho), node.level + 1, node.cost + grafo[node.path[node.path.size()-1]][0], novoCaminho);
				fila.push(filho);
			}
		}
	}

	return melhorCusto;

}

/// @brief Executa o algoritmo Branch and Bound utilizando busca em profundidade
/// @param grafo Grafo completo ponderado e não direcionado respeitando a desigualdade triangular
/// @param n Número de vértices do grafo
/// @return Custo do menor caminho encontrado 
double bnb_dfs(vector<vector<double>>& grafo, int n) {
    Node raiz(bound(grafo, {0}), 0, 0, {0});
    stack<Node> pilha;
    pilha.push(raiz);

    double melhorCusto = numeric_limits<double>::infinity();
    vector<int> melhorCaminho;

    while (!pilha.empty()) {
        Node node = pilha.top();
        pilha.pop();

        if (node.level > n - 1) {
            if (node.cost < melhorCusto) {
                melhorCusto = node.cost;
                melhorCaminho = node.path;
            }
        } else if (node.bound < melhorCusto) {
            if (node.level < n - 1) {
                for (int k = 0; k < n; k++) {
                    double custo = grafo[node.path[node.path.size() - 1]][k];
                    if (find(node.path.begin(), node.path.end(), k) == node.path.end() &&
                        custo != numeric_limits<double>::infinity()) {
                        vector<int> novoCaminho = node.path;
                        novoCaminho.push_back(k);
                        Node filho(bound(grafo, novoCaminho), node.level + 1, node.cost + custo, novoCaminho);
                        pilha.push(filho);
                    }
                }
            } else if (grafo[node.path[node.path.size() - 1]][0] != numeric_limits<double>::infinity() &&
                       node.path.size() == n) {
                vector<int> novoCaminho = node.path;
                novoCaminho.push_back(0);
                Node filho(bound(grafo, novoCaminho), node.level + 1,
                           node.cost + grafo[node.path[node.path.size() - 1]][0], novoCaminho);
                pilha.push(filho);
            }
        }
    }

    return melhorCusto;
}

void testeInicial(){
	int n = 5;

	double inf = numeric_limits<double>::infinity();

	vector<vector<double>> grafo = {
		{inf, 3.0, 1.0, 5.0, 8.0},
	  	{3.0, inf, 6.0, 7.0, 9.0},
		{1.0, 6.0, inf, 4.0, 2.0},
		{5.0, 7.0, 4.0, inf, 3.0},
		{8.0, 9.0, 2.0, 3.0, inf}
	};

	double custo = bnb_bfs(grafo, n);

	cout << "Custo: " << custo << endl;
}

void moduloTestes(){
	// abre arquivo lista-datasets.txt
	ifstream arquivo("lista-datasets.txt");
	string linha;

	if (arquivo.is_open()){
		// Descarta a header
		getline(arquivo, linha);

		// Abre (ou cria) o arquivo de saída
		ofstream arquivoSaida("saida.csv", ios::app);
		if (!arquivoSaida) {
			arquivoSaida << "Arquivo;Algoritmo;Tempo;Custo;Aproximacao;Memoria" << endl;
		}
		arquivoSaida.close();
		
		while (getline(arquivo, linha)){
			// Extrai o nome do arquivo
			string nomeArquivo = linha.substr(0, linha.find("\t"));
			vector<int> valorOtimo = extrairNumeros(linha);
			valorOtimo.size() == 3 ? valorOtimo = {valorOtimo[1], valorOtimo[2]} : valorOtimo = {valorOtimo[1]};

			// Gera o grafo a partir do arquivo
			nomeArquivo = "datasets/" + nomeArquivo + ".tsp";
			vector<vector<double>> grafo;
			try{
				grafo = geraGrafo(nomeArquivo);
			}catch (const exception& e){
				cout << "Erro ao ler o arquivo " << nomeArquivo << endl;
				cout << e.what() << endl;
				continue;
			}

			// Executa o algoritmo
			double tempo;
			double custo;
			bool exato;
			double memoria;

			// Executa o algoritmo
			try{
				tempo = clock();
				custo = bnb_bfs(grafo, grafo.size());
				tempo = clock() - tempo;
				tempo = (clock() - tempo) / (double)CLOCKS_PER_SEC; // Converte para segundos
				tempo = round(tempo * 100000) / 100000;
				exato = valorOtimo.size() == 1 ? (custo == valorOtimo[0]) : (valorOtimo[0] <= custo && custo <= valorOtimo[1]);
				memoria = 0;

				// Escreve no arquivo de saída
				ofstream arquivoSaida("saida-bnb.csv", ios::app);
				arquivoSaida << nomeArquivo << ";Branch and bound;" << tempo << ";" << custo << ";" << exato << ";" << memoria << endl;
				arquivoSaida.close();
			}catch (const exception& e){
				cout << "Erro ao executar o algoritmo bnb no arquivo " << nomeArquivo << endl;
				cout << e.what() << endl;
			}
		}
		arquivo.close();
	}
}

int main(int argc, char const *argv[]){
	testeInicial();
	// moduloTestes();	
	return 0;
}