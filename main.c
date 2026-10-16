#include <stdio.h>
#include <stdlib.h>
#include <conio.h> 
#include <string.h>
#include <ctype.h>
#include <windows.h>

int parsing(int *control, char input_string[], char temp_str[]);
void run(int *grid, int size_x, int size_y);
void moveCursorTopLeft();
void ocultarCursor();
void configurar_terminal(int size_x, int size_y);
void atualizar_celula(int linha, int coluna, int tipo, int is_player, int base_y);
void bit_activator(int player_x, int player_y, int size_x, int size_y, unsigned long long bit_mask[], 
    unsigned long long *root_mask, int vertical_symetry, int horizontal_symetry);

int main() {
    int *grid = NULL;
    int size_x = 0, size_y = 0;

    int control = 0;
    char input_string[50], temp_str[10];

    printf("Qual sera o tamanho do seu terreno? (ex: <5x10> x e y multiplicados devem ser <= 4096): ");
    fgets(input_string, sizeof(input_string), stdin);

    size_x = parsing(&control, input_string, temp_str);

    size_y = parsing(&control, input_string, temp_str);

    if (size_x * size_y > 4096) {
        printf("ERRO: Valor invalido ou nao inserido\n");
        return 1;
    }

    grid = calloc(size_x * size_y, sizeof(int));

    run(grid, size_x, size_y);

    free(grid);
    return 0;
}

int parsing(int *control, char input_string[], char temp_str[]){
    int control2 = 0;

    while (*control < 50 && input_string[*control] != '\0' && input_string[*control] != '\n'){
        while (isdigit(input_string[*control])) {
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

void ocultarCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE; 
    SetConsoleCursorInfo(consoleHandle, &info);
}

void configurar_terminal(int size_x, int size_y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // 1. Desativa a quebra de linha automática (evita que o mapa deforme se tocar na borda)
    DWORD mode;
    GetConsoleMode(hConsole, &mode);
    mode &= ~ENABLE_WRAP_AT_EOL_OUTPUT;
    SetConsoleMode(hConsole, mode);

    // 2. Aumenta o buffer do console para criar barra de rolagem se o mapa for maior que a janela
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    
    // Calcula largura (4 caracteres por celula) e garante um mínimo para o menu não cortar
    short largura_necessaria = (short)(size_y * 4);
    if (largura_necessaria < 120) largura_necessaria = 120; 
    
    // Calcula altura necessária (2 linhas por celula + 10 do menu)
    short altura_necessaria = (short)(size_x * 2 + 10);
    
    COORD novo_buffer = csbi.dwSize;
    if (novo_buffer.X < largura_necessaria) novo_buffer.X = largura_necessaria;
    if (novo_buffer.Y < altura_necessaria) novo_buffer.Y = altura_necessaria;

    SetConsoleScreenBufferSize(hConsole, novo_buffer);
}

void atualizar_celula(int linha, int coluna, int tipo, int is_player, int base_y) {
    COORD coord;
    coord.X = coluna * 4;
    coord.Y = base_y + (linha * 2); 
    
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    if (is_player) {
        printf("[P] ");
    } else if (tipo == 0) {
        printf(" .  ");
    } else if (tipo == 1) { 
        printf(" M  ");
    } else if (tipo == 2) { 
        printf(" @  ");
    } else if (tipo == 3) { 
        printf(" O  ");
    } else if (tipo == 4) { 
        printf(" *  ");
    }
}

void run(int *grid, int size_x, int size_y){

    int player_x = 0, player_y = 0;
    int running = 1;
    int first_time = 1;
    
    int current_plant_type = 0;
    int base_y = 0;

    int horizontal_symetry = 0;
    int vertical_symetry = 0;

    unsigned long long bit_mask[(size_x * size_y + 63) / 64];
    memset(bit_mask, 0, sizeof(bit_mask));

    unsigned long long root_mask = 0;
    char buffer[4000 * 6];

    configurar_terminal(size_x, size_y);
    ocultarCursor();
    system("cls");

    while(running){
        if(first_time == 1){
            COORD coord = {0, 0};
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

            printf("=== O FAZENDEIRO INTERATIVO ===\n");
            printf("Setas: Mover | 't': trigo(M) | 'a': alface(@) | 'm': melancia(O) | 'c': cenoura(*)\n"
                   " '1': colher(.) | '2': simetria vertical | '3': simetria horizontal | 'q': Sair\n\n");

            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            base_y = csbi.dwCursorPosition.Y;

            int buffer_pos = 0;
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

        } else {
            while(root_mask != 0){
                int leaf_idx = __builtin_ctzll(root_mask);

                while(bit_mask[leaf_idx] != 0) {
                    int bit_idx = __builtin_ctzll(bit_mask[leaf_idx]);
                    
                    int linear_pos = (leaf_idx * 64) + bit_idx;
                    int linha = linear_pos / size_y;
                    int coluna = linear_pos % size_y;

                    grid[linear_pos] = current_plant_type;

                    if (!(linha == player_x && coluna == player_y)) {
                        atualizar_celula(linha, coluna, current_plant_type, 0, base_y);
                    }

                    bit_mask[leaf_idx] &= ~(1ULL << bit_idx);
                }
                root_mask &= ~(1ULL << leaf_idx);
            }
        }

        int old_x = player_x;
        int old_y = player_y;
        int ch = _getch();

        if (ch == 224 || ch == 0) {
            ch = _getch();
            switch(ch) {
                case 72: if (player_x > 0) player_x--; break; // Cima
                case 80: if (player_x < size_x - 1) player_x++; break; // Baixo
                case 75: if (player_y > 0) player_y--; break; // Esquerda
                case 77: if (player_y < size_y - 1) player_y++; break; // Direita
            }   

            if (old_x != player_x || old_y != player_y) {
                int tipo_chao_antigo = grid[(old_x * size_y) + old_y];
                atualizar_celula(old_x, old_y, tipo_chao_antigo, 0, base_y); // Apaga rastro
                atualizar_celula(player_x, player_y, 0, 1, base_y);          // Desenha jogador
            }
        } else {
            switch(ch) {
                case '1': 
                    current_plant_type = 0; 
                    bit_activator(player_x, player_y, size_x, size_y, bit_mask, &root_mask, vertical_symetry, horizontal_symetry);
                    break;
                case '2': vertical_symetry = !vertical_symetry; break;
                case '3': horizontal_symetry = !horizontal_symetry; break;
                case 't': 
                    current_plant_type = 1;
                    bit_activator(player_x, player_y, size_x, size_y, bit_mask, &root_mask, vertical_symetry, horizontal_symetry);
                    break;
                case 'a': 
                    current_plant_type = 2;
                    bit_activator(player_x, player_y, size_x, size_y, bit_mask, &root_mask, vertical_symetry, horizontal_symetry);
                    break;
                case 'm': 
                    current_plant_type = 3;
                    bit_activator(player_x, player_y, size_x, size_y, bit_mask, &root_mask, vertical_symetry, horizontal_symetry);
                    break;
                case 'c': 
                    current_plant_type = 4;
                    bit_activator(player_x, player_y, size_x, size_y, bit_mask, &root_mask, vertical_symetry, horizontal_symetry);
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

    bit_mask[pos / 64] |= 1ULL << (pos % 64);
    *root_mask |= 1ULL << (pos / 64);
    
    if(vertical_symetry == 1){
        int sym_pos = (pos / size_y) * size_y + (size_y - 1 - player_y);
        bit_mask[sym_pos / 64] |= 1ULL << (sym_pos % 64);
        *root_mask |= 1ULL << (sym_pos / 64);
    }
    
    if(horizontal_symetry == 1){
        int adjusted_pos = ((size_x - 1 - player_x) * size_y + player_y);
        bit_mask[adjusted_pos / 64] |= 1ULL << (adjusted_pos % 64); 
        *root_mask |= 1ULL << (adjusted_pos / 64);

        if(vertical_symetry == 1){
            int sym_adj_pos = ((size_x - 1 - player_x) * size_y + (size_y - 1 - player_y));
            bit_mask[sym_adj_pos / 64] |= 1ULL << (sym_adj_pos % 64);
            *root_mask |= 1ULL << (sym_adj_pos / 64);
        }   
    }
}