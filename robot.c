#include <stdio.h>
#include <stdlib.h> // Para o system("clear")
#include <locale.h> // Para o funcionar caracteres especiais

#define SIZE 8

// Definição das direções prioritárias
const int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
// Ordem: cima, baixo, esquerda, direita

// Função para imprimir a matriz com o robô
void printMatrixWithRobot(int matrix[SIZE][SIZE], int x, int y) {
    // system("clear") // Ativar caso esteja usando linux ou compilador online
    system("cls"); // Faz com que não se acumulem matrizes no terminal
    printf("Matriz:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (i == x && j == y) {
                printf("?[ ? ]?"); // Representação do robô
            } else if (matrix[i][j] == 0) {
                printf("|....| "); // Representação padrão da matriz
            } else if (matrix[i][j] == 1) {
                printf("|@@@@| "); // Sujeira
            } else {
                printf("|----| "); // Sujeira limpa
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
void moveRobot(int matrix[SIZE][SIZE], int *x, int *y, int dirX, int dirY) {
    *x += dirX;
    *y += dirY;
    
    // Marca o local como limpo
    matrix[*x][*y] = 0;
}

// Função para limpar todo o ambiente e retornar à estação de partida
void cleanEnvironment(int matrix[SIZE][SIZE], int startX, int startY) {
    int x = startX;
    int y = startY;
    
    // Encontra a sujeira mais próxima
    int targetX = -1, targetY = -1;
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
    
    if (targetX == -1) {
        printf("Não há sujeira para limpar.\n");
        return;
    }
    
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
        moveRobot(matrix, &x, &y, nextX - x, nextY - y);
        printf("Movendo para (%d, %d)\n", x, y);
        printMatrixWithRobot(matrix, x, y); // Exibe a matriz com o robô
    }
    
    // Marca a sujeira como limpa
    matrix[targetX][targetY] = -1; // Marca como sujeira limpa
    
    printf("Sujeira limpa!\n");
    printMatrixWithRobot(matrix, x, y); // Exibe a matriz com o robô
    
    // Movendo o robô de volta à estação de partida
    while (x != startX || y != startY) {
        // Encontra a direção oposta àquela que o robô está indo
        int dirX = startX - x;
        int dirY = startY - y;
        
        // Move o robô para a direção oposta
        moveRobot(matrix, &x, &y, dirX, dirY);
        printf("Retornando para a estação de partida (%d, %d)\n", x, y);
        printMatrixWithRobot(matrix, x, y); // Exibe a matriz com o robô
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    int matrix[SIZE][SIZE] = {0}; // Inicializa a matriz com zeros (lugares limpos)
    int dirtyX, dirtyY;
    
    // Exibe a matriz inicial
    printMatrixWithRobot(matrix, 0, 0); // O robô começa na posição (0, 0)
    
    // Loop para adicionar sujeira
    while (1) {
        // Solicita as coordenadas da sujeira ao usuário
        printf("\nDigite as coordenadas da sujeira (linha) ou -1 para posicionar o robô: ");
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
        printMatrixWithRobot(matrix, 0, 0); // O robô começa na posição (0, 0)
    }
    
    // Definição do local de partida do robô
    int startX, startY;
    printf("Digite a linha de partida do robô: ");
    scanf("%d", &startX);
    
    printf("Digite a coluna de partida do robô: ");
    scanf("%d", &startY);
    
    // Validação das coordenadas de partida
    if (!isValidMove(startX, startY)) {
        printf("Coordenadas inválidas.\n");
        return 1;
    }
    
    // Inicia a limpeza do ambiente a partir da estação de partida
    cleanEnvironment(matrix, startX, startY);
    
    return 0;
}


// Feito por: _a5ur4