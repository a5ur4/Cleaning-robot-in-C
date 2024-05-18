#include <stdio.h>
#include <stdlib.h> // Para system("cls")
#include <locale.h> // Para configurações de localidade
#include <unistd.h> // Para sleep(), ignorar o possivel erro que a IDE apresentar nessa linha, o programa funciona normalmente com ele

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
    #ifdef _WIN32
        system("cls"); // Faz com que não se acumulem matrizes no terminal
    #else
        system("clear");
    #endif
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            white();
            printf("|----| ");
        }
        printf("\n");
    }
    printf("\nFeito por: _a5ur4\n");
}

// Definição das direções prioritárias
const int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Direções: cima, baixo, esquerda, direita

// Função para imprimir a matriz com o robô e sujeira
void printMatrixWithRobot(int matrix[SIZE][SIZE], int x, int y, int startX, int startY) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
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
    printf("\nFeito por: _a5ur4\n");
}

// Função para verificar se o movimento é válido
int isValidMove(int x, int y) {
    return (x >= 0 && x < SIZE && y >= 0 && y < SIZE);
}

// Função para mover o robô em uma direção específica
void moveRobot(int matrix[SIZE][SIZE], int *x, int *y, int dirX, int dirY, int startX, int startY) {
    // Move o robô
    *x += dirX;
    *y += dirY;
    matrix[*x][*y] = -1; // Marca a posição atual como limpa
    printMatrixWithRobot(matrix, *x, *y, startX, startY); // Exibe a matriz com o robô após o movimento
    sleep(1); // Aguarda 1 segundo antes do próximo movimento
}

// Função para mover o robô de volta para a posição inicial
void moonWalker(int matrix[SIZE][SIZE], int *x, int *y, int startX, int startY) {
    while (*x != startX || *y != startY) {
        matrix[*x][*y] = -2; // Marca a posição atual como visitada
        int dirX = (*x < startX) ? 1 : (*x > startX) ? -1 : 0;
        int dirY = (*y < startY) ? 1 : (*y > startY) ? -1 : 0;
        
        if (dirX != 0 && isValidMove(*x + dirX, *y) && matrix[*x + dirX][*y] == -1) {
            *x += dirX;
        } else if (dirY != 0 && isValidMove(*x, *y + dirY) && matrix[*x][*y + dirY] == -1) {
            *y += dirY;
        }
        printMatrixWithRobot(matrix, *x, *y, startX, startY); // Exibe a matriz com o robô após o movimento
        sleep(1); // Aguarda 1 segundo antes do próximo movimento
    }
}

// Função para limpar o ambiente e retornar à posição inicial
void cleanEnvironment(int matrix[SIZE][SIZE], int startX, int startY) {
    int x = startX, y = startY;
    printMatrixWithRobot(matrix, x, y, startX, startY); // Exibe a matriz inicial com o robô na posição de partida

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
            if (targetX != -1) break;
        }

        if (targetX == -1) { // Verifica se há sujeira para limpar
            printf("Todas as sujeiras foram limpas.\n");
            break;
        }

        // Move para a sujeira mais próxima
        while (x != targetX || y != targetY) {
            int nextX = x, nextY = y;
            int minDistance = SIZE * SIZE; // Inicializa com um valor grande

            for (int i = 0; i < 4; i++) { // Itera sobre as direções para encontrar a direção com a menor distância para a sujeira
                int newX = x + directions[i][0];
                int newY = y + directions[i][1];

                if (isValidMove(newX, newY) && matrix[newX][newY] != 0) { // Verifica se o movimento é válido e se não passa por locais com "|----|", que no caso é o 0
                    int distance = abs(newX - targetX) + abs(newY - targetY);
                    if (distance < minDistance) {
                        minDistance = distance;
                        nextX = newX;
                        nextY = newY;
                    }
                }
            }

            if (nextX == x && nextY == y) {
                printf("O robô não pode alcançar a sujeira restante.\n");
                return;
            }

            moveRobot(matrix, &x, &y, nextX - x, nextY - y, startX, startY); // Move o robô para a próxima posição mais próxima da sujeira
        }

        matrix[targetX][targetY] = -1; // Marca a sujeira como limpa
        printf("Sujeira limpa!\n");
    }

    moonWalker(matrix, &x, &y, startX, startY); // Movendo o robô de volta à estação de partida seguindo o caminho inverso
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    int matrix[SIZE][SIZE] = {0}; // Inicializa a matriz com zeros (espaços limpos)
    int dirtyX, dirtyY;

    int startX = -1, startY = -1; 

    // Definição e validação da posição inicial do robô, isso torna tudo mais lento, porém é o que funciona
    while (startX < 0 || startX >= SIZE || startY < 0 || startY >= SIZE) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
        printEmptyMatrix(); // Imprime a matriz vazia

        printf("\nDigite a linha de partida do robô: ");
        scanf("%d", &startX);

        printf("Digite a coluna de partida do robô: ");
        scanf("%d", &startY);
    }

    matrix[startX][startY] = -1; // Definição da estação com o mesmo valor que uma área limpa (-1), se tirar isso daqui quebra tudo

    // Imprime a matriz com a posição inicial do robô
    printMatrixWithRobot(matrix, startX, startY, startX, startY);

    // Loop para adicionar sujeira
    while (1) {
        printf("\nDigite a linha da sujeira ou -1 para iniciar a limpeza: ");
        scanf("%d", &dirtyX);

        if (dirtyX == -1) break; // Verifica se o usuário deseja encerrar

        printf("Digite a coluna da sujeira: ");
        scanf("%d", &dirtyY);

        if (dirtyX < 0 || dirtyX >= SIZE || dirtyY < 0 || dirtyY >= SIZE) { // Verifica se as coordenadas são válidas
            printf("Coordenadas inválidas.\n");
            continue;
        }

        matrix[dirtyX][dirtyY] = 1; // Define a posição da sujeira

        // Imprime a matriz com a sujeira atualizada
        printMatrixWithRobot(matrix, startX, startY, startX, startY);
    }

    // Inicia a limpeza do ambiente a partir da posição inicial
    cleanEnvironment(matrix, startX, startY);

    return 0;
}

// Feito por: _a5ur4