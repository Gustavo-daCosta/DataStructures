#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSOS 100

// Estrutura para representar um processo
typedef struct {
    int id;
    int tempoExecucao;
    char nome[50];
} Processo;

// Estrutura da Fila
typedef struct {
    Processo processos[MAX_PROCESSOS];
    int inicio;
    int fim;
    int tamanho;
} FilaProcessos;

// Função para inicializar a fila
void iniciarFila(FilaProcessos* fila) {
    fila->inicio = 0;
    fila->fim = -1;
    fila->tamanho = 0;
}

// Função para verificar se a fila está vazia
int filaVazia(FilaProcessos* fila) {
    return fila->tamanho == 0;
}

// Função para verificar se a fila está cheia
int filaCheia(FilaProcessos* fila) {
    return fila->tamanho == MAX_PROCESSOS;
}

// Função para enfileirar processo
int enfileirar(FilaProcessos* fila, int id, char* nome, int tempoExecucao) {
    if (filaCheia(fila)) {
        printf("Erro: Fila de processos cheia! Não é possível adicionar mais processos.\n");
        return 0;
    }
    
    fila->fim = (fila->fim + 1) % MAX_PROCESSOS;
    fila->processos[fila->fim].id = id;
    strcpy(fila->processos[fila->fim].nome, nome);
    fila->processos[fila->fim].tempoExecucao = tempoExecucao;
    fila->tamanho++;
    
    printf("✓ Processo adicionado: ID=%d, Nome=%s, Tempo=%d unidades\n", 
           id, nome, tempoExecucao);
    return 1;
}

// Função para desenfileirar processo
Processo desenfileirar(FilaProcessos* fila) {
    Processo processoVazio = {-1, 0, ""};
    
    if (filaVazia(fila)) {
        printf("Erro: Fila vazia! Nenhum processo para executar.\n");
        return processoVazio;
    }
    
    Processo processo = fila->processos[fila->inicio];
    fila->inicio = (fila->inicio + 1) % MAX_PROCESSOS;
    fila->tamanho--;
    
    return processo;
}

// Função para visualizar o próximo processo
Processo proximoProcesso(FilaProcessos* fila) {
    Processo processoVazio = {-1, 0, ""};
    
    if (filaVazia(fila)) {
        return processoVazio;
    }
    
    return fila->processos[fila->inicio];
}

// Função para executar próximo processo
void executarProximoProcesso(FilaProcessos* fila) {
    if (filaVazia(fila)) {
        printf("Nenhum processo na fila para executar.\n");
        return;
    }
    
    Processo processo = desenfileirar(fila);
    printf("⚡ Executando processo: ID=%d, Nome=%s, Tempo=%d unidades\n", 
           processo.id, processo.nome, processo.tempoExecucao);
    printf("   Processo concluído!\n");
}

// Função para simular execução de todos os processos
void executarTodosProcessos(FilaProcessos* fila) {
    if (filaVazia(fila)) {
        printf("Nenhum processo na fila para executar.\n");
        return;
    }
    
    printf("\n=== SIMULANDO EXECUÇÃO DE TODOS OS PROCESSOS ===\n");
    int tempoTotal = 0;
    int processosExecutados = 0;
    
    while (!filaVazia(fila)) {
        Processo processo = desenfileirar(fila);
        tempoTotal += processo.tempoExecucao;
        processosExecutados++;
        
        printf("⚡ [Tempo %d] Executando: ID=%d, Nome=%s (%d unidades)\n", 
               tempoTotal - processo.tempoExecucao, processo.id, processo.nome, processo.tempoExecucao);
    }
    
    printf("\n✓ Simulação concluída!\n");
    printf("  Processos executados: %d\n", processosExecutados);
    printf("  Tempo total: %d unidades\n", tempoTotal);
}

// Função para exibir status da fila
void exibirStatusFila(FilaProcessos* fila) {
    printf("\n=== STATUS DA FILA ===\n");
    printf("Processos na fila: %d/%d\n", fila->tamanho, MAX_PROCESSOS);
    
    if (filaVazia(fila)) {
        printf("Fila vazia.\n");
        return;
    }
    
    printf("Próximo processo: ID=%d, Nome=%s, Tempo=%d\n", 
           fila->processos[fila->inicio].id, 
           fila->processos[fila->inicio].nome,
           fila->processos[fila->inicio].tempoExecucao);
    
    printf("\nTodos os processos na fila:\n");
    for (int i = 0; i < fila->tamanho; i++) {
        int indice = (fila->inicio + i) % MAX_PROCESSOS;
        printf("  %d. ID=%d, Nome=%s, Tempo=%d\n", 
               i + 1,
               fila->processos[indice].id,
               fila->processos[indice].nome,
               fila->processos[indice].tempoExecucao);
    }
}

// Função para exibir menu
void exibirMenu() {
    printf("\n=== SIMULADOR DE FILA DE PROCESSOS ===\n");
    printf("1. Adicionar processo\n");
    printf("2. Executar próximo processo\n");
    printf("3. Ver próximo processo\n");
    printf("4. Executar todos os processos\n");
    printf("5. Exibir status da fila\n");
    printf("6. Carregar processos de exemplo\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

// Função para carregar processos de exemplo
void carregarExemplos(FilaProcessos* fila) {
    printf("Carregando processos de exemplo...\n");
    
    enfileirar(fila, 101, "Navegador", 5);
    enfileirar(fila, 102, "Editor", 3);
    enfileirar(fila, 103, "Compilador", 8);
    enfileirar(fila, 104, "Antivirus", 2);
    enfileirar(fila, 105, "Backup", 10);
    
    printf("5 processos de exemplo adicionados à fila!\n");
}

int main() {
    FilaProcessos fila;
    iniciarFila(&fila);
    
    int opcao;
    int id, tempo;
    char nome[50];
    
    printf("Bem-vindo ao Simulador de Fila de Processos!\n");
    printf("Este sistema simula o escalonamento FIFO de processos.\n");
    
    while (1) {
        exibirMenu();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                printf("ID do processo: ");
                scanf("%d", &id);
                printf("Nome do processo: ");
                scanf("%s", nome);
                printf("Tempo de execução: ");
                scanf("%d", &tempo);
                
                if (tempo <= 0) {
                    printf("Erro: Tempo de execução deve ser positivo!\n");
                } else {
                    enfileirar(&fila, id, nome, tempo);
                }
                break;
                
            case 2:
                executarProximoProcesso(&fila);
                break;
                
            case 3: {
                Processo proximo = proximoProcesso(&fila);
                if (proximo.id != -1) {
                    printf("Próximo processo: ID=%d, Nome=%s, Tempo=%d\n", 
                           proximo.id, proximo.nome, proximo.tempoExecucao);
                } else {
                    printf("Nenhum processo na fila.\n");
                }
                break;
            }
            
            case 4:
                executarTodosProcessos(&fila);
                break;
                
            case 5:
                exibirStatusFila(&fila);
                break;
                
            case 6:
                carregarExemplos(&fila);
                break;
                
            case 0:
                printf("Encerrando simulador...\n");
                if (!filaVazia(&fila)) {
                    printf("Aviso: %d processo(s) ainda na fila.\n", fila.tamanho);
                }
                printf("Simulador finalizado.\n");
                return 0;
                
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }
    
    return 0;
}