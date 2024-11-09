// Integrantes:
// Daniel Soares    RA:2760482411011
// Natalia Raissa   RA:2760482411013

// Este programa permite o cadastramento, inativação e visualização dos veículos
// ativos do estacionamento.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Registro para armazenar a placa, modelo, cor e status do veículo
typedef struct {
  char placa[8];
  char modelo[51];
  char cor[21];
  int status;
} Registro;

// Funções responsáveis por adicionar, deletar e listar os carros
void addCar(FILE *arquivo);
void deleteCar(FILE *arquivo);
void listCar(FILE *arquivo);

int main() {
  // Declara um ponteiro associado ao arquivo "Registro.bin" e abre-o no modo de
  // leitura e escrita
  FILE *arquivo = fopen("Registro.bin", "r+b");
  // Declara a variável 'escolha', para o usuário selecionar uma opção e limpa a
  // tela
  int escolha = 0;
  system("cls");

  printf("Bem vindo ao cadastro de veiculos!\n\nQue acao deseja realizar?\n\n");

  // Estrutura de repetição que garante que o usuário selecione entre 1, 2, ou
  // 3, caso contrário, exibe uma mensagem de erro até que o usuário selecione
  // uma opção válida
  while (escolha < 1 || escolha > 3) {
    printf("1.Adicionar carro ao cadastro\n2.Inativar um carro no "
           "cadastro\n3.Vizualizar os carros ativos\n\n");
    printf("Digite o numero da acao escolhida:\n");
    scanf("%d", &escolha);

    if (escolha < 1 || escolha > 3) {
      system("cls");
      printf("ERRO: Escolha um numero correspondente a uma acao:\n\n");
    } else {
      printf("Acao %d selecionada\n", escolha);
    }
  }

  // Estrutura de condição, que chama a função que adiciona, inativa ou lista os
  // carros, dependendo da 'escolha' selecionada pelo usuário
  switch (escolha) {
  case 1:
    addCar(arquivo);
    break;

  case 2:
    deleteCar(arquivo);
    break;

  case 3:
    listCar(arquivo);
    break;
  }

  // fecha o arquivo
  fclose(arquivo);
  return 0;
}

// Função responsável por adicionar carros
void addCar(FILE *arquivo) {
  // Cria um registro auxiliar e limpa a tela
  Registro aux, arq;
  system("cls");
  // Pede a inserção das informações do veículo (placa, modelo e cor) através da
  // função 'fgets'
  printf("\n\nRegistro de veiculo iniciado\n\n");

  printf("Insira a Placa do novo veiculo: ");
  // remove o '\n'
  getchar();
  fgets(aux.placa, 8, stdin);

  // Verifica se o carro já existe no sistema, comparando a placa inserida com as placas presentes no arquivo, caso existir pede a inserção da placa novamente e volta o cursor ao início do arquivo
  while ((fread(&arq, sizeof(Registro), 1, arquivo)) != 0) {
    if (strcmp(arq.placa, aux.placa) == 0) {
      printf("Veiculo ja existe no sistema, insira a placa novamente: ");
      getchar();
      fgets(aux.placa, 8, stdin);
      rewind(arquivo);
    }
  }

  printf("Insira o Modelo do novo veiculo: ");
  getchar();
  fgets(aux.modelo, 51, stdin);
  // substitui o '\n' da string por '\0'
  aux.modelo[strlen(aux.modelo) - 1] = '\0';
  printf("Insira o Cor do novo veiculo: ");
  fgets(aux.cor, 21, stdin);
  aux.cor[strlen(aux.cor) - 1] = '\0';
  // automaticamente considera o carro como ativo
  aux.status = 1;

  // posiciona o cursor no final do arquivo
  fseek(arquivo, 0, SEEK_END);
  // adicona o registro do novo carro ao final do arquivo
  fwrite(&aux, sizeof(Registro), 1, arquivo);
  // posiciona o cursor no início do arquivo
  rewind(arquivo);
}

// Função responável por inativar carros
void deleteCar(FILE *arquivo) {
  // Cria um registro, uma string 'placa' e limpa a tela
  Registro aux;
  char placa[8];
  system("cls");

  printf("Qual a placa do veiculo a ser deletado: ");
  // remove o '\n'
  getchar();
  // Armazena a placa do veículo que deve ser inativado em 'placa'
  fgets(placa, 8, stdin);

  // O for lê um registro do arquivo por vez enquanto não chegar ao final do
  // arquivo ou enquanto não entrar na condição
  for (int i = 0; (fread(&aux, sizeof(Registro), 1, arquivo)) != 0; i++) {
    // Compara se a string 'placa' e 'aux.placa' são iguais
    if (strcmp(placa, aux.placa) == 0) {
      // Define o 'aux.status' como zero
      aux.status = 0;
      // Posiciona o cursor no registro do carro que deve ser inativado
      fseek(arquivo, i * sizeof(Registro), SEEK_SET);
      // Sobrescreve o status do carro com zero
      fwrite(&aux, sizeof(Registro), 1, arquivo);
      break;
    }
  }
}

// Função responsável por listar os carros ativos
void listCar(FILE *arquivo) {
  Registro aux;

  // Lê cada registro de um veículo por vez até chegar no final do arquivo
  while ((fread(&aux, sizeof(Registro), 1, arquivo)) != 0) {
    // Imprime os dados do carro caso ele esteja ativo
    if (aux.status == 1) {
      printf("\nPlaca: %s\nModelo: %s\nCor: %s\n", aux.placa, aux.modelo,
             aux.cor);
    }
  }
}