#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura do nó da lista ligada
typedef struct Estudante {
    int matricula;
    char nome[50];
    float nota;
    struct Estudante* proximo;
} Estudante;

// Estrutura para a lista de estudantes
typedef struct {
    Estudante* primeiro;
    int tamanho;
} ListaEstudantes;

// Função para inicializar a lista
void iniciarLista(ListaEstudantes* lista) {
    lista->primeiro = NULL;
    lista->tamanho = 0;
}

// Função para criar um novo estudante
Estudante* criarEstudante(int matricula, char* nome, float nota) {
    Estudante* novo = (Estudante*)malloc(sizeof(Estudante));
    if (novo == NULL) {
        printf("Erro: Falha na alocação de memória!\n");
        return NULL;
    }
    
    novo->matricula = matricula;
    strcpy(novo->nome, nome);
    novo->nota = nota;
    novo->proximo = NULL;
    
    return novo;
}

// Função para inserir estudante no início da lista
int inserirInicio(ListaEstudantes* lista, int matricula, char* nome, float nota) {
    // Verificar se matrícula já existe
    Estudante* atual = lista->primeiro;
    while (atual != NULL) {
        if (atual->matricula == matricula) {
            printf("Erro: Matrícula %d já existe!\n", matricula);
            return 0;
        }
        atual = atual->proximo;
    }
    
    Estudante* novo = criarEstudante(matricula, nome, nota);
    if (novo == NULL) return 0;
    
    novo->proximo = lista->primeiro;
    lista->primeiro = novo;
    lista->tamanho++;
    
    printf("✓ Estudante inserido no início: %s (Matrícula: %d, Nota: %.2f)\n", 
           nome, matricula, nota);
    return 1;
}

// Função para inserir estudante no final da lista
int inserirFinal(ListaEstudantes* lista, int matricula, char* nome, float nota) {
    // Verificar se matrícula já existe
    Estudante* atual = lista->primeiro;
    while (atual != NULL) {
        if (atual->matricula == matricula) {
            printf("Erro: Matrícula %d já existe!\n", matricula);
            return 0;
        }
        atual = atual->proximo;
    }
    
    Estudante* novo = criarEstudante(matricula, nome, nota);
    if (novo == NULL) return 0;
    
    if (lista->primeiro == NULL) {
        lista->primeiro = novo;
    } else {
        atual = lista->primeiro;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novo;
    }
    
    lista->tamanho++;
    printf("✓ Estudante inserido no final: %s (Matrícula: %d, Nota: %.2f)\n", 
           nome, matricula, nota);
    return 1;
}

// Função para inserir estudante em posição específica
int inserirPosicao(ListaEstudantes* lista, int posicao, int matricula, char* nome, float nota) {
    if (posicao < 0 || posicao > lista->tamanho) {
        printf("Erro: Posição inválida! (0 a %d)\n", lista->tamanho);
        return 0;
    }
    
    if (posicao == 0) {
        return inserirInicio(lista, matricula, nome, nota);
    }
    
    // Verificar se matrícula já existe
    Estudante* atual = lista->primeiro;
    while (atual != NULL) {
        if (atual->matricula == matricula) {
            printf("Erro: Matrícula %d já existe!\n", matricula);
            return 0;
        }
        atual = atual->proximo;
    }
    
    Estudante* novo = criarEstudante(matricula, nome, nota);
    if (novo == NULL) return 0;
    
    atual = lista->primeiro;
    for (int i = 0; i < posicao - 1; i++) {
        atual = atual->proximo;
    }
    
    novo->proximo = atual->proximo;
    atual->proximo = novo;
    lista->tamanho++;
    
    printf("✓ Estudante inserido na posição %d: %s (Matrícula: %d, Nota: %.2f)\n", 
           posicao, nome, matricula, nota);
    return 1;
}

// Função para remover estudante por matrícula
int removerEstudante(ListaEstudantes* lista, int matricula) {
    if (lista->primeiro == NULL) {
        printf("Erro: Lista vazia!\n");
        return 0;
    }
    
    Estudante* atual = lista->primeiro;
    Estudante* anterior = NULL;
    
    // Procurar estudante
    while (atual != NULL && atual->matricula != matricula) {
        anterior = atual;
        atual = atual->proximo;
    }
    
    if (atual == NULL) {
        printf("Erro: Estudante com matrícula %d não encontrado!\n", matricula);
        return 0;
    }
    
    // Remover estudante
    if (anterior == NULL) {
        // Primeiro da lista
        lista->primeiro = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }
    
    printf("✓ Estudante removido: %s (Matrícula: %d)\n", atual->nome, atual->matricula);
    free(atual);
    lista->tamanho--;
    return 1;
}

// Função para buscar estudante por matrícula
Estudante* buscarEstudante(ListaEstudantes* lista, int matricula) {
    Estudante* atual = lista->primeiro;
    
    while (atual != NULL) {
        if (atual->matricula == matricula) {
            return atual;
        }
        atual = atual->proximo;
    }
    
    return NULL;
}

// Função para listar todos os estudantes
void listarEstudantes(ListaEstudantes* lista) {
    if (lista->primeiro == NULL) {
        printf("Lista vazia.\n");
        return;
    }
    
    printf("\n=== LISTA DE ESTUDANTES ===\n");
    printf("Total: %d estudante(s)\n\n", lista->tamanho);
    printf("%-10s %-20s %-8s\n", "Matrícula", "Nome", "Nota");
    printf("----------------------------------------\n");
    
    Estudante* atual = lista->primeiro;
    while (atual != NULL) {
        printf("%-10d %-20s %-8.2f\n", atual->matricula, atual->nome, atual->nota);
        atual = atual->proximo;
    }
    printf("\n");
}

// Função para calcular média geral das notas
float calcularMediaGeral(ListaEstudantes* lista) {
    if (lista->primeiro == NULL) {
        return 0.0;
    }
    
    float somaNotas = 0.0;
    Estudante* atual = lista->primeiro;
    
    while (atual != NULL) {
        somaNotas += atual->nota;
        atual = atual->proximo;
    }
    
    return somaNotas / lista->tamanho;
}

// Função para ordenar lista por nota (decrescente)
void ordenarPorNota(ListaEstudantes* lista) {
    if (lista->primeiro == NULL || lista->primeiro->proximo == NULL) {
        return; // Lista vazia ou com apenas um elemento
    }
    
    // Bubble sort adaptado para lista ligada
    int trocou;
    do {
        trocou = 0;
        Estudante* atual = lista->primeiro;
        
        while (atual->proximo != NULL) {
            if (atual->nota < atual->proximo->nota) {
                // Trocar dados dos nós
                int tempMatricula = atual->matricula;
                char tempNome[50];
                strcpy(tempNome, atual->nome);
                float tempNota = atual->nota;
                
                atual->matricula = atual->proximo->matricula;
                strcpy(atual->nome, atual->proximo->nome);
                atual->nota = atual->proximo->nota;
                
                atual->proximo->matricula = tempMatricula;
                strcpy(atual->proximo->nome, tempNome);
                atual->proximo->nota = tempNota;
                
                trocou = 1;
            }
            atual = atual->proximo;
        }
    } while (trocou);
    
    printf("✓ Lista ordenada por nota (decrescente).\n");
}

// Função para liberar toda a lista
void liberarLista(ListaEstudantes* lista) {
    Estudante* atual = lista->primeiro;
    while (atual != NULL) {
        Estudante* proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    lista->primeiro = NULL;
    lista->tamanho = 0;
}

// Função para carregar dados de exemplo
void carregarExemplos(ListaEstudantes* lista) {
    printf("Carregando estudantes de exemplo...\n");
    
    inserirFinal(lista, 2023001, "Ana Silva", 8.5);
    inserirFinal(lista, 2023002, "Bruno Santos", 7.2);
    inserirFinal(lista, 2023003, "Carlos Lima", 9.1);
    inserirFinal(lista, 2023004, "Diana Costa", 6.8);
    inserirFinal(lista, 2023005, "Eduardo Rocha", 8.9);
    
    printf("5 estudantes de exemplo adicionados!\n");
}

// Função para exibir menu
void exibirMenu() {
    printf("\n=== SISTEMA DE GERENCIAMENTO DE ESTUDANTES ===\n");
    printf("1. Inserir estudante no início\n");
    printf("2. Inserir estudante no final\n");
    printf("3. Inserir estudante em posição específica\n");
    printf("4. Remover estudante por matrícula\n");
    printf("5. Buscar estudante por matrícula\n");
    printf("6. Listar todos os estudantes\n");
    printf("7. Calcular média geral das notas\n");
    printf("8. Ordenar por nota (maior para menor)\n");
    printf("9. Carregar dados de exemplo\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

int main() {
    ListaEstudantes lista;
    iniciarLista(&lista);
    
    int opcao, matricula, posicao;
    char nome[50];
    float nota;
    
    printf("Bem-vindo ao Sistema de Gerenciamento de Estudantes!\n");
    printf("Este sistema utiliza lista ligada para armazenar informações dos estudantes.\n");
    
    while (1) {
        exibirMenu();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                printf("Matrícula: ");
                scanf("%d", &matricula);
                printf("Nome: ");
                scanf("%s", nome);
                printf("Nota (0-10): ");
                scanf("%f", &nota);
                
                if (nota < 0 || nota > 10) {
                    printf("Erro: Nota deve estar entre 0 e 10!\n");
                } else {
                    inserirInicio(&lista, matricula, nome, nota);
                }
                break;
                
            case 2:
                printf("Matrícula: ");
                scanf("%d", &matricula);
                printf("Nome: ");
                scanf("%s", nome);
                printf("Nota (0-10): ");
                scanf("%f", &nota);
                
                if (nota < 0 || nota > 10) {
                    printf("Erro: Nota deve estar entre 0 e 10!\n");
                } else {
                    inserirFinal(&lista, matricula, nome, nota);
                }
                break;
                
            case 3:
                printf("Posição (0 a %d): ", lista.tamanho);
                scanf("%d", &posicao);
                printf("Matrícula: ");
                scanf("%d", &matricula);
                printf("Nome: ");
                scanf("%s", nome);
                printf("Nota (0-10): ");
                scanf("%f", &nota);
                
                if (nota < 0 || nota > 10) {
                    printf("Erro: Nota deve estar entre 0 e 10!\n");
                } else {
                    inserirPosicao(&lista, posicao, matricula, nome, nota);
                }
                break;
                
            case 4:
                printf("Matrícula do estudante a remover: ");
                scanf("%d", &matricula);
                removerEstudante(&lista, matricula);
                break;
                
            case 5:
                printf("Matrícula do estudante a buscar: ");
                scanf("%d", &matricula);
                {
                    Estudante* encontrado = buscarEstudante(&lista, matricula);
                    if (encontrado != NULL) {
                        printf("✓ Estudante encontrado:\n");
                        printf("  Matrícula: %d\n", encontrado->matricula);
                        printf("  Nome: %s\n", encontrado->nome);
                        printf("  Nota: %.2f\n", encontrado->nota);
                    } else {
                        printf("✗ Estudante com matrícula %d não encontrado.\n", matricula);
                    }
                }
                break;
                
            case 6:
                listarEstudantes(&lista);
                break;
                
            case 7: {
                float media = calcularMediaGeral(&lista);
                if (lista.tamanho > 0) {
                    printf("Média geral das notas: %.2f\n", media);
                } else {
                    printf("Não há estudantes cadastrados.\n");
                }
                break;
            }
            
            case 8:
                ordenarPorNota(&lista);
                break;
                
            case 9:
                carregarExemplos(&lista);
                break;
                
            case 0:
                printf("Encerrando sistema...\n");
                liberarLista(&lista);
                printf("Sistema finalizado.\n");
                return 0;
                
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }
    
    return 0;
}