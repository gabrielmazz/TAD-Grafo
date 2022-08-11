#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "module.h"

#ifdef __linux__
    #include <unistd.h>
#elif _WIN32
    #include <windows.h>
#endif

//Define se será digrafo ou não
#define DIGRAFO 1

//Definição do meu tipo Grafo
struct grafo{
    int ponderado;      //Info se ele será ponderado
    float** pesos;      //Caso seja um grafico ponderado, será uma matriz dinamica que guardara os valores
    int num_vertices;   //Número de vertices que o grafo contera
    int grau_max;       //Definira quantas ligações cada vertice poderá conter
    int** arestas;      //Guarda minhas conexões sendo uma matriz dinamica
    int* grau;          //Definira um vetor pra quantos ligações ele já fez
};

/*Função criador do grafo, alocando dinamicamente toda a matriz de arestas e se caso for
  a matriz de pesos tambem, passando o numero de vertices, o grau maximo que será alcançado
  e se caso for 1, o grafo será ponderado, se for 0 não será*/
Grafo* cria_grafo(int num_vertices, int grau_max, int ponderado){
    Grafo *gr;

    gr = (Grafo*) malloc(sizeof(struct grafo));     //Aloca a minha estrutura Grafo
    
    //Caso passe do teste, ai sim teremos a alocação por definição
    if(gr != NULL){     //Primeiro teste se não houve erro na minha criação e alocação do Grafo

        gr->num_vertices = num_vertices;    //Guarda na struct o num_vertices
        gr->grau_max = grau_max;            //Guarda na struct o grau_max

        //Define se o grafo é ponderado para passar para a struct
        if(ponderado != 0)
            gr->ponderado = 1;  //Sim ele é ponderado
        else
            gr->ponderado = 0;  //Não ele não é ponderado

        gr->grau = (int*) calloc(num_vertices,sizeof(int)); //Aloca o vetor de graus que serão as ligações ja feitas

        //Criação da matriz de arestas feita com ponteiro de ponteiro
        gr->arestas = (int**) malloc(num_vertices * sizeof(int*));  //Aloca a primeira linha de vetores
        for(int i=0; i<num_vertices; i++)
            gr->arestas[i] = (int*) malloc(grau_max * sizeof(int)); //Aloca as 'segundas' linhas de vetores com o grau_max passado acima

        //Caso o grafo seja ponderado, teresmo novamente a matriz dinamica para guardarmos os pesos de cada aresta
        if(gr->ponderado == 1){
            gr->pesos = (float**) malloc(num_vertices * sizeof(float*));  //Aloca a primeira linha de vetores
            for(int i=0; i<num_vertices; i++)
                gr->pesos[i] = (float*) malloc(grau_max * sizeof(float)); //Aloca todo o tamanho que poderá conter os pesos
        }
    }

    return gr;
}

/*Função que será responsável por excluir o grafo da memória, todas as estruturas alocadas
  dinamicamente, serão desalocado usando free(), sejá a matriz de arestas, e até mesmo as
  de peso*/
void libera_grafo(Grafo* gr){
    //Testa se o grafo existe mesmo, caso sim entrará na função de fato
    if(gr != NULL){
        //For que desalocara a matriz de arestas
        for(int i=0; i<gr->num_vertices; i++)
            free(gr->arestas[i]);
        free(gr->arestas);

        //For que desalocara a matriz de pesos
        if(gr->ponderado){
            for(int i=0; i<gr->num_vertices; i++)
                free(gr->pesos[i]);
            free(gr->pesos);
        }

        free(gr->grau); //Desaloca o vetor das ligações
        free(gr);       //Desaloca de fato o grafo
    }
}

/*Função que será responsável para adicionarmos uma nova aresta no grafo alocado, para isso
  passamos qual será o grafo, aonde será sua origem por exemplo 0, 1, 2 da matriz dinamica
  e qual será o seu destino final no vetor. Caso existe algum peso tambem será passado para
  a adicição na outra matriz responsável*/
int insere_aresta(Grafo* gr, int origem, int destino, int digrafo, float peso){
    //Testa se o grafo está criado mesmo e alocado
    if(gr == NULL)
        return 0;

    /*Verifica se os vertices existem, sendo que ele não pode ser 0 e nem maior que
      meu número de vertices*/
    if(origem < 0 || origem >= gr->num_vertices)
        return 0;

    //Verifica se aonde eu quero inserir a arestas existe de fato no meu grafo
    if(destino < 0 || destino >= gr->num_vertices)
        return 0;

    /*Ocorrera a inserção no final da minha matriz determinado pelo seu destino,
      isso ocorrera tambem no grafo ponderado aonde ele estara sendo colocado*/
    gr->arestas[origem][gr->grau[origem]] = destino;

    //Teste se irá mesmo ser colocada na matriz de pesos
    if(gr->ponderado)
        gr->pesos[origem][gr->grau[origem]] = peso;
    gr->grau[origem]++; //Aumenta para a proxima posição da matriz

    /*Insere outra aresta se não for digrafo, passando de forma diferente passando
      de ordem inversa, do destino e da origem
      - Executara apenas uma vez*/
    if(digrafo == 0)
        insere_aresta(gr, destino, origem, 1, peso);
    return 1;
}

/*Função que ficara responsável para removermos uma determinada aresta sendo muito semelhante
  a própria inserção, na assinatura passaremos sua origem e seu destino para sabermos exatamente
  aonde removermos tal aresta*/
int remove_aresta(Grafo* gr, int origem, int destino, int digrafo){
    //Testa se o grafo está criado mesmo e alocado
    if(gr == NULL)
        return 0;

    /*Verifica se os vertices existem, sendo que ele não pode ser 0 e nem maior que
      meu número de vertices*/
    if(origem < 0 || origem >= gr->num_vertices)
        return 0;

    //Verifica se aonde eu quero inserir a arestas existe de fato no meu grafo
    if(destino < 0 || destino >= gr->num_vertices)
        return 0;

    /*While que será responsável para acharmos a aresta que será removida, percorrendo
      o grafo até conseguir achar a sua parada*/
    int i = 0;
    while(i<gr->grau[origem] && gr->arestas[origem][i] != destino)
        i++;

    /*Caso a condição saia do laço, podemos interpretar que está aresta não existe dentro
      do grafo e simplesmente saira da função*/
    if(i == gr->grau[origem])
        return 0;

    /*Diminui uma aresta e desloca o ultimo valor para frente, é uma forma de
      otimização do código para que não precisar deslocar todos os valores*/
    gr->grau[origem]--; 	
    gr->arestas[origem][i] = gr->arestas[origem][gr->grau[origem]];
    
    //Irá fazer a mesma coisa caso seja ponderado o grafo
    if(gr->ponderado)
        gr->pesos[origem][i] = gr->pesos[origem][gr->grau[origem]];

    /*Remove outra aresta se não for digrafo, passando recursivamente de forma diferente
      novamente invertendo o destino e origem
      - Executando uma vez apenas*/
    if(digrafo == 0)
        remove_aresta(gr, destino, origem, 1);
    return 1;
}

/*Função que será responsável pela a impressão do grafo, percorrendo o num_vertices e posteriomente
  os seus graus*/
void imprime_grafo(Grafo *gr){
    //Testa se o grafo está criado e alocado
    if(gr == NULL)
        return;

    /*For que ficará responsável pelo o print do grafo, no 'i', percorrera todas os
      seus vertices, mostando qual vertice está sendo printado. No 'j' mostrara suas
      ligações, mais especifico suas lista de adjacencia alocadas*/
    for(int i=0; i < gr->num_vertices; i++){
        printf("%d: ", i);
        for(int j=0; j < gr->grau[i]; j++){
            if(gr->ponderado)
                printf("{%d(%.2f)} ", gr->arestas[i][j], gr->pesos[i][j]);
            else
                printf("{%d} ", gr->arestas[i][j]);
        }
        printf("\n");
    }
}

/*============================================================================*/
//ALGORITMOS DE BUSCA EM GRAFO

/*Busca em profundidade parti de um vértice inicial e irá explorar o máximo possível em todos os seus
  ramos antes de retornar, pode ser usado para encontrar componentes conectados e fortemente conectados
  - Usado para achar componentes conectados e fortemente conectados, ordenação topologica de um grafo
    resolver quebra-cabeças como um labirinto*/

/*Função principal aonde fará a interface com o usuário*/
void busca_profundidade_grafo(Grafo *gr, int inicial, int *visitado){
    int cont = 1;

    /*Marca todos os vertices como não visitados
      Isso é feito apenas para evitar que o usuario faça algo que não deve*/
    for(int i=0; i<gr->num_vertices; i++)
        visitado[i] = 0;

    //Chama de fato a busca na sua função auxiliar, passando todos os parametros necessario
    busca_profundidade(gr, inicial, visitado, cont);

    //Printa a busca
    printf("Busca em profundidade:\n\n");
    for(int i=0; i<gr->num_vertices; i++)
        printf("%d -> {%d}\n",i ,visitado[i]);
}

/*Função auxiliar, aonde realizara o cálculo determinado para a busca dentro do grafo*/
void busca_profundidade(Grafo *gr, int inicial, int *visitado, int cont){
    //Vertice recebera que a aresta já foi visitado, passando dentro do vetor, caso seja != 0, não foi visitado
    visitado[inicial] = cont;
    
    //Parte para a verificação de todos os visinhos deste vertice
    for(int i=0; i<gr->grau[inicial]; i++){
        /*Verifica se o vertice já foi visitado, se não foi retorna novamente recursivamente a busca em profundidade
          incrementando o cotador*/
        if(!visitado[gr->arestas[inicial][i]])
            busca_profundidade(gr,gr->arestas[inicial][i],visitado,cont+1);
    }
}

/*Busca em largura parti de um vértice inicial, explorando todos os vértice vizinhos, em seguida para todos
  os vértices vizinhos repetira o processo, assim olhando todos que estão inexplorado
  - Usado para achar componentes conectados, achar todos os vertices conectado, achar o menor caminho
    testar bipartição em grafo*/

void busca_largura_grafo(Grafo *gr, int inicial, int *visitado){
    int vert, NV, cont = 1;
    int *fila, IF = 0, FF = 0;  //IF = inicio da fila, FF = final da fila

    //Marca todos os vértices como não visitados
    for(int i=0; i<gr->num_vertices; i++)
        visitado[i] = 0;

    /*Cria uma fila, para saber a ordem em que foi visitados os vértices*/
    NV = gr->num_vertices;
    fila = (int*) malloc(NV * sizeof(int)); //Fila alocada de forma estática com mesmo tamanho numero de vertices
    FF++;                       //Incrementa a fila
    fila[FF] = inicial;         //Coloca o vertice inicial no final da fila
    visitado[inicial] = cont;   //Marca o vertice como visitado

    //Enquanto a fila não estiver vázia
    while(IF != FF){     
        //Recupera o vertice que está no inicio da fila  
        IF = (IF + 1) % NV;     
        vert = fila[IF];
        cont++;

        //Visita todos os vizinhos que ainda não foram visitados e coloca na fila alocada
        for(int i=0; i<gr->grau[vert]; i++){
            //Verifica se foram visitados ou não
            if(!visitado[gr->arestas[vert][i]]){
                FF = (FF + 1) % NV;                     //Adiciona ele na fila
                fila[FF] = gr->arestas[vert][i];
                visitado[gr->arestas[vert][i]] = cont;  //Marca como visitado no vetor
            }
        }
    }

    free(fila);     //Libera o vetor da fila auxiliar 

    //Printa o vetor visitado
    printf("Busca em largura:\n\n");
    for(int i=0; i < gr->num_vertices; i++)
        printf("%d -> {%d}\n", i, visitado[i]);
}

/*Busca pelo menor caminho parti de um vértice inicial e calcula a menor distancia desse vértice a todos os
  demais, desde que exista um caminho entre eles
  - Resolve problemas como algoritmo de dijkstra
    - Resolve esse problema para grafos dirigidos ou não dirigidos com arestas de peso*/

void menor_caminho_grafo(Grafo *gr, int inicial, int *anterior, float *distancia){
    int cont, NV, ind, *visitado, u;
    cont = NV = gr->num_vertices;

    /*Cria um vetor auxiliar que será inicializado as distancias e seus anteriores
      e como não existe distancia negativa, eles não foram visitados de fato*/
    visitado = (int*) malloc(NV * sizeof(int));
    for(int i=0; i < NV; i++){
        anterior[i] = -1;
        distancia[i] = -1;
        visitado[i] = 0;
    }

    distancia[inicial] = 0;
    /*Se ainda existir vertices que não foram visitados, o while continuara rodando
      e assim marcando como visitados*/
    while(cont > 0){
        u = procura_menor_distancia(distancia, visitado, NV);
        if(u == -1)
            break;

        visitado[u] = 1;        //Marca como visitado
        cont--;                 //Visitado mais um vertice por isso decrementa

        //Atualiza a nova distancia dos vizinhos
        for(int i=0; i<gr->grau[u]; i++){
            ind = gr->arestas[u][i];
            //Testa se o ind chegou, caso não tenha chegado
            if(distancia[ind] < 0){
               distancia[ind] = distancia[u] + 1;//ou peso da aresta
               anterior[ind] = u;
            //Se caso já tenha chegado, colocamos assim
            }else{
                if(distancia[ind] > distancia[u] + 1){
                    distancia[ind] = distancia[u] + 1;//ou peso da aresta
                    anterior[ind] = u;
                }
            }
        }
    }

    free(visitado); //Desaloca o vetor auxiliar
}

/*Procura os vértices com a menor distancia e que não tenham sido visitados, passando
  a distancia que será dada, seus visitados e o numeros de vertices*/
int procura_menor_distancia(float *distancia, int *visitado, int NV){
    int menor = -1, primeiro = 1;
    
    for(int i=0; i < NV; i++){
        /*Caso o vertices não foi visitado, ele estará sendo setado com -1 pois não existe distancia negativa
          exceto o vertice inicial*/
        if(distancia[i] >= 0 && visitado[i] == 0){
            if(primeiro){
                menor = i;
                primeiro = 0;
            }else{
                if(distancia[menor] > distancia[i])
                    menor = i;
            }
        }
    }
    return menor;
}

/*============================================================================*/

//Funções extras apenas para perfumarias do programa
void clear_screen(void){
    //Comando para os dois sistemas "apagarem a tela"
    #ifdef __linux__
        system("clear");
    #elif _WIN32
        system("cls");
    #else
    #endif
}

//Instruções para pausar a execução do programa para que o usuario continue
void stop(void){
    fflush(stdin);
    printf("\nPressione qualquer tecla para continuar. . ."); 
    fflush(stdin);
    getchar();
}

//Instrução para pausar a execução do programa por um tempo passado na função
void stop_time(int tempo){
    #ifdef __linux__
        sleep(tempo);
    #elif WIN32
        Sleep(tempo);
    #else
    #endif
}

/*Função de inserção aleatoriamente para uma menor poluição da main() já que
  utilizamos várias inserções*/
void insere_aleatorio(Grafo* gr, int GRAU){
    srand(time(NULL));

    for(int i=0; i<10 + rand() % 30; i++){
        insere_aresta(gr, rand() % GRAU, rand() % GRAU, DIGRAFO, 0);
    }
}