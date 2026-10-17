# Simulador de Grade de Plantação (Fazendeiro Interativo)

Este projeto consiste em um simulador interativo desenvolvido em linguagem **C**. O software opera como uma **grade de plantação** bidimensional e customizável, onde o usuário pode navegar em tempo real, alterar o estado das células (simulando o plantio e a colheita) e utilizar ferramentas de preenchimento simétrico.

O sistema destaca-se pelo controle direto da renderização do buffer de tela (evitando o "flickering" na atualização de quadros) e pela implementação de operações bit a bit (*bitmasking*) para otimizar a fila de atualizações visuais do terminal.

## Funcionalidades Principais

- **Grade de Plantação Dinâmica:** O usuário define as proporções do terreno na inicialização (ex: `10x20`), com a matriz calculada dinamicamente via alocação de memória.

- **Ferramentas de Simetria (Novo):** O sistema agora oferece suporte a plantio com espelhamento. É possível ativar a simetria no eixo Vertical, Horizontal ou em ambos simultaneamente, permitindo o preenchimento rápido e padronizado da grade de plantação.

- **Renderização Otimizada:** Em vez de redesenhar a tela inteira a cada movimento, o motor gráfico atualiza estritamente os caracteres cujos estados foram alterados por meio do controle preciso das coordenadas do console.

- **Múltiplos Estados de Célula:** Suporte para diferentes identificadores de plantio (Trigo, Alface, Melancia e Cenoura), além da função de limpeza de lote.

## Tabela de Comandos

A navegação e interação ocorrem em tempo real, sem a necessidade de pressionar `Enter` após cada comando.

| Tecla de Entrada | Ação Executada | Representação Visual | 
| ----- | ----- | ----- | 
| `Setas Direcionais` | Movimenta o cursor (Fazendeiro) pela grade | `[P]` | 
| `t` | Define estado da célula: Trigo | `M` | 
| `a` | Define estado da célula: Alface | `@` | 
| `m` | Define estado da célula: Melancia | `O` | 
| `c` | Define estado da célula: Cenoura | `*` | 
| `1` | Colher / Limpar o lote atual | `.` | 
| **`2`** | **Alternar Simetria Vertical (Liga/Desliga)** | - | 
| **`3`** | **Alternar Simetria Horizontal (Liga/Desliga)** | - | 
| `q` / `Q` | Encerrar a execução do programa | - | 

## Arquitetura e Considerações Técnicas

- **Adaptação Multiplataforma:** A arquitetura modular do motor de renderização visa facilitar a portabilidade. Versões em desenvolvimento focarão na adaptação de controle de terminal para abranger de forma nativa sistemas operacionais baseados em Unix (Linux e macOS).

- **Ajuste de Buffer e Limites de Tela:** O sistema calibra o buffer de tela para evitar deformações, porém, **se o tamanho da grade escolhido for maior do que o terminal consegue exibir, ocorrerão quebras de linha forçadas, resultando em erros visuais e de formatação no mapa**. É imprescindível adequar o tamanho do terreno (`x` e `y`) às proporções reais da sua janela de terminal, sendo recomendado executar o programa em tela maximizada.

## Instruções de Compilação e Execução

### Pré-requisitos

- Compilador C padrão (como GCC).

### Processo de Compilação

Navegue até o diretório do arquivo-fonte via terminal e execute o seguinte comando, aplicando otimização e vinculação estática:

```bash
gcc main.c -o fazendeiro -static -O3