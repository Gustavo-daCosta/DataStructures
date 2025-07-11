#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_HASH 101
#define MAX_PALAVRA 50
#define MAX_DEFINICAO 200

// Estrutura para um item do dicionário
typedef struct ItemDicionario {
    char palavra[MAX_PALAVRA];
    char definicao[MAX_DEFINICAO];
    struct ItemDicionario* proximo;
} ItemDicionario;

// Estrutura para a tabela hash (dicionário)
typedef struct {
    ItemDicionario* tabela[TAMANHO_HASH];
    int totalItens;
} Dicionario;

// Função hash simples
unsigned int funcaoHash(char* palavra) {
    unsigned int hash = 0;
    int i = 0;
    
    while (palavra[i] != '\0') {
        hash = hash * 31 + palavra[i];
        i++;
    }
    
    return hash % TAMANHO_HASH;
}

// Função para inicializar o dicionário
void iniciarDicionario(Dicionario* dicionario) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        dicionario->tabela[i] = NULL;
    }
    dicionario->totalItens = 0;
}

// Função para criar um novo item
ItemDicionario* criarItem(char* palavra, char* definicao) {
    ItemDicionario* novo = (ItemDicionario*)malloc(sizeof(ItemDicionario));
    if (novo == NULL) {
        printf("Erro: Falha na alocação de memória!\n");
        return NULL;
    }
    
    strcpy(novo->palavra, palavra);
    strcpy(novo->definicao, definicao);
    novo->proximo = NULL;
    
    return novo;
}

// Função para inserir palavra no dicionário
int inserirPalavra(Dicionario* dicionario, char* palavra, char* definicao) {
    if (strlen(palavra) == 0 || strlen(definicao) == 0) {
        printf("Erro: Palavra e definição não podem estar vazias!\n");
        return 0;
    }
    
    unsigned int indice = funcaoHash(palavra);
    ItemDicionario* atual = dicionario->tabela[indice];
    
    // Verificar se palavra já existe
    while (atual != NULL) {
        if (strcmp(atual->palavra, palavra) == 0) {
            printf("⚠ Palavra '%s' já existe. Atualizando definição...\n", palavra);
            strcpy(atual->definicao, definicao);
            return 1;
        }
        atual = atual->proximo;
    }
    
    // Inserir novo item no início da lista (encadeamento)
    ItemDicionario* novo = criarItem(palavra, definicao);
    if (novo == NULL) return 0;
    
    novo->proximo = dicionario->tabela[indice];
    dicionario->tabela[indice] = novo;
    dicionario->totalItens++;
    
    printf("✓ Palavra '%s' inserida no dicionário.\n", palavra);
    return 1;
}

// Função para buscar definição de uma palavra
char* buscarDefinicao(Dicionario* dicionario, char* palavra) {
    unsigned int indice = funcaoHash(palavra);
    ItemDicionario* atual = dicionario->tabela[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->palavra, palavra) == 0) {
            return atual->definicao;
        }
        atual = atual->proximo;
    }
    
    return NULL; // Palavra não encontrada
}

// Função para remover palavra do dicionário
int removerPalavra(Dicionario* dicionario, char* palavra) {
    unsigned int indice = funcaoHash(palavra);
    ItemDicionario* atual = dicionario->tabela[indice];
    ItemDicionario* anterior = NULL;
    
    while (atual != NULL) {
        if (strcmp(atual->palavra, palavra) == 0) {
            // Palavra encontrada - remover
            if (anterior == NULL) {
                // Primeiro da lista
                dicionario->tabela[indice] = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            
            printf("✓ Palavra '%s' removida do dicionário.\n", palavra);
            free(atual);
            dicionario->totalItens--;
            return 1;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    
    printf("✗ Palavra '%s' não encontrada no dicionário.\n", palavra);
    return 0;
}

// Função para listar todas as palavras
void listarPalavras(Dicionario* dicionario) {
    if (dicionario->totalItens == 0) {
        printf("Dicionário vazio.\n");
        return;
    }
    
    printf("\n=== DICIONÁRIO COMPLETO ===\n");
    printf("Total de palavras: %d\n\n", dicionario->totalItens);
    
    int contador = 1;
    for (int i = 0; i < TAMANHO_HASH; i++) {
        ItemDicionario* atual = dicionario->tabela[i];
        while (atual != NULL) {
            printf("%d. %s\n", contador, atual->palavra);
            printf("   %s\n\n", atual->definicao);
            contador++;
            atual = atual->proximo;
        }
    }
}

// Função para calcular fator de carga
float calcularFatorCarga(Dicionario* dicionario) {
    return (float)dicionario->totalItens / TAMANHO_HASH;
}

// Função para exibir estatísticas da tabela hash
void exibirEstatisticas(Dicionario* dicionario) {
    printf("\n=== ESTATÍSTICAS DA TABELA HASH ===\n");
    printf("Tamanho da tabela: %d\n", TAMANHO_HASH);
    printf("Total de itens: %d\n", dicionario->totalItens);
    printf("Fator de carga: %.4f\n", calcularFatorCarga(dicionario));
    
    // Contar colisões
    int slotsOcupados = 0;
    int maxColisoes = 0;
    int totalColisoes = 0;
    
    for (int i = 0; i < TAMANHO_HASH; i++) {
        if (dicionario->tabela[i] != NULL) {
            slotsOcupados++;
            
            int colisoes = 0;
            ItemDicionario* atual = dicionario->tabela[i];
            while (atual != NULL) {
                colisoes++;
                atual = atual->proximo;
            }
            
            if (colisoes > 1) {
                totalColisoes += colisoes - 1;
                if (colisoes > maxColisoes) {
                    maxColisoes = colisoes;
                }
            }
        }
    }
    
    printf("Slots ocupados: %d/%d (%.2f%%)\n", 
           slotsOcupados, TAMANHO_HASH, 
           (float)slotsOcupados / TAMANHO_HASH * 100);
    printf("Total de colisões: %d\n", totalColisoes);
    printf("Maior cadeia: %d itens\n", maxColisoes);
}

// Função para liberar todo o dicionário
void liberarDicionario(Dicionario* dicionario) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        ItemDicionario* atual = dicionario->tabela[i];
        while (atual != NULL) {
            ItemDicionario* proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
        dicionario->tabela[i] = NULL;
    }
    dicionario->totalItens = 0;
}

// Função para carregar palavras de exemplo
void carregarExemplos(Dicionario* dicionario) {
    printf("Carregando dicionário de exemplo...\n");
    
    inserirPalavra(dicionario, "algoritmo", "Sequência finita de instruções para resolver um problema");
    inserirPalavra(dicionario, "estrutura", "Organização e arranjo de dados na memória");
    inserirPalavra(dicionario, "hash", "Função que mapeia dados de tamanho variável para valores de tamanho fixo");
    inserirPalavra(dicionario, "colisao", "Quando duas chaves diferentes produzem o mesmo valor hash");
    inserirPalavra(dicionario, "encadeamento", "Método para resolver colisões usando listas ligadas");
    inserirPalavra(dicionario, "tabela", "Estrutura de dados que associa chaves a valores");
    inserirPalavra(dicionario, "busca", "Operação para encontrar um elemento em uma estrutura de dados");
    inserirPalavra(dicionario, "insercao", "Operação para adicionar um novo elemento");
    inserirPalavra(dicionario, "remocao", "Operação para eliminar um elemento existente");
    inserirPalavra(dicionario, "complexidade", "Medida de recursos necessários para executar um algoritmo");
    
    printf("10 palavras de exemplo adicionadas ao dicionário!\n");
}

// Função para buscar palavras por prefixo
void buscarPorPrefixo(Dicionario* dicionario, char* prefixo) {
    printf("\nPalavras que começam com '%s':\n", prefixo);
    int encontradas = 0;
    
    for (int i = 0; i < TAMANHO_HASH; i++) {
        ItemDicionario* atual = dicionario->tabela[i];
        while (atual != NULL) {
            if (strncmp(atual->palavra, prefixo, strlen(prefixo)) == 0) {
                printf("- %s: %s\n", atual->palavra, atual->definicao);
                encontradas++;
            }
            atual = atual->proximo;
        }
    }
    
    if (encontradas == 0) {
        printf("Nenhuma palavra encontrada com o prefixo '%s'.\n", prefixo);
    } else {
        printf("Total: %d palavra(s) encontrada(s).\n", encontradas);
    }
}

// Função para exibir menu
void exibirMenu() {
    printf("\n=== DICIONÁRIO DE PALAVRAS ===\n");
    printf("1. Inserir palavra\n");
    printf("2. Buscar definição\n");
    printf("3. Remover palavra\n");
    printf("4. Listar todas as palavras\n");
    printf("5. Buscar por prefixo\n");
    printf("6. Exibir estatísticas\n");
    printf("7. Carregar dicionário de exemplo\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

int main() {
    Dicionario dicionario;
    iniciarDicionario(&dicionario);
    
    int opcao;
    char palavra[MAX_PALAVRA];
    char definicao[MAX_DEFINICAO];
    char prefixo[MAX_PALAVRA];
    
    printf("Bem-vindo ao Dicionário de Palavras!\n");
    printf("Este sistema utiliza tabela hash com encadeamento para resolver colisões.\n");
    
    while (1) {
        exibirMenu();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                printf("Digite a palavra: ");
                scanf("%s", palavra);
                printf("Digite a definição: ");
                getchar(); // Limpar buffer
                fgets(definicao, sizeof(definicao), stdin);
                definicao[strcspn(definicao, "\n")] = 0; // Remover quebra de linha
                
                inserirPalavra(&dicionario, palavra, definicao);
                break;
                
            case 2:
                printf("Digite a palavra a buscar: ");
                scanf("%s", palavra);
                {
                    char* definicaoEncontrada = buscarDefinicao(&dicionario, palavra);
                    if (definicaoEncontrada != NULL) {
                        printf("✓ %s: %s\n", palavra, definicaoEncontrada);
                    } else {
                        printf("✗ Palavra '%s' não encontrada no dicionário.\n", palavra);
                    }
                }
                break;
                
            case 3:
                printf("Digite a palavra a remover: ");
                scanf("%s", palavra);
                removerPalavra(&dicionario, palavra);
                break;
                
            case 4:
                listarPalavras(&dicionario);
                break;
                
            case 5:
                printf("Digite o prefixo: ");
                scanf("%s", prefixo);
                buscarPorPrefixo(&dicionario, prefixo);
                break;
                
            case 6:
                exibirEstatisticas(&dicionario);
                break;
                
            case 7:
                carregarExemplos(&dicionario);
                break;
                
            case 0:
                printf("Encerrando dicionário...\n");
                liberarDicionario(&dicionario);
                printf("Memória liberada.\n");
                printf("Dicionário finalizado.\n");
                return 0;
                
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }
    
    return 0;
}