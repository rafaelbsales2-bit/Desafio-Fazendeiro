# Simulador de Grade de Plantação (Fazendeiro Interativo)

Este projeto consiste em um simulador interativo desenvolvido em linguagem **C**. O software opera como uma **grade de plantação** bidimensional e customizável, onde o usuário pode navegar em tempo real, alterar o estado das células (simulando o plantio e a colheita) e utilizar ferramentas de preenchimento simétrico.

O sistema destaca-se pelo controle direto da renderização do buffer de tela (evitando o "flickering" na atualização de quadros) e pela implementação de operações bit a bit (*bitmasking*) para otimizar a fila de atualizações visuais do terminal.

## Funcionalidades Principais

* **Grade de Plantação Dinâmica:** O usuário define as proporções do terreno na inicialização (ex: `10x20`), com suporte a até 4096 células calculadas dinamicamente via alocação de memória.

* **Ferramentas de Simetria (Novo):** O sistema agora oferece suporte a plantio com espelhamento. É possível ativar a simetria no eixo Vertical, Horizontal ou em ambos simultaneamente, permitindo o preenchimento rápido e padronizado da grade de plantação.

* **Renderização Otimizada:** Em vez de redesenhar a tela inteira a cada movimento, o motor gráfico atualiza estritamente os caracteres cujos estados foram alterados por meio do controle preciso das coordenadas do console.

* **Múltiplos Estados de Célula:** Suporte para diferentes identificadores de plantio (Trigo, Alface, Melancia e Cenoura), além da função de limpeza de lote.

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

* **Adaptação Multiplataforma:** A arquitetura modular do motor de renderização visa facilitar a portabilidade. Versões em desenvolvimento focarão na adaptação de controle de terminal para abranger de forma nativa sistemas operacionais baseados em Unix (Linux e macOS).

* **Ajuste de Buffer:** O sistema calibra automaticamente o buffer de tela (desativando a quebra de linha e expandindo limites de rolagem) para evitar a deformação do mapa nas extremidades. Recomenda-se executar o programa em tela maximizada para melhor visualização de grandes terrenos.

## Instruções de Compilação e Execução

### Pré-requisitos

* Compilador C padrão (como GCC).

### Processo de Compilação

Navegue até o diretório do arquivo-fonte via terminal e execute o seguinte comando, aplicando otimização e vinculação estática:

```bash
gcc main.c -o fazendeiro -static -O3
```

### Inicialização

Execute o binário compilado:

```bash
./fazendeiro
```

Durante a inicialização, o sistema solicitará o tamanho da matriz. A entrada deve conter os valores numéricos de linhas e colunas (o sistema de *parsing* inteligente ignora letras e caracteres especiais, portanto formatos como `10x15`, `<10 15>`, ou `10,15` são perfeitamente válidos, desde que a multiplicação final dos eixos não exceda 4096).