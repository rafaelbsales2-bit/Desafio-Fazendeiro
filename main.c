#include <stdio.h>
#include <stdlib.h>
#include <conio.h> 
#include <string.h>
#include <ctype.h>

int parsing(int *control, char input_string[], char temp_str[]);
void run(int **grid, int size_x, int size_y);

int main() {
    int **grid = NULL;
    int size_x = 0, size_y = 0;

    int control = 0;
    char input_string[50], temp_str[10];

    printf("Qual sera o tamanho do seu terreno? (ex: <5x10> de 1 a 20 para linhas e 1 a 40 para colunas): ");
    fgets(input_string, sizeof(input_string), stdin);

    size_x = parsing(&control, input_string, temp_str);

    if (size_x <= 0 || size_x > 20) {
        printf("ERRO: Valor invalido ou nao inserido\n");
        return 1;
    }

    size_y = parsing(&control, input_string, temp_str);

    if (size_y <= 0 || size_y > 40) {
        printf("ERRO: Valor invalido ou nao inserido\n");
        return 1;
    }

    grid = calloc(size_x, sizeof(int *));

    for(int control = 0; control < size_x; control++){
        grid[control] = calloc(size_y, sizeof(int));
    }

    run(grid, size_x, size_y);

    return 0;
}

int parsing(int *control, char input_string[], char temp_str[]){

    int control2 = 0;

    while (*control < 50 && input_string[*control] != '\0' && input_string[*control] != EOF){

        while (isdigit(input_string[*control]))
        {
            temp_str[control2] = input_string[*control];

            (*control)++;
            control2++;
        }

        if (control2 != 0) break;

        (*control)++;
    }

    temp_str[control2] = '\0';
    
    return atoi(temp_str);
}

void run(int **grid, int size_x, int size_y){

    int player_x = 0, player_y = 0;
    int running = 1;
    
    while(running){

        system("cls");

        printf("=== O FAZENDEIRO INTERATIVO ===\n");
        printf("Setas: Mover | 't': trigo(M) | 'a': alface(@) |\n"
               " 'm': melancia(O) | 'c': cenoura(*) | '1': colher(.) | 'q': Sair\n\n");

        for (int i = 0; i < size_x; i++) {
            for (int j = 0; j < size_y; j++) {

                int type = grid[i][j];

                if (i == player_x && j == player_y) {
                    printf("[P] "); 
                } else if (type == 0){
                    printf(" .  ");
                } else if (type == 1) {
                    printf(" M  "); 
                } else if (type == 2) {
                    printf(" @  "); 
                } else if (type == 3) {
                    printf(" O  "); 
                } else if (type == 4) {
                    printf(" *  ");
                }
            }
            printf("\n\n"); 
        }

        int key = _getch();

        if (key == 224 || key == 0) {
            key = _getch();
            
            switch(key) {
                case 72: 
                    if (player_x > 0) player_x--; 
                    break;
                case 80: 
                    if (player_x < size_x - 1) player_x++; 
                    break;
                case 75: 
                    if (player_y > 0) player_y--; 
                    break;
                case 77: 
                    if (player_y < size_y - 1) player_y++; 
                    break;
            }   
        } else {

            switch(key) {
                case '1':
                    grid[player_x][player_y] = 0;
                    break;
                case 't':
                    grid[player_x][player_y] = 1;
                    break;
                case 'a':
                    grid[player_x][player_y] = 3;
                    break;
                case 'm':
                    grid[player_x][player_y] = 4;
                    break;
                case 'c':
                    grid[player_x][player_y] = 5;
                    break;
                case 'q':
                case 'Q':
                    running = 0; 
                    break;
            }
        }
    }

    system("cls");
    printf("Terminal encerrado.\n");
    return;
}