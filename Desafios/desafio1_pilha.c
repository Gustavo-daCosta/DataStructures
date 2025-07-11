#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

// Estrutura da Pilha
typedef struct {
    char itens[MAX_SIZE];
    int topo;
} Pilha;

// Função para inicializar a pilha
void iniciarPilha(Pilha* p) {
    p->topo = -1;
}

// Função para verificar se a pilha está vazia
int pilhaVazia(Pilha* p) {
    return p->topo == -1;
}

// Função para verificar se a pilha está cheia
int pilhaCheia(Pilha* p) {
    return p->topo == MAX_SIZE - 1;
}

// Função para empilhar
void empilhar(Pilha* p, char item) {
    if (pilhaCheia(p)) {
        printf("Erro: Pilha cheia!\n");
        return;
    }
    p->itens[++p->topo] = item;
}

// Função para desempilhar
char desempilhar(Pilha* p) {
    if (pilhaVazia(p)) {
        printf("Erro: Pilha vazia!\n");
        return '\0';
    }
    return p->itens[p->topo--];
}

// Função para obter o topo da pilha
char obterTopo(Pilha* p) {
    if (pilhaVazia(p)) {
        return '\0';
    }
    return p->itens[p->topo];
}

// Função para verificar se um caractere é de abertura
int ehAbertura(char c) {
    return (c == '(' || c == '[' || c == '{');
}

// Função para verificar se um caractere é de fechamento
int ehFechamento(char c) {
    return (c == ')' || c == ']' || c == '}');
}

// Função para verificar se os caracteres formam um par válido
int ehParValido(char abertura, char fechamento) {
    return ((abertura == '(' && fechamento == ')') ||
            (abertura == '[' && fechamento == ']') ||
            (abertura == '{' && fechamento == '}'));
}

// Função principal para validar expressão
int validarExpressao(char* expressao) {
    Pilha pilha;
    iniciarPilha(&pilha);
    
    int tamanho = strlen(expressao);
    
    for (int i = 0; i < tamanho; i++) {
        char atual = expressao[i];
        
        if (ehAbertura(atual)) {
            empilhar(&pilha, atual);
        } else if (ehFechamento(atual)) {
            if (pilhaVazia(&pilha)) {
                return 0; // Fechamento sem abertura correspondente
            }
            
            char topo = desempilhar(&pilha);
            if (!ehParValido(topo, atual)) {
                return 0; // Par inválido
            }
        }
    }
    
    return pilhaVazia(&pilha); // Válido se pilha estiver vazia ao final
}

// Função para exibir resultado
void exibirResultado(char* expressao, int valida) {
    printf("Expressão: %s\n", expressao);
    if (valida) {
        printf("✓ Expressão VÁLIDA - parênteses, colchetes e chaves estão balanceados\n");
    } else {
        printf("✗ Expressão INVÁLIDA - parênteses, colchetes e chaves não estão balanceados\n");
    }
    printf("\n");
}

int main() {
    char expressao[MAX_SIZE];
    
    printf("=== VALIDADOR DE EXPRESSÕES ===\n");
    printf("Este programa verifica se parênteses (), colchetes [] e chaves {} estão balanceados.\n\n");
    
    // Casos de teste predefinidos
    char* testes[] = {
        "((()))",
        "[{}]", 
        "({[]})",
        "(()",
        "([)]",
        "{[}",
        "()[]{}",
        "({[()]})",
        "(((",
        ")))",
        ""
    };
    
    int numTestes = sizeof(testes) / sizeof(testes[0]);
    
    printf("=== CASOS DE TESTE ===\n");
    for (int i = 0; i < numTestes; i++) {
        int resultado = validarExpressao(testes[i]);
        exibirResultado(testes[i], resultado);
    }
    
    // Entrada interativa
    printf("=== TESTE INTERATIVO ===\n");
    printf("Digite uma expressão para validar (ou 'sair' para terminar):\n");
    
    while (1) {
        printf("> ");
        if (fgets(expressao, sizeof(expressao), stdin) == NULL) {
            break;
        }
        
        // Remove quebra de linha
        expressao[strcspn(expressao, "\n")] = 0;
        
        if (strcmp(expressao, "sair") == 0) {
            break;
        }
        
        if (strlen(expressao) == 0) {
            printf("Expressão vazia é considerada válida.\n\n");
            continue;
        }
        
        int resultado = validarExpressao(expressao);
        exibirResultado(expressao, resultado);
    }
    
    printf("Programa finalizado.\n");
    return 0;
}