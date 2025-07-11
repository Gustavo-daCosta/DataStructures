# Desafios de Estruturas de Dados

Este documento contém 5 desafios práticos de estruturas de dados em linguagem C. Cada desafio tem como objetivo exercitar conceitos fundamentais e aplicar diferentes estruturas de dados em problemas reais.

## Desafio 1: Validador de Expressões - Pilha (Stack)

**Problema:** Implemente um validador de expressões matemáticas que verifica se parênteses, colchetes e chaves estão balanceados em uma expressão.

**Estrutura de Dados:** Pilha (Stack)

**Funcionalidades Esperadas:**
- Verificar se cada abertura de parênteses `(`, colchetes `[` ou chaves `{` possui seu correspondente fechamento
- Verificar se a ordem de fechamento está correta
- Retornar se a expressão está válida ou não

**Exemplo de Entrada:**
```
Válidas: "((()))", "[{}]", "({[]})"
Inválidas: "(()", "([)]", "{[}"
```

**Arquivo:** `Desafios/desafio1_pilha.c`

---

## Desafio 2: Simulador de Fila de Processos - Fila (Queue)

**Problema:** Implemente um simulador de fila de processos de um sistema operacional, onde processos são adicionados ao final da fila e executados na ordem de chegada (FIFO).

**Estrutura de Dados:** Fila (Queue)

**Funcionalidades Esperadas:**
- Adicionar processo à fila com ID e tempo de execução
- Executar processo (remover da fila)
- Mostrar próximo processo a ser executado
- Exibir tamanho atual da fila
- Simular execução de todos os processos

**Exemplo de Uso:**
```
Adicionar processo: ID=101, Tempo=5
Adicionar processo: ID=102, Tempo=3
Executar próximo processo: ID=101
Fila atual: 1 processo(s)
```

**Arquivo:** `Desafios/desafio2_fila.c`

---

## Desafio 3: Sistema de Gerenciamento de Estudantes - Lista Ligada

**Problema:** Implemente um sistema de gerenciamento de estudantes usando lista ligada, onde cada estudante possui matrícula, nome e nota.

**Estrutura de Dados:** Lista Ligada (Linked List)

**Funcionalidades Esperadas:**
- Inserir estudante (no início, no final, ou em posição específica)
- Remover estudante por matrícula
- Buscar estudante por matrícula
- Listar todos os estudantes
- Calcular média geral das notas
- Ordenar lista por nota (do maior para menor)

**Exemplo de Estrutura:**
```c
typedef struct Estudante {
    int matricula;
    char nome[50];
    float nota;
    struct Estudante* proximo;
} Estudante;
```

**Arquivo:** `Desafios/desafio3_lista.c`

---

## Desafio 4: Árvore de Busca de Números - Árvore Binária

**Problema:** Implemente uma árvore binária de busca para armazenar números inteiros, permitindo inserção, busca e diferentes tipos de percurso.

**Estrutura de Dados:** Árvore Binária de Busca (Binary Search Tree)

**Funcionalidades Esperadas:**
- Inserir número na árvore
- Buscar número na árvore
- Remover número da árvore
- Percurso em ordem (in-order)
- Percurso pré-ordem (pre-order)
- Percurso pós-ordem (post-order)
- Encontrar menor e maior valor
- Calcular altura da árvore

**Exemplo de Estrutura:**
```c
typedef struct No {
    int valor;
    struct No* esquerda;
    struct No* direita;
} No;
```

**Arquivo:** `Desafios/desafio4_arvore.c`

---

## Desafio 5: Dicionário de Palavras - Tabela Hash

**Problema:** Implemente um dicionário de palavras usando tabela hash, onde cada palavra possui uma definição associada.

**Estrutura de Dados:** Tabela Hash (Hash Table)

**Funcionalidades Esperadas:**
- Inserir palavra com sua definição
- Buscar definição de uma palavra
- Remover palavra do dicionário
- Listar todas as palavras
- Tratar colisões usando encadeamento
- Calcular fator de carga da tabela

**Exemplo de Estrutura:**
```c
typedef struct ItemDicionario {
    char palavra[50];
    char definicao[200];
    struct ItemDicionario* proximo;
} ItemDicionario;

typedef struct {
    ItemDicionario* tabela[TAMANHO_HASH];
    int tamanho;
} Dicionario;
```

**Arquivo:** `Desafios/desafio5_hash.c`

---

## Como Compilar e Executar

Para compilar qualquer um dos desafios, use:

```bash
cd Desafios/
gcc -o desafio1 desafio1_pilha.c
./desafio1
```

## Critérios de Avaliação

- **Corretude:** O código implementa corretamente a estrutura de dados
- **Eficiência:** Operações implementadas com complexidade adequada
- **Organização:** Código bem estruturado e comentado
- **Tratamento de Erros:** Validação de entradas e tratamento de casos extremos
- **Funcionalidade:** Todas as funcionalidades solicitadas estão implementadas

## Recursos Adicionais

- Utilize as funções auxiliares do projeto CalculadoraRPN como referência
- Implemente validação de entrada em todas as operações
- Adicione mensagens de erro descritivas
- Teste com diferentes cenários de entrada

Boa sorte com os desafios! 🚀