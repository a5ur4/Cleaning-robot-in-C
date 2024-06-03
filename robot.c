#include <stdio.h>
#include <stdlib.h> // Para system("cls")
#include <locale.h> // Para configurações de localidade
#include <unistd.h> // Para sleep(), ignorar o possível erro que a IDE apresentar nessa linha, o programa funciona normalmente com ele
#include <math.h>

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

// Função para imprimir a matriz, agora tudo em uma só função
void printMatrix(int matrix[SIZE][SIZE], int x, int y, int startX, int startY) {
    #ifdef _WIN32
        system("cls"); // Limpa o terminal no Windows
    #else
        system("clear"); // Limpa o terminal em sistemas Unix
    #endif

    printf("   ");
    for (int i = 0; i < SIZE; i++) {
        printf("  %-5d", i); // Imprime os números da linha superior
    }
    printf("\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%-3d", i); // Imprime o número da coluna à esquerda

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


// Definição das direções prioritárias
const int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Direções: cima, baixo, esquerda, direita

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
    printMatrix(matrix, *x, *y, startX, startY); // Exibe a matriz com o robô após o movimento
    sleep(1); // Aguarda 1 segundo antes do próximo movimento
}

// Estrutura para representar uma posição na matriz
typedef struct {
    int x, y; // Coordenadas do nó na matriz, os pontos onde tomara como referência
    int g, h, f; // Parâmetros utilizados para a heurística
} Node; // Nó, representa um ponto ou posição na matriz, cada nó armazena uma informação diferente para a navegação e cálculo

// Função para calcular a heurística, no caso o custo do nó atual até o nó objetivo, respectivamente x e y
// Cálculo feito utilizando a distância de Manhattan, que é a soma das distâncias absolutas das diferenças das coordenadas
int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Função para encontrar o caminho de volta usando A*
void oppenhaimer(int matrix[SIZE][SIZE], int startX, int startY, int goalX, int goalY, int *pathLength, Node path[]) {
    int closedList[SIZE][SIZE] = {0}; // Os nós já limpos são armazenados aqui
    Node openList[SIZE * SIZE]; // Os nós são armazenados aqui
    int openListSize = 0;

    Node parent[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            parent[i][j] = (Node){-1, -1, 0, 0, 0};
        }
    }

    openList[openListSize++] = (Node){startX, startY, 0, heuristic(startX, startY, goalX, goalY), heuristic(startX, startY, goalX, goalY)};
    closedList[startX][startY] = 1;

    while (openListSize > 0) {
        // Encontrar o nó com o menor valor f
        int minIndex = 0;
        for (int i = 1; i < openListSize; i++) {
            if (openList[i].f < openList[minIndex].f) {
                minIndex = i;
            }
        }

        Node current = openList[minIndex];

        // Se alcançar o objetivo
        if (current.x == goalX && current.y == goalY) {
            Node temp = current;
            *pathLength = 0;
            while (parent[temp.x][temp.y].x != -1) {
                path[(*pathLength)++] = temp;
                temp = parent[temp.x][temp.y];
            }
            path[(*pathLength)++] = temp;
            return;
        }

        // Remove o nó atual da lista aberta
        openList[minIndex] = openList[--openListSize];

        // Explorar os vizinhos
        for (int i = 0; i < 4; i++) {
            int newX = current.x + directions[i][0];
            int newY = current.y + directions[i][1];

            if (isValidMove(newX, newY) && (matrix[newX][newY] == -1 || matrix[newX][newY] == -2) && !closedList[newX][newY]) {
                int gNew = current.g + 1;
                int hNew = heuristic(newX, newY, goalX, goalY);
                int fNew = gNew + hNew;

                int inOpenList = 0;
                for (int j = 0; j < openListSize; j++) {
                    if (openList[j].x == newX && openList[j].y == newY) {
                        inOpenList = 1;
                        if (openList[j].f > fNew) {
                            openList[j].g = gNew;
                            openList[j].h = hNew;
                            openList[j].f = fNew;
                            parent[newX][newY] = current;
                        }
                        break;
                    }
                }

                if (!inOpenList) {
                    openList[openListSize++] = (Node){newX, newY, gNew, hNew, fNew};
                    parent[newX][newY] = current;
                }

                closedList[newX][newY] = 1;
            }
        }
    }
}

// Função para mover o robô de volta para a posição inicial
void moonWalker(int matrix[SIZE][SIZE], int *x, int *y, int startX, int startY) {
    Node path[SIZE * SIZE];
    int pathLength;

    sleep(2); // Mostrar que a função moonWalker iniciou

    oppenhaimer(matrix, *x, *y, startX, startY, &pathLength, path);
    for (int i = pathLength - 1; i >= 0; i--) {
        *x = path[i].x;
        *y = path[i].y;
        printMatrix(matrix, *x, *y, startX, startY);
        sleep(1); // Aguarda 1 segundo antes do próximo movimento
    }
}

int hasVisitedRecently(int visited[SIZE][SIZE], int x, int y) {
    return visited[x][y];
}

void markVisited(int visited[SIZE][SIZE], int x, int y) {
    visited[x][y] = 1;
}

int theManWhoSoldTheWorld(int matrix[SIZE][SIZE], int startX, int startY, int endX, int endY) { // Função para calculo para contagem de lixos em um linha coluna e sua procedência
    int dirtCount = 0;
    int x = startX;
    int y = startY;

    // Movimento vertical primeiro
    while (x != endX) {
        if (matrix[x][y] == 1) {
            dirtCount++;
        }
        x += (endX > x) ? 1 : -1;
    }

    // Movimento horizontal depois
    while (y != endY) {
        if (matrix[x][y] == 1) {
            dirtCount++;
        }
        y += (endY > y) ? 1 : -1;
    }

    return dirtCount;
}

// Função para limpar o ambiente e retornar à posição inicial
void cleanEnvironment(int matrix[SIZE][SIZE], int startX, int startY) {
    int x = startX, y = startY;
    int visited[SIZE][SIZE] = {0}; // Array para rastrear células visitadas
    printMatrix(matrix, x, y, startX, startY); // Exibe a matriz inicial com o robô na posição de partida

    // Função para contar o número de sujeiras restantes
    int countDirt(int matrix[SIZE][SIZE]) {
        int dirtCount = 0;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (matrix[i][j] == 1) {
                    dirtCount++;
                }
            }
        }
        return dirtCount;
    }

    // Loop principal para limpar todas as sujeiras do ambiente
    while (1) {
        int targetX = -1, targetY = -1;
        int searchRadius = 1; // Raio da sujeira

        // Encontra a sujeira mais próxima, e caso não ache aumenta o raio para o tamanho maximo da matriz
        while (targetX == -1 && searchRadius <= SIZE) {  // Ajuste aqui
            for (int i = -searchRadius; i <= searchRadius; i++) {
                for (int j = -searchRadius; j <= searchRadius; j++) {
                    int newX = x + i;
                    int newY = y + j;
                    if (isValidMove(newX, newY) && matrix[newX][newY] == 1) { // Verifica a célula vizinha para sujeira
                        targetX = newX;
                        targetY = newY;
                        break;
                    }
                }
                if (targetX != -1) break;
            }
            searchRadius++;
        }

        if (targetX == -1) { // Verifica se há sujeira para limpar
            int dirtCount = countDirt(matrix);
            if (dirtCount == 0) {
                printf("Todas as sujeiras foram limpas, inicio da funcao moonWalker.\n");
                moonWalker(matrix, &x, &y, startX, startY); // Movendo o robô de volta à estação de partida seguindo o caminho inverso
                break;
            } else if (dirtCount >= 1) {
                printf("Restam %d sujeiras, o robo ira verificar a matriz para localizar e limpar.\n", dirtCount);
                sleep(4);
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
            }
        }

        // Move para a sujeira mais próxima
        while (x != targetX || y != targetY) {
            int nextX = x, nextY = y;
            int minDistance = SIZE * SIZE; // Inicializa com um valor grande
            int minDirt = SIZE * SIZE; // Inicializa com um valor grande para quantidade de sujeira

            for (int i = 0; i < 4; i++) { // Itera sobre as direções para encontrar a direção com a menor distância para a sujeira
                int newX = x + directions[i][0];
                int newY = y + directions[i][1];

                if (isValidMove(newX, newY) && matrix[newX][newY] != 0 && !hasVisitedRecently(visited, newX, newY)) {
                    int distance = abs(newX - targetX) + abs(newY - targetY);
                    int dirt = theManWhoSoldTheWorld(matrix, x, y, newX, newY);
                    if (dirt < minDirt || (dirt == minDirt && distance < minDistance)) {
                        minDistance = distance;
                        minDirt = dirt;
                        nextX = newX;
                        nextY = newY;
                    }
                }
            }

            if (nextX == x && nextY == y) {
                printf("O robo nao pode alcancar a sujeira restante.\n");
                return;
            }

            // Marca a célula atual como visitada
            markVisited(visited, x, y);
            // Move o robô para a próxima posição mais próxima da sujeira
            moveRobot(matrix, &x, &y, nextX - x, nextY - y, startX, startY);
        }

        // Marca a sujeira como limpa
        matrix[targetX][targetY] = -1;
        printf("Sujeira limpa!\n");

        // Reseta a matriz de visitados
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                visited[i][j] = 0;
            }
        }

        // Volta a enxergar apenas uma célula de distância
        searchRadius = 1;
    }
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
        printMatrix(matrix, -1, -1, startX, startY); // Imprime a matriz vazia

        printf("\nDigite a linha de partida do robô: ");
        scanf("%d", &startX);

        printf("Digite a coluna de partida do robô: ");
        scanf("%d", &startY);
    }

    matrix[startX][startY] = -1; // Definição da estação com o mesmo valor que uma área limpa (-1), se tirar isso daqui quebra tudo

    // Imprime a matriz com a posição inicial do robô
    printMatrix(matrix, startX, startY, startX, startY);

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
        printMatrix(matrix, startX, startY, startX, startY);
    }

    // Inicia a limpeza do ambiente a partir da posição inicial
    cleanEnvironment(matrix, startX, startY);

    return 0;
}

// Feito por: _a5ur4