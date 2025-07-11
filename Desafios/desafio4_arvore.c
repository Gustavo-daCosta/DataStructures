#include <stdio.h>
#include <stdlib.h>

// Estrutura do nó da árvore binária
typedef struct No {
    int valor;
    struct No* esquerda;
    struct No* direita;
} No;

// Função para criar um novo nó
No* criarNo(int valor) {
    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro: Falha na alocação de memória!\n");
        return NULL;
    }
    
    novo->valor = valor;
    novo->esquerda = NULL;
    novo->direita = NULL;
    
    return novo;
}

// Função para inserir valor na árvore
No* inserir(No* raiz, int valor) {
    if (raiz == NULL) {
        printf("✓ Valor %d inserido na árvore.\n", valor);
        return criarNo(valor);
    }
    
    if (valor < raiz->valor) {
        raiz->esquerda = inserir(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = inserir(raiz->direita, valor);
    } else {
        printf("⚠ Valor %d já existe na árvore.\n", valor);
    }
    
    return raiz;
}

// Função para buscar valor na árvore
No* buscar(No* raiz, int valor) {
    if (raiz == NULL || raiz->valor == valor) {
        return raiz;
    }
    
    if (valor < raiz->valor) {
        return buscar(raiz->esquerda, valor);
    } else {
        return buscar(raiz->direita, valor);
    }
}

// Função para encontrar o menor valor
No* encontrarMinimo(No* raiz) {
    if (raiz == NULL) {
        return NULL;
    }
    
    while (raiz->esquerda != NULL) {
        raiz = raiz->esquerda;
    }
    
    return raiz;
}

// Função para encontrar o maior valor
No* encontrarMaximo(No* raiz) {
    if (raiz == NULL) {
        return NULL;
    }
    
    while (raiz->direita != NULL) {
        raiz = raiz->direita;
    }
    
    return raiz;
}

// Função para remover valor da árvore
No* remover(No* raiz, int valor) {
    if (raiz == NULL) {
        printf("⚠ Valor %d não encontrado na árvore.\n", valor);
        return raiz;
    }
    
    if (valor < raiz->valor) {
        raiz->esquerda = remover(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = remover(raiz->direita, valor);
    } else {
        // Nó encontrado - casos de remoção
        printf("✓ Removendo valor %d da árvore.\n", valor);
        
        // Caso 1: Nó folha (sem filhos)
        if (raiz->esquerda == NULL && raiz->direita == NULL) {
            free(raiz);
            return NULL;
        }
        // Caso 2: Nó com apenas um filho
        else if (raiz->esquerda == NULL) {
            No* temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            No* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        // Caso 3: Nó com dois filhos
        else {
            No* temp = encontrarMinimo(raiz->direita);
            raiz->valor = temp->valor;
            raiz->direita = remover(raiz->direita, temp->valor);
        }
    }
    
    return raiz;
}

// Função para percurso em ordem (in-order)
void percorsoInOrdem(No* raiz) {
    if (raiz != NULL) {
        percorsoInOrdem(raiz->esquerda);
        printf("%d ", raiz->valor);
        percorsoInOrdem(raiz->direita);
    }
}

// Função para percurso pré-ordem (pre-order)
void percorsoPreOrdem(No* raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->valor);
        percorsoPreOrdem(raiz->esquerda);
        percorsoPreOrdem(raiz->direita);
    }
}

// Função para percurso pós-ordem (post-order)
void percorsoPosOrdem(No* raiz) {
    if (raiz != NULL) {
        percorsoPosOrdem(raiz->esquerda);
        percorsoPosOrdem(raiz->direita);
        printf("%d ", raiz->valor);
    }
}

// Função para calcular altura da árvore
int calcularAltura(No* raiz) {
    if (raiz == NULL) {
        return -1;
    }
    
    int alturaEsquerda = calcularAltura(raiz->esquerda);
    int alturaDireita = calcularAltura(raiz->direita);
    
    return 1 + (alturaEsquerda > alturaDireita ? alturaEsquerda : alturaDireita);
}

// Função para contar total de nós
int contarNos(No* raiz) {
    if (raiz == NULL) {
        return 0;
    }
    
    return 1 + contarNos(raiz->esquerda) + contarNos(raiz->direita);
}

// Função para exibir árvore de forma visual (simplificada)
void exibirArvore(No* raiz, int nivel) {
    if (raiz != NULL) {
        exibirArvore(raiz->direita, nivel + 1);
        
        for (int i = 0; i < nivel; i++) {
            printf("    ");
        }
        printf("%d\n", raiz->valor);
        
        exibirArvore(raiz->esquerda, nivel + 1);
    }
}

// Função para liberar toda a árvore
void liberarArvore(No* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

// Função para carregar valores de exemplo
No* carregarExemplos(No* raiz) {
    printf("Carregando valores de exemplo...\n");
    
    int valores[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
    int tamanho = sizeof(valores) / sizeof(valores[0]);
    
    for (int i = 0; i < tamanho; i++) {
        raiz = inserir(raiz, valores[i]);
    }
    
    printf("%d valores inseridos na árvore!\n", tamanho);
    return raiz;
}

// Função para exibir informações da árvore
void exibirInformacoes(No* raiz) {
    if (raiz == NULL) {
        printf("Árvore vazia.\n");
        return;
    }
    
    printf("\n=== INFORMAÇÕES DA ÁRVORE ===\n");
    printf("Total de nós: %d\n", contarNos(raiz));
    printf("Altura: %d\n", calcularAltura(raiz));
    
    No* minimo = encontrarMinimo(raiz);
    No* maximo = encontrarMaximo(raiz);
    
    printf("Menor valor: %d\n", minimo->valor);
    printf("Maior valor: %d\n", maximo->valor);
    printf("Valor da raiz: %d\n", raiz->valor);
}

// Função para exibir menu
void exibirMenu() {
    printf("\n=== ÁRVORE BINÁRIA DE BUSCA ===\n");
    printf("1. Inserir valor\n");
    printf("2. Buscar valor\n");
    printf("3. Remover valor\n");
    printf("4. Percurso em ordem (in-order)\n");
    printf("5. Percurso pré-ordem (pre-order)\n");
    printf("6. Percurso pós-ordem (post-order)\n");
    printf("7. Exibir árvore visual\n");
    printf("8. Exibir informações da árvore\n");
    printf("9. Encontrar menor valor\n");
    printf("10. Encontrar maior valor\n");
    printf("11. Carregar valores de exemplo\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

int main() {
    No* raiz = NULL;
    int opcao, valor;
    
    printf("Bem-vindo ao Sistema de Árvore Binária de Busca!\n");
    printf("Este sistema permite inserir, buscar e remover números inteiros.\n");
    
    while (1) {
        exibirMenu();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                printf("Digite o valor a inserir: ");
                scanf("%d", &valor);
                raiz = inserir(raiz, valor);
                break;
                
            case 2:
                printf("Digite o valor a buscar: ");
                scanf("%d", &valor);
                {
                    No* encontrado = buscar(raiz, valor);
                    if (encontrado != NULL) {
                        printf("✓ Valor %d encontrado na árvore.\n", valor);
                    } else {
                        printf("✗ Valor %d não encontrado na árvore.\n", valor);
                    }
                }
                break;
                
            case 3:
                printf("Digite o valor a remover: ");
                scanf("%d", &valor);
                raiz = remover(raiz, valor);
                break;
                
            case 4:
                printf("Percurso em ordem: ");
                percorsoInOrdem(raiz);
                printf("\n");
                break;
                
            case 5:
                printf("Percurso pré-ordem: ");
                percorsoPreOrdem(raiz);
                printf("\n");
                break;
                
            case 6:
                printf("Percurso pós-ordem: ");
                percorsoPosOrdem(raiz);
                printf("\n");
                break;
                
            case 7:
                printf("\n=== VISUALIZAÇÃO DA ÁRVORE ===\n");
                if (raiz == NULL) {
                    printf("Árvore vazia.\n");
                } else {
                    printf("(Árvore rotacionada 90° - direita em cima, esquerda embaixo)\n\n");
                    exibirArvore(raiz, 0);
                }
                break;
                
            case 8:
                exibirInformacoes(raiz);
                break;
                
            case 9: {
                No* minimo = encontrarMinimo(raiz);
                if (minimo != NULL) {
                    printf("Menor valor na árvore: %d\n", minimo->valor);
                } else {
                    printf("Árvore vazia.\n");
                }
                break;
            }
            
            case 10: {
                No* maximo = encontrarMaximo(raiz);
                if (maximo != NULL) {
                    printf("Maior valor na árvore: %d\n", maximo->valor);
                } else {
                    printf("Árvore vazia.\n");
                }
                break;
            }
            
            case 11:
                raiz = carregarExemplos(raiz);
                break;
                
            case 0:
                printf("Encerrando sistema...\n");
                liberarArvore(raiz);
                printf("Árvore liberada da memória.\n");
                printf("Sistema finalizado.\n");
                return 0;
                
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }
    
    return 0;
}