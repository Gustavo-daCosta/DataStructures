#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

// Estrutura de pilha
typedef struct {
    int itens[MAX];
    int topo;
} Pilha;

// Instânciação da estrutura Pilha
Pilha pilha;

// Inicia a pilha
void initPilha() {
    pilha.topo = -1;
}

// Informa se a pilha está vazia
int isEmpty() {
    return pilha.topo == -1;
}

// Informa se a pilha está cheia
int isFull() {
    return pilha.topo == MAX - 1;
}

// Retorna o tamanho da pilha
int size() {
    return pilha.topo + 1;
}

// Limpa a pilha
void clean() {
    pilha.topo = -1;
}

// Adiciona novo elemento na pilha
void push(int elemento) {
    if (isFull()) {
        printf("A pilha está cheia!\n");
        return;
    }

    pilha.topo++;
    pilha.itens[pilha.topo] = elemento;
}

// Remove um elemento da pilha
int pop() {
    if (isEmpty()) {
        printf("A pilha está vazia!\n");
        return 0;
    }

    pilha.topo--;
    return pilha.itens[pilha.topo];
}

// Pega o último elemento adicionado a pilha
int top() {
    if (isEmpty()) {
        printf("A pilha está vazia!\n");
        return 0;
    }
    return pilha.itens[pilha.topo];
}

// Verifica se o elemento é um número ou um operador
int isOperator(char *elemento) {
    return strlen(elemento) == 1 && strchr("+-*/", elemento[0]) != NULL;
}

// Função para efetuar os cálculos
void calculator(char operacao) {
    if (size() < 2) {
        printf("São necessários dois números na pilha para a operação.\n");
        return;
    }

    int b = pop();
    int a = pop();
    int resultado;

    switch (operacao) {
        case '+':
            resultado = a + b;
            printf("Cálculo: %d + %d = %d\n", a, b, resultado);
            break;
        case '-':
            resultado = a - b;
            printf("Cálculo: %d - %d = %d\n", a, b, resultado);
            break;
        case '*':
            resultado = a * b;
            printf("Cálculo: %d * %d = %d\n", a, b, resultado);
            break;
        case '/':
            if (b == 0) {
                printf("Não é possível dividir por 0.\n");
                return;
            }
            resultado = a / b;
            printf("Cálculo: %d / %d = %d\n", a, b, resultado);
            break;
        default:
            printf("Operador inválido: %c\n", operacao);
            return;
    }

    push(resultado);
}

int main() {
    char expressao[1024];
    printf("Digite a expressão RPN: ");
    fgets(expressao, sizeof(expressao), stdin);

    initPilha();

    char *elemento = strtok(expressao, " \t\n");
    while (elemento != NULL) {
        if (isOperator(elemento)) {
            calculator(elemento[0]);
        } else {
            int numero = atoi(elemento);
            push(numero);
        }
        elemento = strtok(NULL, " \t\n");
    }

    if (size() == 1) {
        int resultado = pop();
        printf("Resultado final: %d\n", resultado);
    } else {
        printf("Expressão malformada. Elementos restantes na pilha: %d\n", size());
    }

    return 0;
}
