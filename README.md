# Documentação do Projeto: Robô de Limpeza

## Descrição do Projeto

Este projeto implementa um robô de limpeza simulado em C, que se move em uma matriz 8x8, limpa sujeiras especificadas pelo usuário e retorna à sua posição inicial após a limpeza. O robô evita passar por locais inacessíveis representados por "|----|".

## Funcionalidades

- Inicialização do ambiente de limpeza em uma matriz 8x8.
- Definição da posição inicial do robô pelo usuário.
- Adição de sujeiras em posições especificadas pelo usuário.
- Movimento do robô para limpar todas as sujeiras na matriz.
- Retorno do robô à posição inicial após a limpeza utilizando o algoritmo A* (implementado na função `oppenhaimer`).
- Visualização da matriz no terminal, com diferentes cores representando o robô, a sujeira, áreas limpas e a posição inicial.

## Requisitos

- Compilador C
- Biblioteca padrão C
- Biblioteca `locale.h` para configurações de localidade
- Biblioteca `unistd.h` para função `sleep()`

## Uso

### Compilação

Para compilar o programa, use um compilador C. Por exemplo, no terminal do Linux ou no prompt de comando do Windows, navegue até o diretório do projeto e execute:

```sh
gcc -o robot robot.c
```

### Execução

Após a compilação, execute o programa:

```sh
./robot
```

### Funcionamento

1. O programa solicita ao usuário para definir a posição inicial do robô na matriz (linha e coluna).
2. A matriz vazia é exibida, destacando a posição inicial do robô.
3. O usuário pode adicionar sujeiras especificando as coordenadas (linha e coluna).
4. Após adicionar todas as sujeiras, o usuário pode iniciar a limpeza digitando `-1` como coordenada de linha.
5. O robô se move pela matriz limpando as sujeiras, evitando locais inacessíveis representados por "|----|".
6. Após limpar todas as sujeiras, o robô retorna à posição inicial utilizando o algoritmo A* (implementado na função `oppenhaimer`) para encontrar o caminho mais curto.
7. A cada movimento, a matriz é exibida no terminal, mostrando a posição atual do robô e as áreas limpas.

## Estrutura do Código

### Definições e Configurações

- `#include <stdio.h>`
- `#include <stdlib.h>`: Para a função `system()`.
- `#include <locale.h>`: Para configurações de localidade.
- `#include <unistd.h>`: Para a função `sleep()`.
- `#include <math.h>`: Para funções matemáticas.
- `#define SIZE 8`: Define o tamanho da matriz.

### Funções de Cor

Funções para definir cores no terminal:

- `void white()`
- `void red()`
- `void purple()`
- `void green()`
- `void yellow()`

### Função de Impressão

- `void printMatrix(int matrix[SIZE][SIZE], int x, int y, int startX, int startY)`: Imprime a matriz com o robô, sujeira e áreas limpas.

### Funções de Movimento e Verificação

- `int isValidMove(int x, int y)`: Verifica se o movimento é válido dentro da matriz.
- `void moveRobot(int matrix[SIZE][SIZE], int *x, int *y, int dirX, int dirY, int startX, int startY)`: Move o robô em uma direção específica e atualiza a matriz.
- `void moonWalker(int matrix[SIZE][SIZE], int *x, int *y, int startX, int startY)`: Move o robô de volta à posição inicial usando o algoritmo A* (implementado na função `oppenhaimer`).

### Estruturas e Funções do Algoritmo A*

- **Estrutura `Node`**: Representa uma posição na matriz, usada para a navegação e cálculo do caminho.
  ```c
  typedef struct {
      int x, y; // Coordenadas do nó na matriz
      int g, h, f; // Custos usados na heurística
  } Node;
  ```
- `int heuristic(int x1, int y1, int x2, int y2)`: Calcula a heurística (distância de Manhattan) entre dois pontos.
- `void oppenhaimer(int matrix[SIZE][SIZE], int startX, int startY, int goalX, int goalY, int *pathLength, Node path[])`: Implementa o algoritmo A* para encontrar o caminho mais curto.

### Função Principal de Limpeza

- `void cleanEnvironment(int matrix[SIZE][SIZE], int startX, int startY)`: Função principal que executa a limpeza do ambiente, movendo o robô para limpar sujeiras e retornando à posição inicial.

### Função Principal

- `int main()`: Função principal que inicializa a matriz, define a posição inicial do robô, permite ao usuário adicionar sujeiras e inicia o processo de limpeza.

## Exemplo de Uso

Ao iniciar o programa, o usuário verá a matriz vazia e será solicitado a definir a posição inicial do robô. Após definir a posição inicial, o usuário pode adicionar sujeiras especificando suas coordenadas. Quando todas as sujeiras forem adicionadas, o usuário pode iniciar a limpeza digitando `-1` como coordenada de linha. O robô então se moverá pela matriz, limpando as sujeiras e evitando locais inacessíveis, até retornar à posição inicial.

## Autor

Feito por: `_a5ur4`

---

Este README fornece uma visão geral do projeto e como ele funciona. Para mais detalhes, consulte o código-fonte e os comentários no código.