#include "mbed.h"

Ticker tick1, tick2;
DigitalOut led1(LED2); // used pin in brackets (závorka)
DigitalOut led2(LED4); // used pin in brackets (závorka)

/* Control brightness of all LEDs, set different for every LED.*/
void breathingBrightness()
{
    static int l_ticks_of_led1 = 0;
    static int l_ticks_of_led2 = 0;

    int32_t l_periode_led_4allGeneral = 33;

    /* myšlenka: DigitalOut leds[8] = {PTC4, PTC5, PTC7, PTC8};
    if (g_but9 == 0 && conditionINDEX > 0)
    {
        conditionINDEX--;
    }

    if (button_PTC10(PTC10) == 0)
    {
        duty_cycle += 10;

        if (   ... osetreni hranic (border) ...    )
        {
            duty_cycle = 2;
        }
    }*/

    int duty_cycle_led1 = 50; // PWM duty cycle as a percentage (50 % = on half the time)
    int duty_cycle_led2 = 89; // feel free to change my value

    // calculate the duration of the on and off periods based on the duty cycle
    int on_duration_led1 = l_periode_led_4allGeneral * duty_cycle_led1 / 100; // float??? :)
    int on_duration_led2 = l_periode_led_4allGeneral * duty_cycle_led2 / 100;

    // turn on the LED for the 'on_duration'
    led1 = (l_ticks_of_led1 < on_duration_led1) ? 1 : 0; // if 1 ---> LED On
    led2 = (l_ticks_of_led2 < on_duration_led2) ? 1 : 0; // if 1 ---> LED On

    l_ticks_of_led1++;                                // milliseconds counter
    if (l_ticks_of_led1 >= l_periode_led_4allGeneral) // end of periode?
    {
        l_ticks_of_led1 = 0; // start periode again
    }

    l_ticks_of_led2++;                                // milliseconds counter
    if (l_ticks_of_led2 >= l_periode_led_4allGeneral) // end of periode?
    {
        l_ticks_of_led2 = 0; // start periode again
    }
}

void control_from_ticker()
{
    static int l_ticks_of_led1 = 0;
    static int l_ticks_of_led2 = 0;

    int32_t l_periode_led_4allGeneral = 1000;

    /***********************************core***********************************/
    // turn on LEDs one by one
    if (l_ticks_of_led1 == 0)                                // time to switch on?
        led1 = 1;                                            // LED On
    if (l_ticks_of_led1 >= l_periode_led_4allGeneral * 0.75) // end of periode?
        led1 = 0;                                            // LED Off

    if (l_ticks_of_led2 == l_periode_led_4allGeneral * 0.25) // time to switch on?
        led2 = 1;                                            // LED On
    if (l_ticks_of_led2 == l_periode_led_4allGeneral * 0.50) // end of periode?
        led2 = 0;                                            // LED Off
    /***********************************core***********************************/

    l_ticks_of_led1++;                                // milliseconds counter
    if (l_ticks_of_led1 >= l_periode_led_4allGeneral) // end of periode?
    {
        l_ticks_of_led1 = 0; // start periode again
    }

    l_ticks_of_led2++;                                // milliseconds counter
    if (l_ticks_of_led2 >= l_periode_led_4allGeneral) // end of periode?
    {
        l_ticks_of_led2 = 0; // start periode again
    }
}

int main()
{
    printf("--Poustim program a kontroluji seriovou linku ve skolnim terminalu :D--\n");

    tick1.attach(callback(&breathingBrightness), 0.001f);
    // tick2.attach(callback(&control_from_ticker), 0.001f);// NEpouštím zároveň (najednou)

    wait_ms(osWaitForever); // jen kvůli webové verzi, jinak u callbacku NEužívat (jako printf)
}
