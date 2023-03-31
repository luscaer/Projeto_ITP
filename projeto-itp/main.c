//./uno bot_A bot_B bot_Grupo -h (Help dos comandos) -v passo a passo
// :D
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Constantes para as strings a serem lidas:
#define MAX_LINE 100
#define MAX_ACTION 10
#define MAX_ID_SIZE 10

// Debug:
void debug(char *message) {
  fprintf(stderr, "%s\n", message);
}

//Função para remover os colchetes da string da mão inicial:
void removedor_de_Colchete(char* string){
  for(int i = 0; i < strlen(string); i++)
    if(string[i] == '['){
      for(int j = i; j < strlen(string); j++)
        string[j] = string[j+2];
      string[(strlen(string)-2)] = '\0';
    }
}

//Função para criar um vetor de strings para melhorar a busca nas cartas na mão:
char** organizar_Mao (int tam, char* string){
  char** vets = calloc(tam, sizeof(char*));
  for(int i = 0; i < tam; i++)
    vets[i] = calloc(MAX_ID_SIZE, sizeof(char));

  char* pedaco;
  pedaco = calloc(MAX_ID_SIZE, sizeof(char));
  int j = 0;
  pedaco = strtok(string, " ");
  
  while(pedaco != NULL){
    strcpy(vets[j], pedaco);
    pedaco = strtok(NULL, " ");
    j++;
  }

  return vets;
}

int procurar_Carta(char* carta_Atual, char** Mao, int tam, int *pos) {
  for(int i = 0; i < tam; i++){
    if(Mao[i][0] == carta_Atual[0]){
      *pos = i;
      return 1;
    }
    else if(Mao[i][1] == carta_Atual[1] && Mao[i][2] == carta_Atual[2] && Mao[i][3] == carta_Atual[3]){
      *pos = i;
      return 1;
    }
  }

  return 0;
}

//Função para realocar memória quando comprar:
char** realocar_Memoria(int tam, int num, char** Mao, char* acao){
  Mao = realloc(Mao, sizeof(char*) * (tam+num));
  for(int i = tam; i < (tam+num); i++){
    Mao[i] = calloc(MAX_ID_SIZE, sizeof(char));
  }
  printf("%s %d\n", acao, num);
  for(int i = tam; i < tam+num; i++){
    scanf(" %s\n", Mao[i]);
  }
  return Mao;
}

// Função Main
int main() {
  // Varíavel para a leitura dos jogadores da partida:
  char PLAYERS[MAX_LINE];
  // Variável para a leitura do ID do nosso bot:
  char meu_ID[MAX_ID_SIZE];
  // Variável para leitura da mão recebida:
  char mao_Inicial[MAX_LINE];
  // Variável relacionada a mão do jogador:
  char** minha_Mao;
  //Tamanho padrão da mão inicial:
  int tamanho_da_Mao = 7;
  //Variável para a leitura da carta sobre a mesa:
  char carta_da_Mesa[MAX_ID_SIZE];

  setbuf(stdin, NULL);   // stdin, stdout e stderr não terão buffers
  setbuf(stdout, NULL);  // assim, nada é "guardado temporariamente"
  setbuf(stderr, NULL);

  // === INÍCIO DA PARTIDA === //

  //Leitura dos dados dos jogadores:
  scanf("PLAYERS %[^\n]\n", PLAYERS);
  //debug(PLAYERS);

  //Leitura do próprio ID:
  scanf("YOU %s\n", meu_ID);
  //debug(meu_ID);

  //Leitura da mão recebida:
  scanf("HAND %[^\n]\n", mao_Inicial);
  //Uma função para remover os colchetes:
  removedor_de_Colchete(mao_Inicial);
  //debug(mao_Inicial); 

  //Função para organizar a mão em um vetor de strings:
  minha_Mao = organizar_Mao(tamanho_da_Mao, mao_Inicial);
  /*for(int i = 0; i < tamanho_da_Mao; i++){
    debug(minha_Mao[i]);
  }*/

  //Leitura da carta sobre a mesa:
  scanf("TABLE %s\n", carta_da_Mesa);
  //debug(carta_da_Mesa);

  // === PARTIDA ===

  //Variável para receber a ação do jogador atual
  char action[MAX_ACTION];
  //Variavel da ação antiga:
  char aux_Action[MAX_ACTION];
  //Variável para receber uma carta caso tenha a ação de DISCARD:
  char complement[MAX_LINE]; 
  //Variável para receber a mudança de naipe, caso seja lançada uma carta C ou A
  char complement2[MAX_LINE];

  //Recebimento e saída de dados:
  while(1) {
    //Aguardando a vez do bot:
    do {
      //Leitura da ação e, caso seja um DISCARD, leitura da carta:
      strcpy(aux_Action, action);
      scanf("%s %s", action, complement);
      if(strcmp(action, "DISCARD") == 0){
        //Caso tenha mudança de naipe:
        if(complement[0] == 'A' || complement[0] == 'C'){
          scanf(" %s", complement2);
          complement[1] = complement2[0];
          complement[2] = complement2[1];
          complement[3] = complement2[2]; 
          strcpy(carta_da_Mesa, complement);
          //debug(carta_da_Mesa);
        }
        else{
          strcpy(carta_da_Mesa, complement);
          //debug(carta_da_Mesa);
        }
      }
      
    } while (strcmp(action, "TURN") || strcmp(complement, meu_ID));
    
    //Posição da carta a ser jogada:
    int posicao_Carta;

    //Caso a carta em cima da mesa seja um +4 e nós somos os felizardos:
    if(carta_da_Mesa[0] == 'C' && strcmp(aux_Action, "DISCARD") == 0){
      strcpy(action, "BUY");
      minha_Mao = realocar_Memoria(tamanho_da_Mao, 4, minha_Mao, action);
      tamanho_da_Mao += 4;
    }
    //Caso a carta em cima da mesa seja um +2 e nós somos os felizardos:
    else if(carta_da_Mesa[0] == 'V' && strcmp(aux_Action, "DISCARD") == 0){
      strcpy(action, "BUY");
      minha_Mao = realocar_Memoria(tamanho_da_Mao, 2, minha_Mao, action);
      tamanho_da_Mao += 2;
    }
    //Caso tenha carta para descartar:
    else if(procurar_Carta(carta_da_Mesa, minha_Mao, tamanho_da_Mao, &posicao_Carta)){
        strcpy(action, "DISCARD");     
        printf("%s %s", action, minha_Mao[posicao_Carta]);

        if (minha_Mao[posicao_Carta][0] == 'C' || minha_Mao[posicao_Carta][0] == 'A') {
          printf(" ♥\n");
        }
        else printf("\n");
      
        for(int i = posicao_Carta; i < tamanho_da_Mao-1; i++)
          strcpy(minha_Mao[i], minha_Mao[i+1]);
        
        tamanho_da_Mao--;
        //free(minha_Mao[tamanho_da_Mao]);
        minha_Mao = realloc(minha_Mao, sizeof(char*) * (tamanho_da_Mao));
    }
    //Caso não tenha cartas e tenha que comprar:
    else if(procurar_Carta(carta_da_Mesa, minha_Mao, tamanho_da_Mao, &posicao_Carta) == 0){
        strcpy(action, "BUY");
        minha_Mao = realocar_Memoria(tamanho_da_Mao, 1, minha_Mao, action);
        tamanho_da_Mao ++;
    }
  }

  return 0;
}