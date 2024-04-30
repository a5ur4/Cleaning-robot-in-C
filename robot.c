#include <stdio.h>
#include <stdlib.h> // Para o system("clear")
#include <locale.h> // Para o funcionar caracteres especiais

#define SIZE 8

// Defini��o das dire��es priorit�rias
const int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
// Ordem: cima, baixo, esquerda, direita

// Fun��o para imprimir a matriz com o rob�
void printMatrixWithRobot(int matrix[SIZE][SIZE], int x, int y) {
    // system("clear") // Ativar caso esteja usando linux ou compilador online
    system("cls"); // Faz com que n�o se acumulem matrizes no terminal
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (i == x && j == y) {
                // printf("?[ ? ]?"); // Representa��o do rob�
                printf("[o_0]b "); // Caso a representa��o do rob� padr�o de erro, utilize essa
            } else if (matrix[i][j] == 0) {
                printf("|....| "); // Representa��o padr�o da matriz
            } else if (matrix[i][j] == 1) {
                printf("|@@@@| "); // Sujeira
            } else {
                printf("|    | "); // Sujeira limpa
            }
        }
        printf("\n");
    }
}

// Fun��o para verificar se o movimento � v�lido
int isValidMove(int x, int y) {
    return (x >= 0 && x < SIZE && y >= 0 && y < SIZE);
}

// Fun��o para verificar se h� sujeira e lugar limpo na dire��o priorit�ria
int checkDirection(int matrix[SIZE][SIZE], int x, int y, int dirX, int dirY) {
    int newX = x + dirX;
    int newY = y + dirY;
    
    // Verifica se o movimento � v�lido
    if (!isValidMove(newX, newY))
        return 0; // Movimento inv�lido
    
    // Verifica se h� sujeira e lugar limpo na dire��o
    return matrix[newX][newY];
}

// Fun��o para mover o rob� na dire��o priorit�ria
void moveRobot(int matrix[SIZE][SIZE], int *x, int *y, int dirX, int dirY) {
    *x += dirX;
    *y += dirY;
    
    // Marca o local como limpo
    matrix[*x][*y] = 0;
}

// Fun��o para limpar todo o ambiente e retornar � esta��o de partida
void cleanEnvironment(int matrix[SIZE][SIZE], int startX, int startY) {
    int x = startX;
    int y = startY;
    
    // Loop principal para limpar todas as sujeiras do ambiente
    while (1) {
        int targetX = -1, targetY = -1;
        
        // Encontra a sujeira mais pr�xima
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
        
        // Verifica se h� sujeira para limpar
        if (targetX == -1) {
            printf("Todas as sujeiras foram limpas.\n");
            break;
        }
        
        // Movimento do rob� para limpar a sujeira mais pr�xima
        while (x != targetX || y != targetY) {
            int nextX = x, nextY = y;
            int minDistance = SIZE * SIZE; // Valor inicial grande

            // Itera sobre as dire��es para encontrar a dire��o com a menor dist�ncia para a sujeira
            for (int i = 0; i < 4; i++) {
                int newX = x + directions[i][0];
                int newY = y + directions[i][1];

                // Verifica se o movimento � v�lido e se est� mais pr�ximo da sujeira
                if (isValidMove(newX, newY) && matrix[newX][newY] == 1) {
                    int distance = abs(newX - targetX) + abs(newY - targetY);
                    if (distance < minDistance) {
                        minDistance = distance;
                        nextX = newX;
                        nextY = newY;
                    }
                }
            }

            // Move o rob� para a pr�xima posi��o mais pr�xima da sujeira
            moveRobot(matrix, &x, &y, nextX - x, nextY - y);
            printf("Movendo para (%d, %d)\n", x, y);
            printMatrixWithRobot(matrix, x, y); // Exibe a matriz com o rob�
        }

        // Marca a sujeira como limpa
        matrix[targetX][targetY] = -1; // Marca como sujeira limpa

        printf("Sujeira limpa!\n");
        printMatrixWithRobot(matrix, x, y); // Exibe a matriz com o rob�
    }
    
    // Movendo o rob� de volta � esta��o de partida
    while (x != startX || y != startY) {
        // Encontra a dire��o oposta �quela que o rob� est� indo
        int dirX = startX - x;
        int dirY = startY - y;

        // Move o rob� para a dire��o oposta
        moveRobot(matrix, &x, &y, dirX, dirY);
        printf("Retornando para a esta��o de partida (%d, %d)\n", x, y);
        printMatrixWithRobot(matrix, x, y); // Exibe a matriz com o rob�
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    int matrix[SIZE][SIZE] = {0}; // Inicializa a matriz com zeros (lugares limpos)
    int dirtyX, dirtyY;
    
    // Exibe a matriz inicial
    printMatrixWithRobot(matrix, 0, 0); // O rob� come�a na posi��o (0, 0)
    
    // Loop para adicionar sujeira
    while (1) {
        // Solicita as coordenadas da sujeira ao usu�rio
        printf("\nDigite as coordenadas da sujeira (linha) ou -1 para posicionar o rob�: ");
        scanf("%d", &dirtyX);
        
        // Verifica se o usu�rio deseja encerrar
        if (dirtyX == -1)
            break;
        
        printf("Digite a coluna da sujeira: ");
        scanf("%d", &dirtyY);
        
        // Verifica se as coordenadas s�o v�lidas
        if (dirtyX < 0 || dirtyX >= SIZE || dirtyY < 0 || dirtyY >= SIZE) {
            printf("Coordenadas inv�lidas.\n");
            continue;
        }
        
        // Coloca sujeira na posi��o especificada
        matrix[dirtyX][dirtyY] = 1;
        
        // Exibe a matriz atualizada com a sujeira adicionada
        printMatrixWithRobot(matrix, 0, 0); // O rob� come�a na posi��o (0, 0)
    }
    
    // Defini��o do local de partida do rob�
    int startX, startY;
    printf("Digite a linha de partida do rob�: ");
    scanf("%d", &startX);
    
    printf("Digite a coluna de partida do rob�: ");
    scanf("%d", &startY);
    
    // Valida��o das coordenadas de partida
    if (!isValidMove(startX, startY)) {
        printf("Coordenadas inv�lidas.\n");
        return 1;
    }
    
    // Inicia a limpeza do ambiente a partir da esta��o de partida
    cleanEnvironment(matrix, startX, startY);
    
    return 0;
}


// Feito por: _a5ur4