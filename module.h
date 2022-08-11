//Declaração da minha estrutura Grafo
typedef struct grafo Grafo;

//Definição do Grafo
Grafo* cria_grafo(int num_vertices, int grau_max, int ponderado);
void libera_grafo(Grafo* gr);
int insere_aresta(Grafo* gr, int origem, int destino, int digrafo, float peso);
int remove_aresta(Grafo* gr, int origem, int destino, int digrafo);
void imprime_grafo(Grafo *gr);

//Buscas do grafo
void busca_profundidade(Grafo *gr, int inicial, int *visitado, int cont);
void busca_profundidade_grafo(Grafo *gr, int inicial, int *visitado);
void busca_largura_grafo(Grafo *gr, int inicial, int *visitado);
int procura_menor_distancia(float *distancia, int *visitado, int NV);
void menor_caminho_grafo(Grafo *gr, int inicial, int *anterior, float *distancia);

void clear_screen(void);
void stop(void);
void stop_time(int tempo);
void insere_aleatorio(Grafo* gr, int GRAU);