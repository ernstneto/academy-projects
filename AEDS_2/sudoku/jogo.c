#include <stdio.h>
#include <stdlib.h>

#define N 9

int verificarLinha(int** grid, int linha, int num) {
    for (int coluna = 0; coluna < N; coluna++) {
        if (grid[linha][coluna] == num) {
            return 0;
        }
    }
    return 1;
}

int verificarColuna(int** grid, int coluna, int num) {
    for (int linha = 0; linha < N; linha++) {
        if (grid[linha][coluna] == num) {
            return 0;
        }
    }
    return 1;
}

int verificarBloco(int** grid, int linhaInicio, int colunaInicio, int num) {
    for (int linha = 0; linha < 3; linha++) {
        for (int coluna = 0; coluna < 3; coluna++) {
            if (grid[linha + linhaInicio][coluna + colunaInicio] == num) {
                return 0;
            }
        }
    }
    return 1;
}

int verificarPosicaoSegura(int** grid, int linha, int coluna, int num) {
    return verificarLinha(grid, linha, num) &&
           verificarColuna(grid, coluna, num) &&
           verificarBloco(grid, linha - linha % 3, coluna - coluna % 3, num);
}

int encontrarProximaCelulaVazia(int** grid, int* linha, int* coluna) {
    for (*linha = 0; *linha < N; (*linha)++) {
        for (*coluna = 0; *coluna < N; (*coluna)++) {
            if (grid[*linha][*coluna] == 0) {
                return 1;
            }
        }
    }
    return 0;
}

int resolverSudoku(int** grid, int* cont) {
    int linha, coluna;
    
    if (!encontrarProximaCelulaVazia(grid, &linha, &coluna)) {
        return 1; // Todas as células estão preenchidas
    }
    
    for (int num = 1; num <= 9; num++) {
        if (verificarPosicaoSegura(grid, linha, coluna, num)) {
            grid[linha][coluna] = num;
            (*cont)++;
            
            if (resolverSudoku(grid, cont)) {
                return 1; // Sudoku resolvido
            }
            
            grid[linha][coluna] = 0; // Desfaz a atribuição se não levar à solução
        }
    }
    
    return 0; // Não há solução para o Sudoku
}

void imprimirSudoku(int** grid) {
    for (int linha = 0; linha < N; linha++) {
        for (int coluna = 0; coluna < N; coluna++) {
            printf("%d ", grid[linha][coluna]);
        }
        printf("\n");
    }
}

int main() {
    int** grid = (int**)calloc(N, sizeof(int*));
    for (int i = 0; i < N; i++) {
        grid[i] = (int*)calloc(N, sizeof(int));
    }

    // Preencha o grid com os valores do Sudoku aqui

    int cont = 0;
    if (resolverSudoku(grid, &cont)) {
        printf("Sudoku resolvido:\n");
        imprimirSudoku(grid);
        printf("Quantidade de interações: %d\n", cont);
    } else {
        printf("Não há solução para o Sudoku.\n");
    }

    // Libera a memória alocada
    for (int i = 0; i < N; i++) {
        free(grid[i]);
    }
    free(grid);

    return 0;
}
