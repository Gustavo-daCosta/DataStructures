# Micro Kernel - Teste de Digitação

Este projeto implementa um micro kernel básico de sistema operacional com um jogo interativo de teste de digitação.

## Características Implementadas

### Componentes do Kernel
- **Bootloader em Assembly**: Implementa o multiboot header e inicializa o kernel
- **Kernel em C**: Loop principal do sistema com gerenciamento de vídeo e interrupções
- **Sistema de Interrupções**: Manipulação das interrupções do teclado via IDT (Interrupt Descriptor Table)
- **Driver de Teclado**: Mapeamento completo das teclas com suporte a input do usuário

### Funcionalidades do Sistema
- **Exibição em Tela**: Sistema de impressão de texto colorido no modo VGA
- **Captura de Teclado**: Leitura em tempo real das teclas digitadas
- **Jogo Interativo**: Teste de digitação com medição de performance

### O Jogo - Teste de Digitação
O jogo apresenta frases aleatórias que o usuário deve digitar corretamente:

- **6 frases diferentes** para variedade
- **Feedback visual em tempo real**: 
  - Verde para caracteres corretos
  - Vermelho para caracteres incorretos
- **Métricas de performance**:
  - Palavras por minuto (PPM/WPM)
  - Precisão percentual
  - Tempo total
  - Número de palavras digitadas

### Controles
- **'s'**: Iniciar/reiniciar o jogo
- **Enter**: Submeter a frase digitada
- **ESC**: Sair do jogo ou encerrar o sistema
- **Backspace**: Apagar caracteres

## Como Executar

### Pré-requisitos
- `gcc` (compilador C)
- `nasm` (assembler)
- `ld` (linker)
- `qemu-system-i386` (emulador)
- `make` (sistema de build)

### Compilação e Execução

1. **Compilar o kernel**:
```bash
make
```

2. **Executar no QEMU**:
```bash
make run
```

3. **Limpar arquivos de compilação**:
```bash
make clean
```

### Uso do SistemaCom base ness
1. Ao iniciar, o sistema mostra o menu principal
2. Pressione **'s'** para iniciar o teste de digitação
3. Digite a frase exibida exatamente como mostrada
4. Pressione **Enter** para submeter
5. Veja suas estatísticas de performance
6. Pressione **'s'** para jogar novamente ou **ESC** para sair

## Estrutura do Projeto

```
├── kernel.asm      # Bootloader e funções de baixo nível
├── kernel.c        # Kernel principal e lógica do jogo
├── keyboard.c      # Mapeamento das teclas
├── keyboard.h      # Headers do teclado
├── link.ld         # Script de linking
├── Makefile        # Sistema de build
└── README.md       # Documentação
```

## Implementação Técnica

- **Modo de operação**: 32-bit protected mode
- **Padrão de boot**: Multiboot
- **Gerenciamento de memória**: Acesso direto ao buffer de vídeo (0xb8000)
- **Sistema de cores**: Atributos VGA (verde=0x0A, vermelho=0x0C, branco=0x07)
- **Gerador de números aleatórios**: Implementação própria para seleção de frases
