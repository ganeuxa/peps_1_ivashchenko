/*
 * Практическая работа №1. Программа для МК семейства MCS-51 (DS80C320 / 8051)
 * к схеме Lab1_MCU.ms14.
 *
 * Подключение:
 *   S1  (кнопка на GND, подтяжка R1 10 кОм к VCC) -> P1.0
 *   LED1 (через R2 220 Ом, катод на GND)          -> P1.3
 *   U2  (7-сегментный индикатор с дешифратором)  -> P2.0..P2.3 (код BCD/HEX)
 *   LS1 (зуммер)                                  -> P3.7
 *
 * Логика: пока кнопка нажата — светодиод горит; каждое нажатие
 * увеличивает счётчик 0..F, который выводится на индикатор,
 * и сопровождается коротким звуковым сигналом.
 *
 * Компилятор: SDCC (sdcc prog1.c) или Keil C51 (заменить include на <reg51.h>).
 */
#include <8051.h>

#define BUTTON  P1_0   /* 0 — нажата, 1 — отпущена */
#define LED     P1_3   /* 1 — светодиод включён     */
#define BUZZER  P3_7   /* 1 — звук                  */

static void delay_ms(unsigned int ms)
{
    unsigned int i;
    while (ms--)
        for (i = 0; i < 120; i++)   /* ~1 мс при 11,0592 МГц */
            ;
}

static void beep(void)
{
    BUZZER = 1;
    delay_ms(50);
    BUZZER = 0;
}

void main(void)
{
    unsigned char counter = 0;
    unsigned char prev = 1;

    BUTTON = 1;                 /* порт на вход (квазидвунаправленный) */
    LED = 0;
    BUZZER = 0;
    P2 = (P2 & 0xF0) | counter; /* на индикаторе 0 */

    while (1)
    {
        unsigned char now = BUTTON;

        LED = (now == 0);       /* кнопка нажата -> LED включён */

        if (prev == 1 && now == 0)          /* фронт нажатия */
        {
            delay_ms(20);                   /* антидребезг */
            if (BUTTON == 0)
            {
                counter = (counter + 1) & 0x0F;
                P2 = (P2 & 0xF0) | counter; /* вывод на U2 */
                beep();
            }
        }
        prev = now;
    }
}
