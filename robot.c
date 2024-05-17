#include <stdio.h>
#include <stdlib.h> // Para o system("clear")
#include <locale.h> // Para o funcionamento de caracteres especiais
#include <unistd.h> // Para a função sleep(), ignorar o possivel erro que a IDE apresentar nessa linha, o programa funciona normalmente com ele

#define SIZE 8

// Cores para destacar o robô e outros componentes no terminal
void white() {
    printf("\033[0;37m");
}

void red() {
    printf("\033[1;31m");
}

void purple() {
    printf("\033[0;35m");
}

void green() {
    printf("\033[0;32m");
}

void yellow() {
    printf("\033[1;33m");
}

// Função para imprimir a matriz vazia
void printEmptyMatrix() {
    // system("clear") // Ativar caso esteja usando linux ou compilador online
    system("cls"); // Faz com que não se acumulem matrizes no terminal
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            white();
            printf("|----| ");
        }
        printf("\n");
    }
    printf("\nFeito por: _a5ur4");
    printf("\n");
}

// Definição das direções prioritárias
const int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
// Ordem: cima, baixo, esquerda, direita

// Função para imprimir a matriz com o robô e o ponto de partida
void printMatrixWithRobot(int matrix[SIZE][SIZE], int x, int y, int startX, int startY) {
    // system("clear") // Ativar caso esteja usando linux ou compilador online
    system("cls"); // Faz com que não se acumulem matrizes no terminal
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (i == startX && j == startY) {
                yellow();
                printf("[ ES ] "); // Marcador do ponto de partida
            } else if (i == x && j == y) {
                red();
                printf("[o_0]b "); // Mostra o robô na posição definida
            } else if (matrix[i][j] == 0) {
                white();
                printf("|----| "); // Representação padrão da matriz
            } else if (matrix[i][j] == 1) {
                green();
                printf("|@@@@| "); // Sujeira
            } else {
                purple();
                printf("|    | "); // Sujeira limpa
            }
        }
        printf("\n");
    }
}

// Função para verificar se o movimento é válido
int isValidMove(int x, int y) {
    return (x >= 0 && x < SIZE && y >= 0 && y < SIZE);
}

// Função para verificar se há sujeira e lugar limpo na direção prioritária
int checkDirection(int matrix[SIZE][SIZE], int x, int y, int dirX, int dirY) {
    int newX = x + dirX;
    int newY = y + dirY;
    
    // Verifica se o movimento é válido
    if (!isValidMove(newX, newY))
        return 0; // Movimento inválido
    
    // Verifica se há sujeira e lugar limpo na direção
    return matrix[newX][newY];
}

// Função para mover o robô na direção prioritária
void moveRobot(int matrix[SIZE][SIZE], int *x, int *y, int dirX, int dirY, int startX, int startY) {
    // Move o robô
    *x += dirX;
    *y += dirY;

    // Marca a posição atual como limpa
    matrix[*x][*y] = -1;

    // Exibe a matriz com o robô após o movimento
    printMatrixWithRobot(matrix, *x, *y, startX, startY);

    // Aguarda 1 segundo antes do próximo movimento
    sleep(1);
}

// Função para mover o robô de volta para sua posição inicial, seguindo o caminho inverso
void moonWalker(int matrix[SIZE][SIZE], int *x, int *y, int startX, int startY) {
    int prevX = -1, prevY = -1; // Armazena a posição anterior do robô

    // Movimento do robô de volta para a posição inicial
    while (*x != startX || *y != startY) {
        // Marca a posição anterior como visitada
        matrix[*x][*y] = -2;

        // Encontra a direção para a posição inicial
        int dirX = (*x < startX) ? 1 : (*x > startX) ? -1 : 0;
        int dirY = (*y < startY) ? 1 : (*y > startY) ? -1 : 0;

        // Armazena a posição atual como anterior
        prevX = *x;
        prevY = *y;

        // Move o robô para a próxima posição em direção à posição inicial
        if (dirX != 0) {
            if (isValidMove(*x + dirX, *y) && matrix[*x + dirX][*y] == -1) {
                *x += dirX;
            } else if (isValidMove(*x, *y + 1) && matrix[*x][*y + 1] == -1) {
                *y += 1;
            } else if (isValidMove(*x, *y - 1) && matrix[*x][*y - 1] == -1) {
                *y -= 1;
            }
        } else if (dirY != 0) {
            if (isValidMove(*x, *y + dirY) && matrix[*x][*y + dirY] == -1) {
                *y += dirY;
            } else if (isValidMove(*x + 1, *y) && matrix[*x + 1][*y] == -1) {
                *x += 1;
            } else if (isValidMove(*x - 1, *y) && matrix[*x - 1][*y] == -1) {
                *x -= 1;
            }
        }

        // Exibe a matriz com o robô após o movimento
        printMatrixWithRobot(matrix, *x, *y, startX, startY);

        // Aguarda 1 segundo antes do próximo movimento
        sleep(1);
    }
}

// Função para limpar todo o ambiente e retornar à estação de partida
void cleanEnvironment(int matrix[SIZE][SIZE], int startX, int startY) {
    int x = startX;
    int y = startY;

    // Exibe a matriz inicial com o robô na posição de partida
    printMatrixWithRobot(matrix, x, y, startX, startY);

    // Loop principal para limpar todas as sujeiras do ambiente
    while (1) {
        int targetX = -1, targetY = -1;

        // Encontra a sujeira mais próxima
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (matrix[i][j] == 1) {
                    targetX = i;
                    targetY = j;
                    break;
                }
            }
            if (targetX != -1)
                break;
        }

        // Verifica se há sujeira para limpar
        if (targetX == -1) {
            printf("Todas as sujeiras foram limpas.\n");
            break;
        }

        // Movimento do robô para limpar a sujeira mais próxima
        while (x != targetX || y != targetY) {
            int nextX = x, nextY = y;
            int minDistance = SIZE * SIZE; // Valor inicial grande

            // Itera sobre as direções para encontrar a direção com a menor distância para a sujeira
            for (int i = 0; i < 4; i++) {
                int newX = x + directions[i][0];
                int newY = y + directions[i][1];

                // Verifica se o movimento é válido e se está mais próximo da sujeira
                if (isValidMove(newX, newY) && matrix[newX][newY] == 1) {
                    int distance = abs(newX - targetX) + abs(newY - targetY);
                    if (distance < minDistance) {
                        minDistance = distance;
                        nextX = newX;
                        nextY = newY;
                    }
                }
            }

            // Move o robô para a próxima posição mais próxima da sujeira
            moveRobot(matrix, &x, &y, nextX - x, nextY - y, startX, startY);
        }

        // Marca a sujeira como limpa
        matrix[targetX][targetY] = -1; // Marca como sujeira limpa

        printf("Sujeira limpa!\n");
    }

    // Movendo o robô de volta à estação de partida seguindo o caminho inverso
    moonWalker(matrix, &x, &y, startX, startY);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    int matrix[SIZE][SIZE] = {0}; // Inicializa a matriz com zeros (lugares limpos)
    int dirtyX, dirtyY;

    // Definição do local de partida do robô
    int startX, startY;
    
    // Definição e validação da posição inicial do robô, isso torna tudo um pouco mais lento, mas é a solução que consegui por agora
    while (startX && startY > SIZE)
    {
        system("cls");

        printEmptyMatrix(); // Imprime a matriz vazia

        printf("\nDigite a linha de partida do robo: ");
        scanf("%d", &startX);

        printf("Digite a coluna de partida do robo: ");
        scanf("%d", &startY);
    }

    // Definição da estação com o mesmo valor que uma área limpa (-1), se tirar isso daqui quebra tudo
    matrix[startX][startY] = -1;

    // Exibe a matriz com o robô na posição de partida
    printMatrixWithRobot(matrix, startX, startY, startX, startY);

    // Loop para adicionar sujeira
    while (1) {
        // Solicita as coordenadas da sujeira ao usuário
        printf("\nDigite a linha da sujeira ou -1 para iniciar a limpeza: ");
        scanf("%d", &dirtyX);

        // Verifica se o usuário deseja encerrar
        if (dirtyX == -1)
            break;

        printf("Digite a coluna da sujeira: ");
        scanf("%d", &dirtyY);

        // Verifica se as coordenadas são válidas
        if (dirtyX < 0 || dirtyX >= SIZE || dirtyY < 0 || dirtyY >= SIZE) {
            printf("Coordenadas inválidas.\n");
            continue;
        }

        // Coloca sujeira na posição especificada
        matrix[dirtyX][dirtyY] = 1;

        // Exibe a matriz atualizada com a sujeira adicionada
        printMatrixWithRobot(matrix, startX, startY, startX, startY);
    }

    // Inicia a limpeza do ambiente a partir da estação de partida
    cleanEnvironment(matrix, startX, startY);

    return 0;
}

// Feito por: _a5ur4