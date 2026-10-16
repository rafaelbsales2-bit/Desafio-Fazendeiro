#include <stdio.h>
#include <stdlib.h>
#include <conio.h> 
#include <string.h>
#include <ctype.h>
#include <windows.h>

int parsing(int *control, char input_string[], char temp_str[]);
void run(int *grid, int size_x, int size_y);
void moveCursorTopLeft();
void bit_activator(int player_x, int player_y, int size_x, int size_y, unsigned long long bit_mask[], 
    unsigned long long *root_mask, int vertical_symetry, int horizontal_symetry);

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

void moveCursorTopLeft() {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void run(int *grid, int size_x, int size_y){

    int player_x = 0, player_y = 0;
    int running = 1;
    int first_time = 1;
    int key = 1;

    unsigned long long bit_mask[(size_x * size_y + 63) / 64];
    memset(bit_mask, 0, sizeof(bit_mask));

    unsigned long long root_mask = 0;

    char buffer[4000];

    system("cls");

    while(running){

        moveCursorTopLeft();

        printf("=== O FAZENDEIRO INTERATIVO ===\n");
        printf("Setas: Mover | 't': trigo(M) | 'a': alface(@) | 'm': melancia(O) | 'c': cenoura(*)"
               " | '1': colher(.) | '2': modo simetria vertical | '3': modo simetria horizontal | 'q': Sair\n\n");

        int buffer_pos = 0;

        int horizontal_symetry = 0;
        int vertical_symetry = 0;
        
        if(first_time == 1){

            for (int i = 0; i < size_x; i++) {
                for (int j = 0; j < size_y; j++) {

                    int type = grid[(i * size_y) + j]; 
                    const char* temp_str = "    ";

                    if (i == player_x && j == player_y) {
                        temp_str = "[P] "; 
                    } else if (type == 0){
                        temp_str = " .  ";
                    } else if (type == 1) {
                        temp_str = " M  "; 
                    } else if (type == 2) {
                        temp_str = " @  "; 
                    } else if (type == 3) {
                        temp_str = " O  "; 
                    } else if (type == 4) {
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
            first_time = 0;

        } else{

            while(root_mask != 0){
                unsigned int leaf_idx = 63 - __builtin_ctzll(root_mask);
                unsigned int bit_idx = 63 -  __builtin_ctzll(bit_mask[leaf_idx]);

                int linear_pos = (leaf_idx * 64) + bit_idx; 
    
                // Convertendo posição linear de volta para coordenadas 2D da matriz
                int linha = linear_pos / size_y;
                int coluna = linear_pos % size_y;

                // Atualiza o grid na memória
                // (Aviso: certifique-se de converter a tecla 't'/'a' para os inteiros 1, 2, 3...)
                grid[linear_pos] = key; 

                // Atualiza a tela (somente se o jogador não estiver em cima da planta)
                if (!(linha == player_x && coluna == player_y)) {
                    atualizar_celula(linha, coluna, grid[linear_pos], 0);
                }

                root_mask &= ~(1ULL >> leaf_idx);
                bit_mask[leaf_idx] &= ~(1ULL >> bit_idx);
            }
        }

        int key = _getch();

        if (key == 224 || key == 0) {
            key = _getch();

            int old_x = player_x;
            int old_y = player_y;

            switch(key) {
                case 72: if (player_x > 0) player_x--; break; // Cima
                case 80: if (player_x < size_x - 1) player_x++; break; // Baixo
                case 75: if (player_y > 0) player_y--; break; // Esquerda
                case 77: if (player_y < size_y - 1) player_y++; break; // Direita
            }

            if (old_x != player_x || old_y != player_y) {
            // 1. Redesenha o chão na posição antiga
            int tipo_chao_antigo = grid[(old_x * size_y) + old_y];
            atualizar_celula(old_x, old_y, tipo_chao_antigo, 0);
            
            // 2. Desenha o jogador na nova posição
            atualizar_celula(player_x, player_y, 0, 1);
            }
        } else {

            switch(key) {
                case '1': // Colher = 0
                    bit_activator(player_x, player_y, size_x, size_y, bit_mask,
                                &root_mask, vertical_symetry, horizontal_symetry);
                    break;
                case '2': // Ativa/desativa simetria vertical
                    vertical_symetry = ~(vertical_symetry);
                    break;
                case '3': // Ativa/desativa simetria horizontal
                    horizontal_symetry = ~(horizontal_symetry);
                    break;
                case 't': // Trigo
                    bit_activator(player_x, player_y, size_x, size_y, bit_mask,
                                &root_mask, vertical_symetry, horizontal_symetry);
                    break;
                case 'a': // Alface
                    bit_activator(player_x, player_y, size_x, size_y, bit_mask,
                                &root_mask, vertical_symetry, horizontal_symetry);
                    break;
                case 'm': // Melancia
                    bit_activator(player_x, player_y, size_x, size_y, bit_mask,
                                &root_mask, vertical_symetry, horizontal_symetry);
                    break;
                case 'c': // Cenoura
                    bit_activator(player_x, player_y, size_x, size_y, bit_mask,
                                &root_mask, vertical_symetry, horizontal_symetry);
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

void bit_activator(int player_x, int player_y, int size_x, int size_y, unsigned long long bit_mask[],
    unsigned long long *root_mask, int vertical_symetry, int horizontal_symetry)
{

    int pos = (player_x * size_y ) + player_y;

    bit_mask[pos / 64] = 1ULL << pos % 64;

    *root_mask |= 1ULL << pos /64;
    
    if(vertical_symetry == 1){

        bit_mask[(size_y - player_y + pos) / 64 ] |= 1ULL << (pos / 64 + (size_y - player_y));

        *root_mask |= 1ULL << (size_y - player_y + pos) / 64;
    }
    if(horizontal_symetry == 1){
        
        int adjusted_pos = ((size_x - player_x) * size_y + player_y);

        bit_mask[adjusted_pos /64] |= 1ULL << (adjusted_pos % 64); 

        *root_mask |= 1ULL << adjusted_pos /64;

        if(vertical_symetry == 1){
            bit_mask[(size_y - player_y + adjusted_pos) / 64 ] |= 1ULL << (adjusted_pos / 64 + (size_y - player_y));

            *root_mask |= 1ULL << (size_y - player_y + adjusted_pos) / 64;
        }   
    }
}