// Bibliotecas do Pico
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"
// Importando os arquivos das pastas
#include "Funções/font.h"
#include "Funções/ssd1306.h"
#include "Funções/mudar_LED.c"
#include "Funções/numeros.h"
#include "Funções/cores.h"
#include "LED.pio.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define LED_VERDE 11
#define MATRIZ_LED 7
#define botão_A 5
#define botão_B 6
#define Buzzer 10
#define JOYSTICK_X 26 
#define dead_angle 100

// Prototipação das funções
static void gpio_irq_handler(uint gpio, uint32_t events);
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);
void play_note(int buzzer, int frequency, int duration);
void play_musica_atual();

// Variáveis globais
static float intensidade = 0.1;
static volatile uint cont_cor = 0;
static volatile uint32_t last_time = 0;
static volatile uint pos_quadrado[2]={61,29}; 
bool cor = true;
char c;
char buffer[1];
static volatile bool tocando = true; // Controla se a música está tocando ou pausada
volatile bool is_playing = false;  // Variável para verificar se a música está tocando
static volatile int musica_atual = 1; // Começa na música 1
int volume = 50;  // Volume inicial (0 a 100)
ssd1306_t ssd;  // Torna a variável ssd global


void play_love_story() {
  int melody[] = {392, 440, 392, 349, 392, 294, 330, 349};
  int noteDurations[] = {400, 400, 400, 800, 400, 400, 400, 800};
  gpio_put(LED_VERDE, 1); 
  for (int i = 0; i < 8; i++) {
      if (!tocando) break;
      play_note(Buzzer, melody[i], noteDurations[i]);
      sleep_ms(50);
  }
  gpio_put(LED_VERDE, 0); 
}

void play_super_mario() {
  int melody[] = {262, 262, 294, 330, 349, 330, 294, 262, 262, 262};
  int noteDurations[] = {400, 400, 400, 400, 400, 400, 400, 400, 400, 400};
  gpio_put(LED_VERDE, 1);
  for (int i = 0; i < 10; i++) {
      if (!tocando) break;
      play_note(Buzzer, melody[i], noteDurations[i]);
      sleep_ms(50);
  }
  gpio_put(LED_VERDE, 0); 
}

void play_another_brick_in_the_wall() {
  int melody[] = {349, 349, 349, 349, 392, 392, 392, 392, 440, 440};
  int noteDurations[] = {400, 400, 400, 400, 400, 400, 400, 400, 400, 400};
  gpio_put(LED_VERDE, 1);
  for (int i = 0; i < 10; i++) {
      if (!tocando) break;
      play_note(Buzzer, melody[i], noteDurations[i]);
      sleep_ms(50);
  }
  gpio_put(LED_VERDE, 0); 
}

void play_shake_it_off() {
  int melody[] = {523, 523, 587, 523, 659, 698, 523};
  int noteDurations[] = {400, 400, 400, 400, 400, 400, 800};
  gpio_put(LED_VERDE, 1);
  for (int i = 0; i < 7; i++) {
      if (!tocando) break;
      play_note(Buzzer, melody[i], noteDurations[i]);
      sleep_ms(50);
  }
  gpio_put(LED_VERDE, 0); 
}

void play_twinkle_twinkle() {
  int melody[] = {262, 262, 392, 392, 440, 440, 392, 349, 349, 330, 330, 294, 294, 262};
  int noteDurations[] = {400, 400, 400, 400, 400, 400, 800, 400, 400, 400, 400, 400, 400, 800};
  gpio_put(LED_VERDE, 1);
  for (int i = 0; i < 14; i++) {
      if (!tocando) break;
      play_note(Buzzer, melody[i], noteDurations[i]);
      sleep_ms(50);
  }
  gpio_put(LED_VERDE, 0); 
}


void play_bad_romance() {
  int melody[] = {523, 523, 523, 523, 440, 440, 349};
  int noteDurations[] = {400, 400, 400, 400, 400, 400, 800};
  gpio_put(LED_VERDE, 1);
  for (int i = 0; i < 7; i++) {
      if (!tocando) break;
      play_note(Buzzer, melody[i], noteDurations[i]);
      sleep_ms(50);
  }
  gpio_put(LED_VERDE, 0); 
}

void play_legend_of_zelda() {
  int melody[] = {523, 523, 523, 523, 392, 392, 440, 440};
  int noteDurations[] = {400, 400, 400, 400, 400, 400, 400, 400};
  gpio_put(LED_VERDE, 1);
  for (int i = 0; i < 8; i++) {
      if (!tocando) break;
      play_note(Buzzer, melody[i], noteDurations[i]);
      sleep_ms(50);
  }
  gpio_put(LED_VERDE, 0); 
}

void play_tetris() {
  int melody[] = {262, 294, 330, 349, 392, 440, 392};
  int noteDurations[] = {200, 200, 200, 200, 200, 200, 400};
  gpio_put(LED_VERDE, 1);
  for (int i = 0; i < 7; i++) {
      if (!tocando) break;
      play_note(Buzzer, melody[i], noteDurations[i]);
      sleep_ms(50);
  }
  gpio_put(LED_VERDE, 0); 
}

void play_mega_man() {
  int melody[] = {392, 392, 440, 440, 523, 523, 440, 392, 349, 349, 392};
  int noteDurations[] = {400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 800};
  gpio_put(LED_VERDE, 1);
  for (int i = 0; i < 11; i++) {
      if (!tocando) break;
      play_note(Buzzer, melody[i], noteDurations[i]);
      sleep_ms(50);
  }
  gpio_put(LED_VERDE, 0); 
}

void play_musica_atual() {
  if (!tocando) return;
  switch (musica_atual) {
      case 1: play_love_story(); break;
      case 2: play_super_mario(); break;
      case 3: play_another_brick_in_the_wall(); break;
      case 4: play_shake_it_off(); break;
      case 5: play_twinkle_twinkle(); break;
      case 6: play_bad_romance(); break;
      case 7: play_legend_of_zelda(); break;
      case 8: play_tetris(); break;
      case 9: play_mega_man(); break;
  } // Marca como tocando
}

int main() {
  static PIO pio = pio0; 
  uint32_t valor_led;

  stdio_init_all();

  uint offset = pio_add_program(pio, &pio_matrix_program);
  uint sm = pio_claim_unused_sm(pio, true);
  pio_matrix_program_init(pio, sm, offset, MATRIZ_LED);

  i2c_init(I2C_PORT, 400 * 1000);
  uart_init(UART_ID, BAUD_RATE);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA);
  gpio_pull_up(I2C_SCL);

  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
  ssd1306_config(&ssd);
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  adc_init();
  adc_gpio_init(JOYSTICK_X);  
  gpio_init(Buzzer);
  gpio_set_dir(Buzzer, GPIO_OUT);
  gpio_init(LED_VERDE);
  gpio_set_dir(LED_VERDE, GPIO_OUT);
  gpio_init(botão_A);
  gpio_set_dir(botão_A, GPIO_IN);
  gpio_pull_up(botão_A);
  gpio_init(botão_B);
  gpio_set_dir(botão_B, GPIO_IN);
  gpio_pull_up(botão_B);

  gpio_set_irq_enabled_with_callback(botão_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  gpio_set_irq_enabled_with_callback(botão_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

  while (true) 
  { 
      c = getchar();
      if (c != '\n') {
        buffer[0] = c;
      }
      if (buffer[0] >= '0' && buffer[0] <= '9') {
        desenho_pio(numeros[buffer[0] - 48], valor_led, pio, sm, 
                    intensidade * lista_de_cores[cont_cor][0], 
                    intensidade * lista_de_cores[cont_cor][1], 
                    intensidade * lista_de_cores[cont_cor][2]);
      } 
      adc_select_input(1);  // Seleciona o pino correto do ADC
      uint16_t raw_value = adc_read();  // Lê o valor do ADC
      printf("%u\n", raw_value);  // Imprime o valor do ADC
      volume = (raw_value * 100) / 4095;  
      if (abs(raw_value - 2047) > dead_angle) {
        pos_quadrado[0] = 61 + ((raw_value - 2047) / 2047.0) * 58;
      } else {
        pos_quadrado[0] = 61;
      }
      ssd1306_fill(&ssd, cor);
      ssd1306_rect(&ssd, 3, 3, 122, 58, !cor, cor);
      ssd1306_draw_string(&ssd,"0",pos_quadrado[0],pos_quadrado[1]);// Desenha o quadrado
      ssd1306_draw_string(&ssd, "Digite algo:", 8, 10);
      ssd1306_draw_string(&ssd, buffer, 50, 20);
      ssd1306_send_data(&ssd);
      if (c != '\n' && c >= '1' && c <= '9') {  // Verifica se o caractere digitado é um número
        musica_atual = c - '0';  // Converte o caractere para o número correspondente
        tocando = true;  // Garante que a música recomece
        play_musica_atual();  // Toca a música
      }

    sleep_ms(100);
  }
}

void gpio_irq_handler(uint gpio, uint32_t events) {
  uint32_t current_time = to_us_since_boot(get_absolute_time());// Obtém o tempo atual em microssegundos
  if (current_time - last_time > 100000) { // Verifica se passou tempo suficiente desde o último evento com 200 ms de debouncing 
    last_time = current_time; // Atualiza o tempo do último evento
    if (gpio == botão_A) {
      tocando = !tocando; // Alterna entre tocar e pausar
    }
    else if (gpio == botão_B) {
    cont_cor++;
    if (cont_cor == 6) {
      cont_cor = 0;
    }
  }
}
}

// Função play_note modificada para usar o PWM
void play_note(int buzzer, int frequency, int duration) {
  if (frequency == 0) {
      sleep_ms(duration);
      return;
  }

  gpio_set_function(buzzer, GPIO_FUNC_PWM);
  uint slice = pwm_gpio_to_slice_num(buzzer);

  uint32_t clock = clock_get_hz(clk_sys);
  uint32_t divider = 16;
  uint32_t wrap = clock / (divider * frequency);

  pwm_set_clkdiv(slice, divider);
  pwm_set_wrap(slice, wrap);

  // Ajusta a intensidade do volume
  uint32_t level = (wrap * volume) / 100;  // Volume ajustado
  pwm_set_gpio_level(buzzer, level);

  pwm_set_enabled(slice, true);
  sleep_ms(duration);
  pwm_set_enabled(slice, false);

  gpio_set_function(buzzer, GPIO_FUNC_SIO);
  gpio_set_dir(buzzer, GPIO_OUT);
  gpio_put(buzzer, 0);
}

