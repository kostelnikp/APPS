#include "mbed.h"

#define WAIT_TIME_US 500000

class Blinker
{
public:
    Blinker(DigitalOut & led ) : g_led(led){
        this->LightnesOfLed = 10;
        this->ticks = 0;
    }
    void Blink()
    {
        if(this->ticks < this->LightnesOfLed)
        {
            this->g_led = 1;
        }
        else{
            g_led = 0;
        }

        this->ticks = this->ticks < 10 ? this->ticks + 1 : 0;
    }

    void IncreseLight()
    {
        this->LightnesOfLed = this->LightnesOfLed < 10 ? this->LightnesOfLed + 1 : this->LightnesOfLed;
    }
    void DecreseLight()
    {
        this->LightnesOfLed = this->LightnesOfLed > 0 ? this->LightnesOfLed - 1 : this->LightnesOfLed;
    }
private:
    int LightnesOfLed;
    int ticks;
    DigitalOut & g_led;
};

DigitalOut g_led_PTA1( PTA1, 0 );
DigitalOut g_led_PTA2( PTA2, 0 );

DigitalIn g_but_1(PTC9);
DigitalIn g_but_2(PTC10);


int main()
{
    printf("This is the bare metal blinky example running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    Blinker b1(g_led_PTA1);
    Ticker t1;
    t1.attach_us(callback(&b1, &Blinker::Blink), 1000);

    while (true){
        if(!g_but_1){
            b1.DecreseLight();
            while(!g_but_1);
        }
        if(!g_but_2){
            b1.IncreseLight();
           while(!g_but_2);
        }
    }
}
