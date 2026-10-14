#include <stdio.h>
#include <stdlib.h>
#include <conio.h> 
#include <string.h>
#include <ctype.h>

int parsing(int *control, char input_string[], char temp_str[]);
void run(int *grid, int size_x, int size_y);

int main() {
    int *grid = NULL;
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

    grid = calloc(size_x * size_y, sizeof(int *));

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

void run(int *grid, int size_x, int size_y){

    int player_x = 0, player_y = 0;
    int running = 1;

    char buffer[4000];

    while(running){

        system("cls");   

        printf("=== O FAZENDEIRO INTERATIVO ===\n");
        printf("Setas: Mover | 't': trigo(M) | 'a': alface(@) |\n"
               " 'm': melancia(O) | 'c': cenoura(*) | '1': colher(.) | 'q': Sair\n\n");

        int buffer_pos = 0;

        for (int i = 0; i < size_x; i++) {
            for (int j = 0; j < size_y; j++) {

                int type = *(grid + (i * size_y) + j); 
                const char* temp_str = "    ";

                if (i == player_x && j == player_y) {
                    temp_str = "[P] "; 
                } else if (type == 0){
                    temp_str = " .  ";
                } else if (type == 1) {
                    temp_str = " M  "; 
                } else if (type == 3) {
                    temp_str = " @  "; 
                } else if (type == 4) {
                    temp_str = " O  "; 
                } else if (type == 5) {
                    temp_str = " *  ";
                }

                memcpy(&buffer[buffer_pos], temp_str, 4);
                buffer_pos += 4;
            }

            memcpy(&buffer[buffer_pos], "\n\n", 2);
            buffer_pos += 2;
        }

        buffer[buffer_pos] = '\0';

        fputs(buffer, stdout);

        int key = _getch();

        if (key == 224 || key == 0) {
            key = _getch();
            
            switch(key) {
                case 72: // Cima
                    if (player_x > 0) player_x--; 
                    break;
                case 80: // Baixo
                    if (player_x < size_x - 1) player_x++; 
                    break;
                case 75: // Esquerda
                    if (player_y > 0) player_y--; 
                    break;
                case 77: // Direita
                    if (player_y < size_y - 1) player_y++; 
                    break;
            }   
        } else {

            switch(key) {
                case '1':
                    *(grid + (player_x * size_y ) + player_y) = 0; // Colher
                    break;
                case 't':
                    *(grid + (player_x * size_y ) + player_y) = 1; // Trigo
                    break;
                case 'a':
                    *(grid + (player_x * size_y ) + player_y) = 3; // Alface
                    break;
                case 'm':
                    *(grid + (player_x * size_y ) + player_y) = 4; // Melancia
                    break;
                case 'c':
                    *(grid + (player_x * size_y ) + player_y) = 5; // Cenoura
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