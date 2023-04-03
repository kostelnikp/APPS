#include "mbed.h"

#define WAIT_TIME_US 500000

DigitalOut g_led_PTB2( PTB2, 0 );
DigitalOut g_led_PTB3( PTB3, 0 );
DigitalOut g_led_PTB9( PTB9, 0 );


DigitalOut g_led_PTA1( PTA1, 0 );
DigitalOut g_led_PTA2( PTA2, 0 );

DigitalIn g_but_PTC9(PTC9);
DigitalIn g_but_PTC10(PTC10);
DigitalIn g_but_PTC11(PTC11);

class PWMLed
{
public:
    PWMLed(DigitalOut & led ) : g_led(led){
        this->brightness = 27;
        this->ticks = 0;
    }
    void Blink()
    {
        if(this->ticks < this->brightness)
        {
            this->g_led = 1;
        }
        else{
            g_led = 0;
        }

        if (ticks < 27)
        {
        	ticks++;
        }
        else
        {
        	ticks = 0;
        }
    }

    void IncreseLight()
    {
    	if (brightness < 27)
    	{
    		brightness++;
    	}
    }
    void DecreseLight()
    {
    	if (brightness > 0)
    	{
    	    brightness--;
    	}
    }
private:
    int brightness;
    int ticks;
    DigitalOut & g_led;
};



int main()
{
    printf("This is the bare metal blinky example running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    PWMLed br(g_led_PTB2);
    PWMLed bg(g_led_PTB3);
    PWMLed bb(g_led_PTB9);

    PWMLed b1[] = {br,bg,bb};

    int i = 0;

    Ticker t1;
    Ticker t2;
    Ticker t3;

    t1.attach_us(callback(&br, &PWMLed::Blink), 1000);
    t2.attach_us(callback(&bg, &PWMLed::Blink), 1000);
    t3.attach_us(callback(&bb, &PWMLed::Blink), 1000);

    while (true){
    	if(!g_but_PTC11){
    		i++;
    		while(!g_but_PTC10);
    		if (i>=3)
    		{
    			i = 0;
    		}

    	}
    	switch (i){
    		case 0:
    			if(!g_but_PTC9)
    			{
    				br.DecreseLight();
    			 	while(!g_but_PTC9);
    			 }
    			 if(!g_but_PTC10)
    			 {
    			     br.IncreseLight();
    			     while(!g_but_PTC10);
    			  }
    			break;
    		case 1:
    			if(!g_but_PTC9)
    			{
    			     bb.DecreseLight();
    			     while(!g_but_PTC9);
    			}
    			if(!g_but_PTC10)
    			{
    			      bb.IncreseLight();
    			      while(!g_but_PTC10);
    			}
    			break;
    		case 2:
    			if(!g_but_PTC9)
    			{
    			      bg.DecreseLight();
    			      while(!g_but_PTC9);
    			}
    			if(!g_but_PTC10)
    			{
    			    bg.IncreseLight();
    			    while(!g_but_PTC10);
    			}
    			break;
    		}

    }
}
