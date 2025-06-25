# Calculadora em Notação Polonesa Reversa

Desenvolvimento de uma calculadora, em linguagem C, utilizando a estrutura de Notação Polonesa Reversa, 

## Estrutura do código

Primeiramente, utilizando o TAD descrito no enunciado, criei a estrutura de Pilha:

```C
typedef struct {
    int itens[MAX]; // MAX = 100
    int topo;
} Pilha;
```

Depois, declarei uma série de funções que auxiliam no desenvolvimento, sendo elas:

- `initPilha()`: Inicia a pilha
- `isEmpty() -> int`: Se a pilha estiver vazia, retorna 1, agora caso a pilha contenha valores, retorna 0
- `isFull() -> int`: Se a pilha estiver cheia, retorna 1, caso não esteja, retorna 0
- `size() -> int` -> int: Retorna o tamanho da pilha
- `push(int elemento)`: Adiciona um novo elemento a pilha
- `pop() -> int`: Remove o último item adicionado a pilha, e retorna o item excluído
- `top() -> int`: Retorna o item no topo da pilha, ou seja, o último item a ser adicionado
- `isOperator(char elemento) -> int`: Verifica se o item é um operador ou um número
- `calculator(char operacao)`: Função que tem a responsabilidade de fazer o cálculo, seguindo o padrão da Notação Polonesa Reversa
- `main()`: Função principal do código, onde os inputs são recebidos e as funções são invocadas

## Restrições e tratamento de erros

Existem algumas limitações no código, e essas limitações estão acompanhadas de tratametnos de erros, como:

1. Tamanho da pilha

A pilha possui um tamanho máximo de 100 itens, e consequentemente existe um tratamento de erros que impede a adição de novos valores quando a pilha atinge o tamanho máximo.
Além disso, também existe um tratamento para impedir a remoção de valores da lista quando ela está vazia

2. Operações

Só é possível utilizar as 4 operações básicas, sendo elas a adição, subtração, multiplicação e divisão, logo existe um tratamento de erros para impedir que apenas estas operações sejam utilizadas.

Além disso, dentro das próprias operações existe esse tratamento, por exemplo, impedir a divisão por zero.

3. Mensagens de erro

Todo local com tratamento de erros, retorna mensagens de erros descritivas, explicando qual é o problema.
