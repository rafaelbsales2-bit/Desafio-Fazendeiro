#include <stdio.h>
#include <stdlib.h>
#include <conio.h> 
#include <string.h>
#include <ctype.h>

int parsing(int *control, char input_string[], char temp_str[]);

int main() {
    int **grid = NULL;
    int size_x = 0, size_y = 0;

    int control = 0;
    char input_string[50], temp_str[10];

    printf("Qual sera o tamanho do seu terreno? (ex: <5x10> ou <2 2>): ");
    fgets(input_string, sizeof(input_string), stdin);

    //Parsing do primeiro digito
    size_x = parsing(&control, input_string, temp_str);

    if (size_x == 0) {
        printf("ERRO: Valor invalido ou nao inserido\n");
        return 1;
    }

    //parsing segundo digito
    size_y = parsing(&control, input_string, temp_str);

    if (size_y == 0) {
        printf("ERRO: Valor invalido ou nao inserido\n");
        return 1;
    }
    
    printf("SIZE: %dx%d\n", size_x, size_y);

    //inicialização
    grid = malloc(size_x * sizeof(int *));

    for(int control = 0; control < size_x; control++){
        grid[control] = malloc(size_y * sizeof(int));
    }
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