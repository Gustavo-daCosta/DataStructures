#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PESSOAS 50
#define MAX_NOME 30

// Estrutura para representar uma conexão (amizade)
typedef struct NoAmizade {
    int idAmigo;
    struct NoAmizade* proximo;
} NoAmizade;

// Estrutura para representar uma pessoa na rede social
typedef struct {
    int id;
    char nome[MAX_NOME];
    NoAmizade* amigos;  // Lista de adjacência
    int ativo;          // 1 se a pessoa existe, 0 caso contrário
} Pessoa;

// Estrutura para o grafo da rede social
typedef struct {
    Pessoa pessoas[MAX_PESSOAS];
    int totalPessoas;
} RedeSocial;

// Estrutura para fila (usada no BFS)
typedef struct {
    int itens[MAX_PESSOAS];
    int inicio, fim;
} Fila;

// Funções da fila
void iniciarFila(Fila* f) {
    f->inicio = f->fim = -1;
}

int filaVazia(Fila* f) {
    return f->inicio == -1;
}

void enfileirar(Fila* f, int item) {
    if (f->inicio == -1) {
        f->inicio = f->fim = 0;
    } else {
        f->fim++;
    }
    f->itens[f->fim] = item;
}

int desenfileirar(Fila* f) {
    if (filaVazia(f)) return -1;
    int item = f->itens[f->inicio];
    if (f->inicio == f->fim) {
        f->inicio = f->fim = -1;
    } else {
        f->inicio++;
    }
    return item;
}

// Funções da Rede Social
void iniciarRedeSocial(RedeSocial* rede) {
    rede->totalPessoas = 0;
    for (int i = 0; i < MAX_PESSOAS; i++) {
        rede->pessoas[i].ativo = 0;
        rede->pessoas[i].amigos = NULL;
    }
}

// Função para encontrar pessoa por ID
int encontrarPessoa(RedeSocial* rede, int id) {
    for (int i = 0; i < MAX_PESSOAS; i++) {
        if (rede->pessoas[i].ativo && rede->pessoas[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Função para adicionar pessoa à rede social
void adicionarPessoa(RedeSocial* rede, int id, char* nome) {
    if (rede->totalPessoas >= MAX_PESSOAS) {
        printf("Erro: Rede social lotada!\n");
        return;
    }
    
    if (encontrarPessoa(rede, id) != -1) {
        printf("Erro: Pessoa com ID %d já existe!\n", id);
        return;
    }
    
    // Encontrar primeira posição livre
    for (int i = 0; i < MAX_PESSOAS; i++) {
        if (!rede->pessoas[i].ativo) {
            rede->pessoas[i].id = id;
            strcpy(rede->pessoas[i].nome, nome);
            rede->pessoas[i].amigos = NULL;
            rede->pessoas[i].ativo = 1;
            rede->totalPessoas++;
            printf("Pessoa %s (ID: %d) adicionada com sucesso!\n", nome, id);
            return;
        }
    }
}

// Função para adicionar amizade (conexão bidirecional)
void adicionarAmizade(RedeSocial* rede, int id1, int id2) {
    int pos1 = encontrarPessoa(rede, id1);
    int pos2 = encontrarPessoa(rede, id2);
    
    if (pos1 == -1 || pos2 == -1) {
        printf("Erro: Uma ou ambas as pessoas não existem!\n");
        return;
    }
    
    if (id1 == id2) {
        printf("Erro: Uma pessoa não pode ser amiga de si mesma!\n");
        return;
    }
    
    // Verificar se já são amigos
    NoAmizade* atual = rede->pessoas[pos1].amigos;
    while (atual != NULL) {
        if (atual->idAmigo == id2) {
            printf("Erro: %s e %s já são amigos!\n", 
                   rede->pessoas[pos1].nome, rede->pessoas[pos2].nome);
            return;
        }
        atual = atual->proximo;
    }
    
    // Adicionar id2 à lista de amigos de id1
    NoAmizade* novoAmigo1 = (NoAmizade*)malloc(sizeof(NoAmizade));
    novoAmigo1->idAmigo = id2;
    novoAmigo1->proximo = rede->pessoas[pos1].amigos;
    rede->pessoas[pos1].amigos = novoAmigo1;
    
    // Adicionar id1 à lista de amigos de id2
    NoAmizade* novoAmigo2 = (NoAmizade*)malloc(sizeof(NoAmizade));
    novoAmigo2->idAmigo = id1;
    novoAmigo2->proximo = rede->pessoas[pos2].amigos;
    rede->pessoas[pos2].amigos = novoAmigo2;
    
    printf("Amizade criada entre %s e %s!\n", 
           rede->pessoas[pos1].nome, rede->pessoas[pos2].nome);
}

// Função para listar amigos de uma pessoa
void listarAmigos(RedeSocial* rede, int id) {
    int pos = encontrarPessoa(rede, id);
    if (pos == -1) {
        printf("Pessoa com ID %d não encontrada!\n", id);
        return;
    }
    
    printf("\nAmigos de %s (ID: %d):\n", rede->pessoas[pos].nome, id);
    NoAmizade* atual = rede->pessoas[pos].amigos;
    int contador = 0;
    
    while (atual != NULL) {
        int posAmigo = encontrarPessoa(rede, atual->idAmigo);
        if (posAmigo != -1) {
            printf("- %s (ID: %d)\n", rede->pessoas[posAmigo].nome, atual->idAmigo);
            contador++;
        }
        atual = atual->proximo;
    }
    
    if (contador == 0) {
        printf("(Nenhum amigo encontrado)\n");
    } else {
        printf("Total: %d amigo(s)\n", contador);
    }
}

// Função BFS para encontrar caminho entre duas pessoas
int buscaLargura(RedeSocial* rede, int idOrigem, int idDestino) {
    if (idOrigem == idDestino) return 0;
    
    int visitado[MAX_PESSOAS] = {0};
    int distancia[MAX_PESSOAS];
    Fila fila;
    iniciarFila(&fila);
    
    // Inicializar distâncias
    for (int i = 0; i < MAX_PESSOAS; i++) {
        distancia[i] = -1;
    }
    
    int posOrigem = encontrarPessoa(rede, idOrigem);
    if (posOrigem == -1) return -1;
    
    visitado[posOrigem] = 1;
    distancia[posOrigem] = 0;
    enfileirar(&fila, posOrigem);
    
    while (!filaVazia(&fila)) {
        int posAtual = desenfileirar(&fila);
        int idAtual = rede->pessoas[posAtual].id;
        
        if (idAtual == idDestino) {
            return distancia[posAtual];
        }
        
        NoAmizade* amigo = rede->pessoas[posAtual].amigos;
        while (amigo != NULL) {
            int posAmigo = encontrarPessoa(rede, amigo->idAmigo);
            if (posAmigo != -1 && !visitado[posAmigo]) {
                visitado[posAmigo] = 1;
                distancia[posAmigo] = distancia[posAtual] + 1;
                enfileirar(&fila, posAmigo);
            }
            amigo = amigo->proximo;
        }
    }
    
    return -1; // Não há caminho
}

// Função DFS para explorar a rede
void buscaProfundidade(RedeSocial* rede, int pos, int visitado[], int* contador) {
    visitado[pos] = 1;
    (*contador)++;
    printf("- %s (ID: %d)\n", rede->pessoas[pos].nome, rede->pessoas[pos].id);
    
    NoAmizade* amigo = rede->pessoas[pos].amigos;
    while (amigo != NULL) {
        int posAmigo = encontrarPessoa(rede, amigo->idAmigo);
        if (posAmigo != -1 && !visitado[posAmigo]) {
            buscaProfundidade(rede, posAmigo, visitado, contador);
        }
        amigo = amigo->proximo;
    }
}

// Função para encontrar componentes conectados
void encontrarComponentes(RedeSocial* rede) {
    int visitado[MAX_PESSOAS] = {0};
    int componente = 1;
    
    printf("\n=== COMPONENTES CONECTADOS DA REDE ===\n");
    
    for (int i = 0; i < MAX_PESSOAS; i++) {
        if (rede->pessoas[i].ativo && !visitado[i]) {
            printf("\nComponente %d:\n", componente);
            int contador = 0;
            buscaProfundidade(rede, i, visitado, &contador);
            printf("Total de pessoas: %d\n", contador);
            componente++;
        }
    }
}

// Função para listar todas as pessoas
void listarTodasPessoas(RedeSocial* rede) {
    printf("\n=== TODAS AS PESSOAS NA REDE ===\n");
    for (int i = 0; i < MAX_PESSOAS; i++) {
        if (rede->pessoas[i].ativo) {
            printf("ID: %d - Nome: %s\n", rede->pessoas[i].id, rede->pessoas[i].nome);
        }
    }
    printf("Total: %d pessoa(s)\n", rede->totalPessoas);
}

// Função para encontrar amigos em comum
void amigosEmComum(RedeSocial* rede, int id1, int id2) {
    int pos1 = encontrarPessoa(rede, id1);
    int pos2 = encontrarPessoa(rede, id2);
    
    if (pos1 == -1 || pos2 == -1) {
        printf("Erro: Uma ou ambas as pessoas não existem!\n");
        return;
    }
    
    printf("\nAmigos em comum entre %s e %s:\n", 
           rede->pessoas[pos1].nome, rede->pessoas[pos2].nome);
    
    int contador = 0;
    NoAmizade* amigo1 = rede->pessoas[pos1].amigos;
    
    while (amigo1 != NULL) {
        NoAmizade* amigo2 = rede->pessoas[pos2].amigos;
        while (amigo2 != NULL) {
            if (amigo1->idAmigo == amigo2->idAmigo) {
                int posComum = encontrarPessoa(rede, amigo1->idAmigo);
                if (posComum != -1) {
                    printf("- %s (ID: %d)\n", rede->pessoas[posComum].nome, amigo1->idAmigo);
                    contador++;
                }
                break;
            }
            amigo2 = amigo2->proximo;
        }
        amigo1 = amigo1->proximo;
    }
    
    if (contador == 0) {
        printf("(Nenhum amigo em comum)\n");
    } else {
        printf("Total: %d amigo(s) em comum\n", contador);
    }
}

// Função para carregar dados de exemplo
void carregarDadosExemplo(RedeSocial* rede) {
    printf("Carregando dados de exemplo...\n");
    
    // Adicionar pessoas
    adicionarPessoa(rede, 1, "Alice");
    adicionarPessoa(rede, 2, "Bob");
    adicionarPessoa(rede, 3, "Carol");
    adicionarPessoa(rede, 4, "David");
    adicionarPessoa(rede, 5, "Eva");
    adicionarPessoa(rede, 6, "Frank");
    
    // Adicionar amizades
    adicionarAmizade(rede, 1, 2);  // Alice - Bob
    adicionarAmizade(rede, 1, 3);  // Alice - Carol
    adicionarAmizade(rede, 2, 4);  // Bob - David
    adicionarAmizade(rede, 3, 4);  // Carol - David
    adicionarAmizade(rede, 4, 5);  // David - Eva
    // Frank fica isolado propositalmente
    
    printf("\nDados de exemplo carregados!\n");
}

// Função principal com menu
int main() {
    RedeSocial rede;
    iniciarRedeSocial(&rede);
    
    int opcao, id1, id2, distancia;
    char nome[MAX_NOME];
    
    printf("=== SIMULADOR DE REDE SOCIAL - GRAFO ===\n");
    printf("Estrutura de dados: Grafo com Lista de Adjacência\n\n");
    
    do {
        printf("\n=== MENU ===\n");
        printf("1. Adicionar pessoa\n");
        printf("2. Adicionar amizade\n");
        printf("3. Listar amigos de uma pessoa\n");
        printf("4. Listar todas as pessoas\n");
        printf("5. Encontrar distância entre pessoas\n");
        printf("6. Encontrar amigos em comum\n");
        printf("7. Mostrar componentes conectados\n");
        printf("8. Carregar dados de exemplo\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                printf("Digite o ID da pessoa: ");
                scanf("%d", &id1);
                printf("Digite o nome da pessoa: ");
                scanf(" %[^\n]", nome);
                adicionarPessoa(&rede, id1, nome);
                break;
                
            case 2:
                printf("Digite o ID da primeira pessoa: ");
                scanf("%d", &id1);
                printf("Digite o ID da segunda pessoa: ");
                scanf("%d", &id2);
                adicionarAmizade(&rede, id1, id2);
                break;
                
            case 3:
                printf("Digite o ID da pessoa: ");
                scanf("%d", &id1);
                listarAmigos(&rede, id1);
                break;
                
            case 4:
                listarTodasPessoas(&rede);
                break;
                
            case 5:
                printf("Digite o ID da pessoa de origem: ");
                scanf("%d", &id1);
                printf("Digite o ID da pessoa de destino: ");
                scanf("%d", &id2);
                distancia = buscaLargura(&rede, id1, id2);
                if (distancia == -1) {
                    printf("Não há conexão entre as pessoas!\n");
                } else if (distancia == 0) {
                    printf("São a mesma pessoa!\n");
                } else {
                    printf("Distância: %d grau(s) de separação\n", distancia);
                }
                break;
                
            case 6:
                printf("Digite o ID da primeira pessoa: ");
                scanf("%d", &id1);
                printf("Digite o ID da segunda pessoa: ");
                scanf("%d", &id2);
                amigosEmComum(&rede, id1, id2);
                break;
                
            case 7:
                encontrarComponentes(&rede);
                break;
                
            case 8:
                carregarDadosExemplo(&rede);
                break;
                
            case 0:
                printf("Saindo do simulador...\n");
                break;
                
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
    
    // Liberar memória das listas de adjacência
    for (int i = 0; i < MAX_PESSOAS; i++) {
        if (rede.pessoas[i].ativo) {
            NoAmizade* atual = rede.pessoas[i].amigos;
            while (atual != NULL) {
                NoAmizade* temp = atual;
                atual = atual->proximo;
                free(temp);
            }
        }
    }
    
    return 0;
}