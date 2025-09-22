#include "keyboard.h"

#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

#define ENTER_KEY_CODE 0x1C

#define MAX_INPUT_LENGTH 100
#define NUM_PHRASES 6

static char* test_phrases[NUM_PHRASES] = {
    "ai que meu nome e nicola nao sei o que nao sei o que la",
    "eu ser murilove",
    "muito trampo fazer microkernel",
    "hello world",
    "ola mundo",
	"vou trancar a faculdade"
};

typedef struct {
    char input_buffer[MAX_INPUT_LENGTH];
    char* current_phrase;
    int buffer_pos;
    int phrase_index;
    int correct_chars;
    int total_chars;
    int mistakes;
    int words_typed;
    int start_time;
    int end_time;
    int game_active;
    int phrase_completed;
} typing_game_t;

static typing_game_t game;
static volatile int timer_counter = 0;
static int game_mode = 0; // 0 = normal mode, 1 = typing game mode

extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

// Declaração das funções
void idt_init(void);
void keyboard_init(void);
void print(const char *str);
void print_n(void);
void clear_screen(void);
void keyboard_handler_main(void);
void handle_typing_input(char keycode);
void check_phrase(void);
void init_typing_game(void);
unsigned int simple_rand(void);
int str_len(const char* str);
int str_comparison(const char* str1, const char* str2, int length);
int count_words(const char* str);
int calculate_wpm(int words, int time_seconds);
void int_to_string(int num, char* str);

unsigned int current_loc = 0;
char *vidptr = (char*)0xb8000;

struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];

// Seed para gerar valores aleatórios
static unsigned int rand_seed = 12345;

// Função para gerar valores aleatórios
unsigned int simple_rand() {
    rand_seed = rand_seed * 1103515245 + 12345;
    rand_seed ^= timer_counter;
    return (rand_seed / 65536) % 32768;
}

/* Pega o tamanho da string */
int str_len(const char* str) {
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

/* Compara duas string */
int str_comparison(const char* str1, const char* str2, int length) {
    for (int i = 0; i < length; i++) {
        if (str1[i] != str2[i]) return 0;
    }
    return 1;
}

/* Conta a quantidade de palavras em uma string */
int count_words(const char* str) {
    int words = 0;
    int in_word = 0;
    int i = 0;
    
    while (str[i] != '\0') {
        if (str[i] != ' ' && !in_word) {
            words++;
            in_word = 1;
        } else if (str[i] == ' ') {
            in_word = 0;
        }
        i++;
    }
    return words;
}

/* Calcula palavras por minuto */
int calculate_wpm(int words, int time_seconds) {
    if (time_seconds == 0) return 0;
    return (words * 60) / time_seconds;
}

/* Integer to string conversion */
void int_to_string(int num, char* str) {
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    int i = 0;
    int temp = num;
    
    while (temp > 0) {
        temp /= 10;
        i++;
    }
    
    str[i] = '\0';
    
    while (num > 0) {
        str[--i] = (num % 10) + '0';
        num /= 10;
    }
}

void init_typing_game() {
    clear_screen();
    current_loc = 0;
    
    print("=== TESTE DE DIGITACAO ===");
    print_n();
    print_n();
    
    /* Instructions */
    print("Instrucões:");
    print_n();
    print("- Escreva a frase exatamente como ela é mostrada");
    print_n();
    print("- Pressione ENTER quando estiver pronto");
    print_n();
    print("- Pressione ESC para sair do jogo");
    print_n();
    print_n();
    
    /* Initialize game state */
    game.buffer_pos = 0;
    
    rand_seed += timer_counter + current_loc;
    game.phrase_index = simple_rand() % NUM_PHRASES;
    game.current_phrase = test_phrases[game.phrase_index];
    
    game.correct_chars = 0;
    game.total_chars = 0;
    game.mistakes = 0;
    game.words_typed = 0;
    game.start_time = timer_counter;
    game.game_active = 1;
    game.phrase_completed = 0;
    
    for (int i = 0; i < MAX_INPUT_LENGTH; i++) {
        game.input_buffer[i] = '\0';
    }
    
    /* Mostra a frase */
    print("Digite esta frase:");
    print_n();
    print(game.current_phrase);
    print_n();
    print_n();
    print("Sua entrada: ");
}

// Inicia o IDT
void idt_init(void)
{
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	/* populate IDT entry of keyboard's interrupt */
	keyboard_address = (unsigned long)keyboard_handler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = INTERRUPT_GATE;
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	write_port(0x20 , 0x11);
	write_port(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	write_port(0x21 , 0x20);
	write_port(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	write_port(0x21 , 0x00);
	write_port(0xA1 , 0x00);

	/* ICW4 - environment info */
	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);

	/* mask interrupts */
	write_port(0x21 , 0xff);
	write_port(0xA1 , 0xff);

	/* fill the IDT descriptor */
	idt_address = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
}

void keyboard_init(void)
{
	write_port(0x21 , 0xFD);
}

void print(const char *str)
{
	unsigned int i = 0;
	while (str[i] != '\0') {
		vidptr[current_loc++] = str[i++];
		vidptr[current_loc++] = 0x07;
	}
}

// Print com nova linha 
void print_n(void)
{
	unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
	current_loc = current_loc + (line_size - current_loc % (line_size));
}

// Limpa a tela
void clear_screen(void)
{
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x07;
	}
}

void keyboard_handler_main(void)
{
	unsigned char status;
	unsigned char keycode;

	/* write EOI */
	write_port(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);
	if (status & 0x01) {
		keycode = read_port(KEYBOARD_DATA_PORT);
		if(keycode >= 128)
			return;

		/* Modo de jogo vs modo normal */
		if (game_mode == 1) {
			handle_typing_input(keycode);
		} else {
			if(keycode == ENTER_KEY_CODE) {
				print_n();
				return;
			}
			
			/* Se o usuário quer começar/recomeçar o jogo */
			if(keyboard_map[keycode] == 's') { // Tecla s
				game_mode = 1;
				init_typing_game();
				return;
			}
			
			/* Se o usuário quiser sair, pressionando ESC */
			if(keycode == 1) { /* Tecla ESC */
				clear_screen();
				current_loc = 0;
				print("Sistema encerrado. Obrigado por usar!");
				print_n();
				while(1) {
					asm("hlt");
				}
			}
			
			char c = keyboard_map[keycode];
			if (c != 0) {
				vidptr[current_loc++] = c;
				vidptr[current_loc++] = 0x07;
			}
		}
	}
}

void handle_typing_input(char keycode) {
	timer_counter++;
	
	char key = keyboard_map[(unsigned char) keycode];
	
	if (keycode == 1) {
		game_mode = 0;
		clear_screen();
		current_loc = 0;
		print("Voltando ao menu principal...");
		print_n();
		print("Pressione 's' para jogar novamente ou ESC para sair.");
		print_n();
		return;
	}
	
	/* Se o jogo não está ativo, permitir reiniciar com 's' */
	if (!game.game_active || game.phrase_completed) {
		if (keyboard_map[(unsigned char) keycode] == 's') {
			init_typing_game();
			return;
		}
		return;
	}
	
	if (keycode == ENTER_KEY_CODE) {
		game.input_buffer[game.buffer_pos] = '\0';
		check_phrase();
		return;
	}
	
	if (keycode == 14) { // 14 = Backspace
		if (game.buffer_pos > 0) {
			game.buffer_pos--;
			game.input_buffer[game.buffer_pos] = '\0';
			if (current_loc > 0) {
				current_loc -= 2;
				vidptr[current_loc] = ' ';
				vidptr[current_loc + 1] = 0x07;
			}
		}
		return;
	}
	
	/* Input do teclado */
	if (game.buffer_pos < MAX_INPUT_LENGTH - 1 && key >= 32 && key <= 126) {
		game.input_buffer[game.buffer_pos] = key;
		game.total_chars++;
		
		// Verifica se o input está correto e pinta os caracteres conforme o resultado
		if (game.buffer_pos < str_len(game.current_phrase) && 
		    key == game.current_phrase[game.buffer_pos]) {
			game.correct_chars++;
			vidptr[current_loc++] = key;
			vidptr[current_loc++] = 0x0A; // Verde = correto
		} else {
			vidptr[current_loc++] = key;
			vidptr[current_loc++] = 0x0C; // Vermelho = incorreto
		}
		
		game.buffer_pos++;
	}
}

// Verifica se a frase está correta
void check_phrase() {
	int phrase_length = str_len(game.current_phrase);
	
	if (game.buffer_pos == phrase_length && 
	    str_comparison(game.input_buffer, game.current_phrase, phrase_length)) {
		
		/* Caso a submissão esteja correta */
		game.end_time = timer_counter;
		game.phrase_completed = 1;
		game.words_typed = count_words(game.current_phrase);
		
		print_n();
		print_n();
		print("*** CORRETO! ***");
		print_n();
		print_n();
		
		/* Faz o cálculo das métricas e mostra os resultados */
		int time_elapsed = (game.end_time - game.start_time) / 10;
		if (time_elapsed == 0) time_elapsed = 1;
		
		int wpm = calculate_wpm(game.words_typed, time_elapsed);
		int accuracy = (game.correct_chars * 100) / (game.total_chars > 0 ? game.total_chars : 1);
		
		print("Resultados:");
		print_n();
		
		print("Palavras escritas: ");
		char num_str[10];
		int_to_string(game.words_typed, num_str);
		print(num_str);
		print_n();
		
		print("Tempo: ~");
		int_to_string(time_elapsed, num_str);
		print(num_str);
		print(" segundos");
		print_n();
		
		print("Velocidade: ~");
		int_to_string(wpm, num_str);
		print(num_str);
		print("PPM - Palavras por minuto");
		print_n();
		
		print("Acurácia: ");
		int_to_string(accuracy, num_str);
		print(num_str);
		print("%");
		print_n();
		print_n();
		
		print("Pressione 's' para jogar novamente, ou ESC para sair.");
		print_n();
		
		game.game_active = 0;
		
	} else {
		/* Submissão incorreta */
		print_n();
		print("Tente novamente! Pressione 's' para recomecar.");
		print_n();
		game.game_active = 0;
	}
}

void kmain(void)
{
	const char *str = "Game de Teste de Digitacao";
	clear_screen();
	print(str);
	print_n();
	print("==================================");
	print_n();
	print_n();
	
	print("Instrucoes:");
	print_n();
	print("- Pressione 's' para iniciar o jogo");
	print_n();
	print("- Digite normalmente para testar o teclado");
	print_n();
	print_n();
	
	print("Inicie: ");

	idt_init();
	keyboard_init();

	while(1);
}