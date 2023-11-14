#include <stdbool.h>
#include <stdio.h>

#define MAX_TAM 100

typedef struct {
  int x;
  int y;
} Point;

bool limite(int x, int y, int altura, int largura) {
  return (x >= 0 && x < altura && y >= 0 && y < largura);
}

void cercar_comedor_de_humano(char mapa[MAX_TAM][MAX_TAM], int altura, int largura) {
  int dx[] = {0, 0, 1, -1, 1, -1, 1, -1};
  int dy[] = {1, -1, 0, 0, 1, -1, -1, 1};

  for (int i = 0; i < altura; i++) {
    for (int j = 0; j < largura; j++) {
      if (mapa[i][j] == '*') {
         if (i > 0 && mapa[i - 1][j] == '!') {
          printf("Saída -1: Tesouro ao lado de um canibal.\n");
           exit(1);
         
        }
        if (i < altura - 1 && mapa[i + 1][j] == '!') {
          printf("Saída -1: Tesouro ao lado de um canibal.\n");
         exit(1);
        }
        if (j > 0 && mapa[i][j - 1] == '!') {
          printf("Saída -1: Tesouro ao lado de um canibal.\n");
         exit(1);
        }
        if (j < largura - 1 && mapa[i][j + 1] == '!') {
          printf("Saída -1: Tesouro ao lado de um canibal.\n");
         exit(1);
        }
        if (i > 0 && j > 0 && mapa[i - 1][j - 1] == '!') {
          printf("Saída -1: Tesouro ao lado de um canibal.\n");
         exit(1);
        }
        if (i > 0 && j < largura - 1 && mapa[i - 1][j + 1] == '!') {
          printf("Saída -1: Tesouro ao lado de um canibal.\n");
         exit(1);
        }
        if (i < altura - 1 && j > 0 && mapa[i + 1][j - 1] == '!') {
          printf("Saída -1: Tesouro ao lado de um canibal.\n");
         exit(1);
        }
        if (i < altura - 1 && j < largura - 1 &&
            mapa[i + 1][j + 1] == '!') {
          printf("Saída -1: Tesouro ao lado de um canibal.\n");
         exit(1);
        }

        for (int k = 0; k < 8; k++) {
          int nx = i + dx[k];
          int ny = j + dy[k];

          if (limite(nx, ny, altura, largura) && mapa[nx][ny] != '@') {
            mapa[nx][ny] = '#';
          }
        }
      }
    }
  }
}

int busca_profunda(char mapa[MAX_TAM][MAX_TAM], int altura, int largura, Point inicio,
        Point end) {
  int dx[] = {0, 0, 1, -1};
  int dy[] = {1, -1, 0, 0};
  bool visitado[MAX_TAM][MAX_TAM] = {{false}};
  int tempo[MAX_TAM][MAX_TAM] = {{0}};

  Point fila[MAX_TAM * MAX_TAM];
  int ida = 0, volta = 0;

  fila[volta++] = inicio;
  visitado[inicio.x][inicio.y] = true;

  while (ida < volta) {
    Point atual = fila[ida++];
    int x = atual.x;
    int y = atual.y;

    for (int i = 0; i < 4; i++) {
      int nx = x + dx[i];
      int ny = y + dy[i];

      if (limite(nx, ny, altura, largura) && !visitado[nx][ny] &&
          mapa[nx][ny] != '#' && mapa[nx][ny] != '~') {
        fila[volta++] = (Point){nx, ny};
        visitado[nx][ny] = true;
        tempo[nx][ny] = tempo[x][y] + 1;
      }
    }
  }

  return tempo[end.x][end.y];
}

void imprime_mapa_do_tesouro(char mapa[MAX_TAM][MAX_TAM], int altura, int largura) {
  for (int i = 0; i < altura; i++) {
    for (int j = 0; j < largura; j++) {
      printf("%c ", mapa[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

bool tesouro_livre(char mapa[MAX_TAM][MAX_TAM], int altura, int largura) {
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            if (mapa[i][j] == '!') {

                if (mapa[i - 1][j] != '.' && mapa[i - 1][j] != '!' && mapa[i - 1][j] != '@' && mapa[i + 1][j] != '.' &&  mapa[i + 1][j] != '!' && mapa[i + 1][j] != '@' && mapa[i][j - 1] != '.' && mapa[i][j - 1] != '!' && mapa[i][j - 1] != '@' && mapa[i][j + 1] != '.' && mapa[i][j + 1] != '!' && mapa[i][j + 1] != '@'){
                    return true;
                }
            }
        }
    }
  
    return false;
}


int main() {
  FILE *file = fopen("mapa.txt", "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo mapa.txt.\n");
    return 1;
  }

  int altura, largura, num_mapa = 1;

  while (fscanf(file, "%d %d", &altura, &largura) == 2 && altura > 0 &&
         largura > 0) {
    char mapa[MAX_TAM][MAX_TAM];
    char originalMapa[MAX_TAM][MAX_TAM];
  
    Point inicio;

    for (int i = 0; i < altura; i++) {
      fscanf(file, "%s", mapa[i]);
      for (int j = 0; j < largura; j++) {
        if (mapa[i][j] == '@') {
          inicio.x = i;
          inicio.y = j;
        }
        originalMapa[i][j] = mapa[i][j];
      }
    }  if(tesouro_livre(mapa, altura, largura)){
      printf("-1\n");
      exit(1);
    }

    printf("\nMapa %d:\n", num_mapa);
    imprime_mapa_do_tesouro(mapa, altura, largura);

    Point posAtual = inicio;
    int totalTesouros = 0;
    int totalTempo = 0;

    cercar_comedor_de_humano(mapa, altura, largura);

    while (true) {
      Point posTesouro;
      bool achouTesouro = false;

      for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
          if (mapa[i][j] == '!') {
            posTesouro.x = i;
            posTesouro.y = j;
            achouTesouro = true;
            break;
          }
        }
        if (achouTesouro) {
          break;
        }
      }

      if (!achouTesouro) {
        break;
      }

      bool tesouroAoAlcance = false;
      int dx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
      int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};

      for (int k = 0; k < 8; k++) {
        int nx = posTesouro.x + dx[k];
        int ny = posTesouro.y + dy[k];

        if (limite(nx, ny, altura, largura) && mapa[nx][ny] == '*') {
          tesouroAoAlcance = true;
          break;
        }
      }

      if (tesouroAoAlcance) {
        printf("-1 - Não é possível pegar todos os tesouros.\n");
        return 0;
      }

      int tempoTesouro = busca_profunda(mapa, altura, largura, posAtual, posTesouro);
      totalTempo += tempoTesouro;
      posAtual = posTesouro;

      mapa[posAtual.x][posAtual.y] = '.';

      totalTesouros++;
    }

    for (int i = 0; i < altura; i++) {
      for (int j = 0; j < largura; j++) {
        mapa[i][j] = originalMapa[i][j];
      }
    }

    int tempoRetorno = busca_profunda(mapa, altura, largura, posAtual, inicio);
    totalTempo += tempoRetorno;

    printf("Menor tempo necessário para Jack Sparrow pegar os tesouros e fugir daqueles que querem matar ele pois existe uma recompensa pela saua cabeça: %d horas\n", totalTempo);
    printf("Quantidade de tesouros encontrados: %d\n", totalTesouros);

    printf("\n=========================\n");
    num_mapa++;
  }

  fclose(file);

  return 0;
}