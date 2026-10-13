# 🌾 Desafio Fazendeiro - O Fazendeiro Interativo

Um simulador interativo de fazenda em terminal desenvolvido em linguagem **C**. O jogador controla um fazendeiro dentro de um terreno configurável, podendo se movimentar pela grade, semear diferentes tipos de cultivos e colher a plantação.

---

## 🎮 Funcionalidades

- **Terreno customizável:** Defina as dimensões da grade no início do programa (linhas de 1 a 20 e colunas de 1 a 40).
- **Movimentação fluida:** Navegação em tempo real pelo campo através das setas do teclado.
- **Opções de plantio e colheita:**
  - `t` - Trigo (`M`)
  - `a` - Alface (`@`)
  - `m` - Melancia (`O`)
  - `c` - Cenoura (`*`)
  - `1` - Colher / Limpar terreno (`.`)
- **Visualização dinâmica:** Atualização contínua do mapa com a posição do fazendeiro (`[P]`) e o estado de cada lote de terra.

---

## ⌨️ Controles

| Tecla | Ação | Símbolo no Mapa |
| :---: | :--- | :---: |
| `↑` `↓` `←` `→` | Mover fazendeiro | `[P]` |
| `t` | Plantar Trigo | `M` |
| `a` | Plantar Alface | `@` |
| `m` | Plantar Melancia | `O` |
| `c` | Plantar Cenoura | `*` |
| `1` | Colher lote atual | `.` |
| `q` / `Q` | Sair do jogo | - |

---

## 🖥️ Dica de Visualização e Execução

> [!TIP]
> **Recomendação de Terminal:**  
> Caso vá utilizar dimensões de terreno maiores (próximas de 20 linhas ou 40 colunas), **recomenda-se expandir a janela do terminal ou executá-lo em tela cheia/maximizada**. Isso evita quebras de linha automáticas e garante uma renderização limpa e alinhada da grade.

---

## 🚀 Como Compilar e Executar

Este projeto foi construído para terminais Windows utilizando as bibliotecas `<conio.h>` e chamadas de sistema locais.

### Pré-requisitos
- Compilador C (como `gcc` / MinGW).

### Compilação
Abra o terminal no diretório do projeto e execute:

```bash
gcc main.c -o fazendeiro
```

### Execução
```bash
./fazendeiro
```

Ao iniciar, informe as dimensões desejadas quando solicitado (por exemplo: `10x20` ou `5 10`).
