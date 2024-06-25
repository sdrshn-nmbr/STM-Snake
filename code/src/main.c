#include "stm32f0xx.h" // Include STM32F0xx peripheral library
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcd.h"
#include "snake.h"
#include "clock.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
#define N 1000

int step0 = 0;
int offset0 = 0;
int step1 = 0;
int offset1 = 0;

short int wavetable[N];
const char *keymap_arr = "DCBA#9630852*741";
Snake *snake;
Direction snake_direction = UP; // snake direction
Direction nxt_snake_direction = UP;
uint8_t col = 0;                // current column
int GROW_SNAKE = 0;             // 0 for no growth, 1 for growth
int * food;
bool food_exists = false;
int count = 2;
int speed = 5999;

void enable_ports()
{
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  GPIOC->MODER &= ~0x000000ff; // clear bits 0-7
  GPIOC->MODER |= 0x00005500;  // set bits 8,10,12,14. ports 4-7 as output
  GPIOC->PUPDR &= ~0x000000ff; // set bits 2,4,6,8
  GPIOC->PUPDR |= 0x000000aa;  // set bits 2,4,6,8
}
void drive_column(int c)
{
  c = c & 0x3;
  GPIOC->BSRR |= 0xf << 20;
  GPIOC->BSRR |= 0x1 << (c + 4);
}
int read_rows()
{
  return (GPIOC->IDR & 0xf);
}
char rows_to_key(int rows)
{
  for (int i = 0; i < 4; i++)
  {
    if ((rows & (1 << i)) == 1 << i)
    {
      char c = keymap_arr[(col & 0x3) * 4 + (i)];
      return c;
    }
  }
  return 0;
}

void handle_key(char key)
{
  if (key == '2')
  {
    if (snake_direction == DOWN)
      return;
    nxt_snake_direction = UP;
  }
  else if (key == '4')
  {
    if (snake_direction == RIGHT)
      return;
    nxt_snake_direction = LEFT;
  }
  else if (key == '5')
  {
    if (snake_direction == UP)
      return;
    nxt_snake_direction = DOWN;
  }
  else if (key == '6')
  {
    if (snake_direction == LEFT)
      return;
    nxt_snake_direction = RIGHT;
  }
  else if (key == '9')
  {
    LCD_Clear(GREEN);
    snake = initSnake(16, 12);
    food_exists = false;
  }
}

void render(Snake *snake)
{
  while (snake->next != NULL)
  {
    LCD_DrawFillRectangle(snake->x * 10, snake->y * 10, snake->x * 10 + 10, snake->y * 10 + 10, BLACK);
    snake = snake->next;
  }
  LCD_DrawFillRectangle(snake->x * 10, snake->y * 10, snake->x * 10 + 10, snake->y * 10 + 10, GREEN);
}

void TIM7_IRQHandler()
{
  TIM7->SR &= ~TIM_SR_UIF;
  int rows = GPIOC->IDR;
  if (rows != 0)
  {
    char key = rows_to_key(rows);
    handle_key(key);
  }
  col = col + 1;
  if (col > 7)
  {
    col = 0;
  }
  drive_column(col);
}

void setup_dac(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    GPIOA->MODER |= GPIO_MODER_MODER4;
    //output pin pa4
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;

    DAC->CR &= ~(DAC_CR_TSEL1);
    DAC->CR |= DAC_CR_TEN1;

    DAC->CR |= DAC_CR_EN1;
}

void generate_sound() {
    float frequency = 440.0; // Frequency of the sound (e.g., A440)
    int duration = 500000; // Duration in microseconds
    DAC->DHR12R1 = 1;
    for (int i = 0; i < duration; i++) {
        // Calculate sample value for sine wave at current time
        float sample = sin(2 * M_PI * frequency * i / 1000000.0); // Assuming a 1 MHz timer
        // Scale sample to fit DAC range (0 to 4095 for a 12-bit DAC)
        uint16_t dac_value = (sample + 1) * 2047; // Assuming 12-bit DAC

        // Write sample to DAC
        DAC->DHR12R1 = dac_value;

        // Wait for a short period to control sound duration
    }

    // After the sound is finished, turn off the DAC
    DAC->DHR12R1 = 0;
}

void init_tim6(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    TIM6->PSC = 48-1; 


    TIM6->ARR = (1000000 / 20000)-1; //should be larger than presc
    TIM6->DIER |= TIM_DIER_UIE;

    TIM6->CR2 |= TIM_CR2_MMS_1;

    TIM6->CR1 |= TIM_CR1_CEN;

    //interrupt
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
} // NEED A TIMER WITH DAC SOMETHING LIKE THIS (eat apple handler)


void reset_tim14()
{
  TIM14->CR1 &= ~TIM_CR1_CEN;
  TIM14->PSC = 999;
  TIM14->ARR = speed;
  TIM14->CR1 |= TIM_CR1_CEN;
}

void setup_tim7()
{
  RCC->APB1ENR |= 1 << 5; // bit 5 is tim7
  TIM7->PSC = 999;
  TIM7->ARR = 47;
  TIM7->DIER |= TIM_DIER_UIE;
  NVIC->ISER[0] = 1 << TIM7_IRQn; // bit 18
  TIM7->CR1 |= TIM_CR1_CEN;
}

int* generate_food()
{
  int x = rand() % 24;
  int y = rand() % 32;
  bool exists = true;
  while(exists) {
    exists = false;
    Snake * current = snake;
    while(current->next != NULL) {
      if(current->x == x && current->y == y) {
        x = rand() % 24;
        y = rand() % 32;
        exists = true;
        break;
      }
      current = current->next;
    }
  }
  food = (int*)malloc(sizeof(int) * 2);
  food[0] = x;
  food[1] = y;
  LCD_DrawFillRectangle(x * 10, y * 10, x * 10 + 10, y * 10 + 10, RED);
  return food;
}

void TIM6_DAC_IRQHandler() {
    TIM6->SR &= ~TIM_SR_UIF;
    ADC1->CR |= ADC_CR_ADSTART;
    offset0 += step0; // increment offset0 by step0
    offset1 += step1; // increment offset1 by step1
    if(offset0 >= (N<<16)){// if offset0 is >= (N << 16)
        offset0 -= (N <<16);//     decrement offset0 by (N << 16)
    } if(offset1 >=(N <<16)) {// if offset1 is >= (N << 16)
    offset1 -= (N <<16);//     decrement offset1 by (N << 16)
    }
    int samp = wavetable[offset0>>16] + wavetable[offset1>>16];// int samp = sum of wavetable[offset0>>16] and wavetable[offset1>>16]
    samp = samp * 2048;// multiply samp by volume
    samp = samp >> 17;// shift samp right by 17 bits to ensure it's in the right format for `DAC_DHR12R1` 
    samp += 2048;// increment samp by 2048
    DAC->DHR12R1 = samp;// copy samp to DAC->DHR12R1
}

void init_wavetable(void) {
    for(int i=0; i < N; i++)
        wavetable[i] = 32767 * sin(2 * M_PI * i / N);
}

void set_freq(int chan, float f) {
    if (chan == 0) {
        if (f == 0.0) {
            step0 = 0;
            offset0 = 0;
        } else
            step0 = (f * N / 20000) * (1<<16);
    }
    if (chan == 1) {
        if (f == 0.0) {
            step1 = 0;
            offset1 = 0;
        } else
            step1 = (f * N / 20000) * (1<<16);
    }
}

void TIM14_IRQHandler()
{
  TIM14->SR &= ~TIM_SR_UIF;
  if (snake == NULL)
  {
    LCD_Clear(RED);
    return;
  }
  snake_direction = nxt_snake_direction;
  if (!food_exists)
  {
    generate_food();
    food_exists = true;
  }

  if (snake->x == food[0] && snake->y == food[1])
  {
    count++;
    speed *= .9;
    reset_tim14();
    snake = moveSnake(snake, snake_direction, 1);
    generate_food();
    set_freq(0,speed/10);
  }
  else {
    snake = moveSnake(snake, snake_direction, 0);
  }
  if (snake == NULL)
  {
    LCD_Clear(RED);
    return;
  }
  render(snake);
}



void setup_tim14()
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
  TIM14->PSC = 999;
  TIM14->ARR = speed;
  TIM14->DIER |= TIM_DIER_UIE;
  NVIC->ISER[0] = 1 << TIM14_IRQn; // bit 19
  TIM14->CR1 |= TIM_CR1_CEN;
}

int main(void)
{
  internal_clock();
  snake = initSnake(16, 12);
  enable_ports();
  LCD_Setup();
  LCD_Init(lcddev.reset, lcddev.select, lcddev.reg_select);
  init_lcd_spi();
  LCD_Clear(GREEN);
  setup_tim7();
  // init_tim6();
  setup_tim14();
  // setup_dac();

  init_wavetable();
  setup_dac();
  init_tim6();
  set_freq(0,261.626);
  set_freq(1,329.628);
  
  for (;;)
  {
    asm("wfi");
  }
}
