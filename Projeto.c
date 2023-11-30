#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// criando estrutura do tipo reg
typedef struct {
  char produto[20];
  char codigo[20];
  char preco[20];
  char status;
} reg;

// chamando as funções
int tamanho(FILE *);
void cadastrar(FILE *, int);
void consultar(FILE *);
void geraarqtxt(FILE *);

int main() {
  reg contato;
  int op, senha, a = 0;
  FILE *arq;

  // vendo se os arquivos existem
  if ((arq = fopen("dadu.dat", "rb+")) == NULL) {
    if ((arq = fopen("dadu.dat", "wb+")) == NULL) {
      printf("Falha ao abrir o arquivo!\n");
      system("pause");
    }
  }

  do {
    system("clear");
    printf("\n======= MENU ======= \n");
    printf("1.Cadastrar administrador.\n");
    printf("2.Cadastrar produto se for adiministrador.\n");
    printf("3.Consultar produto.\n");
    printf("4.Gerar catálogo.\n");
    printf("5.Sair\n");
    printf("====================\n");
    printf("Produtos cadastrados: %d\n", tamanho(arq));
    printf("\nOpcao:");
    scanf("%d", &op);

    // condição
    switch (op) {
    case 1: {
      int ops, senh;
      system("clear");
      printf("\nSenha padrão >> 0 <<\n\n");

      do {
        printf("=======MENU DO ADM=======\n1) Criar senha.\n2) Alterar "
               "senha.\n3) Excluir senha.\n4) Sair.\n========================");
        printf("\nOpção: ");
        scanf("%d", &ops);
        if (ops == 1 && a == 0) {
          {
            system("clear");
            printf("========Criar senha========\n");
            printf("Crie uma senha usando apenas números: ");
            scanf("%i", &senha);
          }
          system("clear");
          printf(">SENHA CRIADA COM SUCESSO!<\n\n");
          a = 2;
        } else if (ops == 1 && a == 2) {
          system("clear");
          printf(">Senha já criada.<\n");
        } else if (ops == 2) {
          system("clear");
          printf("========Alterar senha========\n");
          printf("Digite sua senha atual: ");
          scanf("%i", &senh);
          if (senh == senha) {
            {
              printf("\nDigite sua nova senha: ");
              scanf("%i", &senha);
            }
            system("clear");
            printf("SENHA ALTERADA COM SUCESSO!\n");
          } else if (senh != senha) {
            system("clear");
            printf("SENHA INCORRETA!\n");
          }
        } else if (ops == 3) {
          system("clear");
          printf("========Excluir senha========\n");
          printf("Digite sua senha atual: ");
          scanf("%i", &senh);

          if (senh == senha) {
            system("clear");
            senha = 0;
            printf("SENHA EXCLUÍDA COM SUCESSO (vá em 'Criar senha' e crie "
                   "outra!\n\n");
            a = 0;
          } else if (senh != senha) {
            system("clear");
            printf("SENHA INCORRETA!\n");
          }
        }
      } while (ops != 4);
    } break;
    case 2:
      cadastrar(arq, senha);
      break;
    case 3:
      consultar(arq);
      break;
    case 4:
      geraarqtxt(arq);
      break;

    case 5:
      fclose(arq);
      break;
    }
  } while (op != 5);

  return 0;
}

void cadastrar(FILE *arq, int a) {

  reg contato;
  char confirma;
  int senh;

  contato.status = 'S';
  fflush(stdin);
  system("clear");
  printf("========CADASTRAR PRODUTO========\n");
  printf("Entre com a senha: ");
  scanf("%i", &senh);
  if (senh == a) {
    system("clear");
    printf("=====Cadastrando novo produto=====\n\n");

    printf("Produto..........:");
    scanf("%c", &contato.produto);
    gets(contato.produto);
    printf("Código......:");
    gets(contato.codigo);
    printf("Preço........:");
    gets(contato.preco);
    printf("\nNumero de produtos registrados:%d\n", tamanho(arq) + 1);
    printf("\nConfirma <s/n>:");
    fflush(stdin);
    scanf("%c", &confirma);

    if (toupper(confirma) == 'S') {
      printf("\ngravando...\n\n");
      fseek(arq, 0, SEEK_END);
      fwrite(&contato, sizeof(reg), 1, arq);
    }
  } else {
    system("clear");
    printf("Senha incorreta!\n");
  }

  system("pause");
}

void consultar(FILE *arq) {
  reg contato;
  char codigo[20];
  int found = 0;
  char sair = 's';
  system("clear");
  printf("\n=====Consulta do produto pelo código=====\n");
  printf("\nInforme o Código...:");
  scanf("%s", codigo);
  fseek(arq, 0, SEEK_SET);

  do {
    while (fread(&contato, sizeof(reg), 1, arq) == 1) {
      if (strcmp(contato.codigo, codigo) == 0 && contato.status != 'N') {
        found = 1;
        printf("\nProduto......:%s", contato.produto);
        printf("\nPreço........:%s\n\n", contato.preco);
        printf("\nPara voltar digite s: ");
        scanf("%s", &sair);
        break;
        system("pause");
      }
    }

    if (!found) {
      printf("\nProduto não encontrado ou excluído!\n");
      printf("\nPara voltar digite s: ");
      scanf("%s", &sair);
    }
  } while (sair != 's');
}

void geraarqtxt(FILE *arq) {
  char nomearq[20];
  int nr;
  reg contato;
  system("clear");
  printf("=====Cátalogo=====\n\n");
  printf("Qual o nome deseja dar para o catalogo:");
  scanf("%s", nomearq);
  strcat(nomearq, ".txt");
  FILE *arqtxt = fopen(nomearq, "w");

  if (!arqtxt) {
    printf("\nNao foi possivel criar esse arquivo!\n");
    system("pause");
  }

  fprintf(arqtxt,
          "Produto           Código        Preço                   Status\n");
  fprintf(arqtxt,
          "================================================================\n");

  for (nr = 0; nr < tamanho(arq); nr++) {
    fseek(arq, nr * sizeof(reg), SEEK_SET);
    fread(&contato, sizeof(reg), 1, arq);
    fprintf(arqtxt, "%-20s%-12s%-25s %c\n", contato.produto, contato.codigo,
            contato.preco, contato.status);
  }
  fprintf(arqtxt,
          "================================================================\n");
  fclose(arqtxt);
}

int tamanho(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  return ftell(arq) / sizeof(reg);
}
