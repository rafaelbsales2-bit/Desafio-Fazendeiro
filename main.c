#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
#endif

int parsing(int *control, char input_string[], char temp_str[]);
void run(int *grid, int size_x, int size_y);
void configurar_terminal_e_ocultar_cursor(int size_x, int size_y);
void restaurar_terminal();
void limpar_tela();
int ler_tecla();
void mover_cursor(int linha, int coluna, int base_y);
void atualizar_celula(int linha, int coluna, int tipo, int is_player, int base_y);
void bit_activator(int player_x, int player_y, int size_x, int size_y, unsigned long long bit_mask[], unsigned long long *root_mask, int vertical_symetry, int horizontal_symetry);

#ifdef _WIN32
void configurar_terminal_e_ocultar_cursor(int size_x, int size_y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hConsole, &mode);
    mode &= ~ENABLE_WRAP_AT_EOL_OUTPUT;
    SetConsoleMode(hConsole, mode);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    
    short largura_necessaria = (short)(size_y * 4);
    if (largura_necessaria < 120) largura_necessaria = 120; 
    
    short altura_necessaria = (short)(size_x * 2 + 10);
    
    COORD novo_buffer = csbi.dwSize;
    if (novo_buffer.X < largura_necessaria) novo_buffer.X = largura_necessaria;
    if (novo_buffer.Y < altura_necessaria) novo_buffer.Y = altura_necessaria;

    SetConsoleScreenBufferSize(hConsole, novo_buffer);

    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE; 
    SetConsoleCursorInfo(hConsole, &info);
}

void restaurar_terminal() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &info);
}

void limpar_tela() {
    system("cls");
}

int ler_tecla() {
    return _getch();
}

void mover_cursor(int linha, int coluna, int base_y) {
    COORD coord;
    coord.X = coluna * 4;
    coord.Y = base_y + (linha * 2); 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
#else
void configurar_terminal_e_ocultar_cursor(int size_x, int size_y) {
    (void)size_x;
    (void)size_y;
    printf("\033[?25l\033[?7l");
    fflush(stdout);
}

void restaurar_terminal() {
    printf("\033[?25h\033[?7h");
    fflush(stdout);
}

void limpar_tela() {
    printf("\033[2J\033[H");
    fflush(stdout);
}

int ler_tecla() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void mover_cursor(int linha, int coluna, int base_y) {
    int row = base_y + (linha * 2);
    int col = (coluna * 4) + 1;
    printf("\033[%d;%dH", row, col);
}
#endif

void atualizar_celula(int linha, int coluna, int tipo, int is_player, int base_y) {
    mover_cursor(linha, coluna, base_y);

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

    #ifndef _WIN32
    fflush(stdout);
    #endif
}

int main() {
    int *grid = NULL;
    int size_x = 0, size_y = 0;

    int control = 0;
    char input_string[50], temp_str[10];

    printf("Qual sera o tamanho do seu terreno? (ex: <5x10> x e y multiplicados devem ser <= 4096): ");
    if (!fgets(input_string, sizeof(input_string), stdin)) return 1;

    size_x = parsing(&control, input_string, temp_str);
    size_y = parsing(&control, input_string, temp_str);

    if (size_x * size_y > 4096 || size_x <= 0 || size_y <= 0) {
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

    configurar_terminal_e_ocultar_cursor(size_x, size_y);
    limpar_tela();

    while(running){
        if(first_time == 1){
            #ifdef _WIN32
            COORD coord = {0, 0};
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            #else
            printf("\033[H");
            #endif

            printf("=== O FAZENDEIRO INTERATIVO ===\n");
            printf("Setas: Mover | 't': trigo(M) | 'a': alface(@) | 'm': melancia(O) | 'c': cenoura(*)\n"
                   " '1': colher(.) | '2': simetria vertical | '3': simetria horizontal | 'q': Sair\n\n");

            #ifdef _WIN32
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            base_y = csbi.dwCursorPosition.Y;
            #else
            base_y = 5;
            #endif

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
            
            #ifndef _WIN32
            fflush(stdout);
            #endif
            
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
        
        int ch = ler_tecla();

        #ifdef _WIN32
        if (ch == 224 || ch == 0) {
            ch = ler_tecla();
            switch(ch) {
                case 72: if (player_x > 0) player_x--; break; 
                case 80: if (player_x < size_x - 1) player_x++; break; 
                case 75: if (player_y > 0) player_y--; break; 
                case 77: if (player_y < size_y - 1) player_y++; break; 
            }
        #else
        if (ch == 27) {
            int seq1 = ler_tecla();
            int seq2 = ler_tecla();
            if (seq1 == '[') {
                switch(seq2) {
                    case 'A': if (player_x > 0) player_x--; break;
                    case 'B': if (player_x < size_x - 1) player_x++; break;
                    case 'D': if (player_y > 0) player_y--; break;
                    case 'C': if (player_y < size_y - 1) player_y++; break;
                }
            }
        #endif

            if (old_x != player_x || old_y != player_y) {
                int tipo_chao_antigo = grid[(old_x * size_y) + old_y];
                atualizar_celula(old_x, old_y, tipo_chao_antigo, 0, base_y); 
                atualizar_celula(player_x, player_y, 0, 1, base_y);          
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

    restaurar_terminal();
    limpar_tela();
    printf("Terminal encerrado.\n");
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