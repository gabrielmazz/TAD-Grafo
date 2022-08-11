#include <stdio.h>
#include <stdlib.h>
#include "module.h"

#ifdef __linux__
    #include <unistd.h>
#elif _WIN32
    #include <windows.h>
    #include <locale.h>
#endif

//Define se será digrafo ou não
#define DIGRAFO 1

int main(void){
    int vertices, grau;

    #ifdef _WIN32
      setlocale(LC_ALL, "Portuguese");
    #endif

    clear_screen();
    //Entradas padrões do grafo pedindo qual será o tamanho dos vertices e seus graus
    printf("Determine quantas vertices o grafo terá: ");
    scanf("%d", &vertices);

    printf("Determine quantos graus o grafo terá: ");
    scanf("%d", &grau);
    clear_screen();

    //Define o novo Grafo com (num_vertices, grau_max, ponderado)
    Grafo* gr = cria_grafo(vertices, grau, 0); 

    int switch01;
    int origem, destino, test;

    //Determina se terá um grafo aleatório, ou se o usuario irá escolher quais serão adicionado
    RETURN_ALG: printf("Algotirmo de manipulação de grafos\n\n");
    printf("Determine a criação do grafo:\n1 - Aleátorio\n2 - Inserção manual:\nR: ");
    scanf("%d", &switch01);
    clear_screen();

    switch(switch01){
      case(1):
        //Insere aleatoriamente um valor de '10 + rand() % 30' vertices no grafo
        insere_aleatorio(gr, grau);
        break;
      
      case(2):
          RETURN01: printf("Determine sua origem: ");
          scanf("%d", &origem);
          printf("Determine seu destino: ");
          scanf("%d", &destino);

          insere_aresta(gr, origem, destino, DIGRAFO, 0);
          clear_screen();

          RETURN_ALT: printf("Quer adicionar mais uma aresta?\n1 - Adicionar\n0 - Não adicionar\nR: ");
          scanf("%d", &test);

          if(test == 1){
            clear_screen();
            goto RETURN01;
          }else if(test == 0){
            clear_screen();
            break;
          }else{
            printf("Escolha uma alternativa...\n");
            stop_time(2);
            clear_screen();
            goto RETURN_ALT;
          }

      default:
        printf("Não adicionou nenhuma aresta, retornando no algoritmo...\n");
        stop_time(2);
        clear_screen();
        goto RETURN_ALG;
    }

    //Menu de modificações do grafo, toda manipulação será feita dentro deste switch
    int switch02, switch03;

    int vertice_inicial;      //Determinante de qual vertice iniciara a busca

    int visitados[vertices];  //Usado nas buscas em profundidade e de largura
    int anterior[vertices];
    float distancia[vertices];


    RETURN02: printf("Determine qual operação será feita:\n\n");
    printf("1 - Imprimir o grafo\n");
    printf("2 - Adicionar uma nova aresta\n");
    printf("3 - Remove uma aresta\n");
    printf("4 - Busca no grafo\n");
    printf("5 - Libera grafo\n");
    printf("R: "); scanf("%d", &switch02);
    clear_screen();

    switch(switch02){
      case(1):
        printf("Impressão do Grafo:\n\n");
        imprime_grafo(gr);
        setbuf(stdin, NULL);
        stop();
        clear_screen();
        goto RETURN02;
        break;
      
      case(2):
        printf("Determine sua origem: ");
        scanf("%d", &origem);
        printf("Determine seu destino: ");
        scanf("%d", &destino);

        insere_aresta(gr, origem, destino, DIGRAFO, 0);
        clear_screen();
        goto RETURN02;
        break;

      case(3):
        printf("Determine sua origem: ");
        scanf("%d", &origem);
        printf("Determine seu destino: ");
        scanf("%d", &destino);

        remove_aresta(gr, origem, destino, DIGRAFO);
        clear_screen();
        goto RETURN02;
        break;
      
      case(4):
        printf("Busca dentro do grafo:\n\n");
        printf("1 - Procurar o menor caminho\n");
        printf("2 - Busca em profundidade\n");
        printf("3 - Busca em largura\n");
        printf("R: "); scanf("%d", &switch03);

        switch(switch03){
          case(1):
            clear_screen();
            printf("Procura o menor caminho:\n");
            printf("\nDetermine o vertice inicial: ");
            scanf("%d", &vertice_inicial);

            clear_screen();
            menor_caminho_grafo(gr, vertice_inicial, anterior, distancia);
            
            //If/else apenas para deixa indexado o código na hora do print
            for(int i=0; i<grau; i++)
              if(anterior[i] < 0)
                printf("%d: %d -> {%.2f}\n", i, anterior[i], distancia[i]);
              else
                printf("%d:  %d -> {%.2f}\n", i, anterior[i], distancia[i]);

            setbuf(stdin, NULL);
            stop();

            clear_screen();
            goto RETURN02;
            break;

          case(2):
            clear_screen();
            printf("Busca em profundidade:\n");
            printf("\nDetermine o vertice inicial: ");
            scanf("%d", &vertice_inicial);

            clear_screen();
            busca_profundidade_grafo(gr, vertice_inicial, visitados);
            
            setbuf(stdin, NULL);
            stop();

            clear_screen();
            goto RETURN02;
            break;

          case(3):
            clear_screen();
            printf("Busca em profundidade:\n");
            printf("\nDetermine o vertice inicial: ");
            scanf("%d", &vertice_inicial);

            clear_screen();
            busca_largura_grafo(gr, vertice_inicial, visitados);
            
            setbuf(stdin, NULL);
            stop();

            clear_screen();
            goto RETURN02;
            break;
        }

        break;

      case(5):
        printf("Liberando grafo, aguarde...\n");
        stop_time(3);
        setbuf(stdin, NULL);
        break;
      
      default:
        printf("Operação inválida, retornando...\n");
        stop_time(2);
        clear_screen();
        goto RETURN02;
    }

    /*Finalização do programa, pedira para clicar em algum botão para continuar e finalizar,
      e logo em seguida dara um clear no terminal para sumir a execução apenas por bens esteticos*/
    clear_screen();
    return EXIT_SUCCESS;
}

/*DICIONÁRIO:
  - stop(); -> Para o programa, até o usuario tecle algum botão
  - stop_time(); -> Pausa o programa por um certo tempo
  - clear_screen(); -> Limpa o console para as proximas mensgans
  - setbuf(stdin, NULL); -> Limpa o buffer do teclado
*/