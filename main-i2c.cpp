// **************************************************************************
//
//               Demo program for APPS labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 02/2022
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         Main program for I2C bus
//
// **************************************************************************

/*#include <mbed.h>

#include "i2c-lib.h"
#include "si4735-lib.h"


// Direction of I2C communication
#define R	0b00000001
#define W	0b00000000

#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

DigitalOut g_led_PTA1( PTA1, 0 );
DigitalOut g_led_PTA2( PTA2, 0 );

DigitalIn g_but_PTC9( PTC9 );
DigitalIn g_but_PTC10( PTC10 );
DigitalIn g_but_PTC11( PTC11 );
DigitalIn g_but_PTC12( PTC12 );

int main( void )
{
	uint8_t l_S1, l_S2, l_RSSI, l_SNR, l_MULT, l_CAP;
	uint8_t l_ack = 0;

	printf( "K64F-KIT ready...\r\n" );

	i2c_init();

	// communication with 8 bit expander PCF8574

	// start communication
	i2c_start();

	// PCF8574 addressing
	// The address is composed from 3 parts!
	//l_ack = i2c_output( HWADR_PCF8574 | A012 | W );

	// Check l_ack! Return value must be 0!
	// ....

	//l_ack = i2c_output( Any_8_bit_value );
	// selected LEDs should light

	// stop communication
	i2c_stop();

	if ( ( l_ack = si4735_init() ) != 0 )
	{
		printf( "Initialization of SI4735 finish with error (%d)\r\n", l_ack );
		return 0;
	}
	else
		printf( "SI4735 initialized.\r\n" );

	printf( "\nTunig of radio station...\r\n" );

	// Required frequency in MHz * 100
	int l_freq = 10140; // Radiozurnal

	// Tuning of radio station
	i2c_start();
	l_ack |= i2c_output( SI4735_ADDRESS | W);
	l_ack |= i2c_output( 0x20 );			// FM_TUNE_FREQ
	l_ack |= i2c_output( 0x00 );			// ARG1
	l_ack |= i2c_output( l_freq >> 8 );		// ARG2 - FreqHi
	l_ack |= i2c_output( l_freq & 0xff );	// ARG3 - FreqLo
	l_ack |= i2c_output( 0x00 );			// ARG4
	i2c_stop();
	// Check l_ack!
	// if...

	// Tuning process inside SI4735
	wait_us( 100000 );
	printf( "... station tuned.\r\n\n" );

	// Example of reading of tuned frequency
	i2c_start();
	l_ack |= i2c_output( SI4735_ADDRESS | W );
	l_ack |= i2c_output( 0x22 );			// FM_TUNE_STATUS
	l_ack |= i2c_output( 0x00 );			// ARG1
	// repeated start
	i2c_start();
	// change direction of communication
	l_ack |= i2c_output( SI4735_ADDRESS | R );
	// read data
	l_S1 = i2c_input();
	i2c_ack();
	l_S2 = i2c_input();
	i2c_ack();
	l_freq = ( uint32_t ) i2c_input() << 8;
	i2c_ack();
	l_freq |= i2c_input();
	i2c_ack();
	l_RSSI = i2c_input();
	i2c_ack();
	l_SNR = i2c_input();
	i2c_ack();
	l_MULT = i2c_input();
	i2c_ack();
	l_CAP = i2c_input();
	i2c_nack();
	i2c_stop();

	if ( l_ack != 0 )
		printf( "Communication error!\r\n" );
	else
		printf( "Current tuned frequency: %d.%dMHz\r\n", l_freq / 100, l_freq % 100 );

	return 0;
}*/

#include <mbed.h>

#include "i2c-lib.h"
#include "si4735-lib.h"

//************************************************************************

// Direction of I2C communication
#define R	0b00000001
#define W	0b00000000

#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

DigitalOut g_led_PTA1( PTA1, 0 );
DigitalOut g_led_PTA2( PTA2, 0 );

DigitalIn but1( PTC9 );
DigitalIn but2( PTC10 );
DigitalIn but3( PTC11 );
DigitalIn but4( PTC12 );

//--SABLONA--//
uint8_t i2c_out_in( uint8_t t_adr, uint8_t *t_out_data, uint32_t t_out_len, uint8_t *t_in_data, uint32_t t_in_len )
{
  i2c_start();

  uint8_t l_ack = i2c_output( t_adr | W );

  if ( l_ack == 0 )
  {
    for (int i=0;i<t_out_len;i++ )
    {
      l_ack |= i2c_output( t_out_data[i] ); // send all t_out_data
    }
  }

  if ( l_ack != 0 ) // error?
  {
    i2c_stop();
    return l_ack;
  }

  if ( t_in_data != nullptr )
  {
    i2c_start(); // repeated start

    l_ack |= i2c_output( t_adr | R );

    for (int i=0;i<t_in_len;i++ )
    {
      i2c_ack();
      t_in_data[i] = i2c_input(); // receive all t_data_in
    }

    i2c_nack();
  }

  i2c_stop();

  return l_ack;
}

class Expander
{
public:
	void bar(uint8_t t_level)
	{
		/*uint8_t led=0;
		for(uint8_t i=0;i<t_level;i++)
		{
			led=(led<<1);
			//led=led+1;
		}
		uint8_t data_out[1]={led};
		int l_ack = i2c_out_in(0x4E, data_out, 1, nullptr, 0);*/

		uint8_t led= 0b00000000;
	    for(uint8_t i = 0; i<t_level; i++)
	    {
	     led = led<<1;
	     led +=1;
	     uint8_t data_out[4]={led};
	     int l_ack = i2c_out_in(0x4E, data_out, 4, nullptr, 0);
	     //i2c_start();
	     //l_ack = i2c_output( 0b01000000 );
	     //l_ack = i2c_output(led);
	     //i2c_stop();
	     //wait_us(250000);
	     //if (i == 8)
	     //{
	      //i = 0;
	     //}
	    }
	}
};

class Radio
{
public:
	uint8_t set_volume(uint16_t sila)
	{
	    uint8_t l_data_out[ 6 ] = { 0x12, 0x00, 0x40, 0x00, 0x00, sila };
	    return i2c_out_in( SI4735_ADDRESS, l_data_out, 6, nullptr, 0 );
	}

	uint8_t search_freq()
	{
		uint8_t l_data_out[2]={0x21,0b00001100};
		return i2c_out_in(SI4735_ADDRESS,l_data_out,2,nullptr,0);

	}
	uint8_t set_freq( uint16_t t_freq )
	{
	  uint8_t l_data_out[ 5 ] = { 0x20, 0x00, t_freq >> 8, t_freq & 0xFF, 0 };
	  return i2c_out_in( SI4735_ADDRESS, l_data_out, 5, nullptr, 0 );
	}
	uint8_t get_tune_status( uint8_t *t_data_status, uint32_t t_data_len )
	{
	  uint8_t l_data_out[ 2 ] = { 0x22, 0 };
	  return i2c_out_in( SI4735_ADDRESS, l_data_out, 2, t_data_status, t_data_len );
	}

};
int main()
{
	 int frekvencia=9500;
	 int volume = 20;
	 Radio radio;
	 int l_ack;
	 i2c_init();

	 if( (l_ack = si4735_init() != 0))
	 {
	  printf("Initialization of SI4735 finish with error (%d)\r\n", l_ack);
	  return 0;
	 }
	 else{
	  printf("SI4735 initialized.\r!n");
	 }

	 l_ack = radio.set_freq(9100);
	 l_ack = radio.set_volume(40);
	 if ( l_ack != 0 )
	 {
		printf( "Communication error!\r\n" );
	 }
	 else
	 {
		 printf( "Current tuned frequency: %d.%dMHz\r\n", frekvencia / 100, frekvencia % 100 );
	 }
	 //int pole[] = radio.get_tune_status(db,cislo);

	 //i2c_start();
	 if(!l_ack)
	 {
	  printf("Ne");
	 }

	 Expander led;
	 //led.bar(3);

	/* while(1)
	 {
	   i2c_init();*/

	 //}


	 while(1)
	 {
		 led.bar(4);
	   if(!but1)
	   {
	    //l_ack = radio.set_freq(8900);
	    /*if ( l_ack != 0 )
	    	 {
	    		printf( "Communication error!\r\n" );
	    	 }
	    	 else
	    	 {
	    		 printf( "Current tuned frequency: %d.%dMHz\r\n", frekvencia / 100, frekvencia % 100 );
	    	 }*/
	    while(!but1);
	   }
	   if(!but2)
	   {
	    volume =volume+ 2;
	    if (volume >= 63)
	    {
	    	volume = 20;
	    }
	    l_ack = radio.set_volume(volume);
	    if ( l_ack != 0 )
	    {
	    	printf( "Communication error!\r\n" );
	    }
	    else
	    {
	    	printf( "Current volume: .%d \r\n", volume);
	    }
	    while(!but2);
	   }
	   if(!but3)
	 	   {
	 	    frekvencia =frekvencia+ 50;
	 	    if (frekvencia >= 10800)
	 	    {
	 	    	frekvencia = 10800;
	 	    }
	 	    l_ack = radio.set_freq(frekvencia);
	 	    if ( l_ack != 0 )
	 	   	 {
	 	   		printf( "Communication error!\r\n" );
	 	   	 }
	 	   	 else
	 	   	 {
	 	   		 printf( "Current tuned frequency: %d.%dMHz\r\n", frekvencia / 100, frekvencia % 100 );
	 	   	 }
	 	    while(!but3);
	 	   }
	   if(!but4)
	   	 	   {
	   	 	    frekvencia =frekvencia- 50;
	   	 	    if (frekvencia <= 8900)
	   	 	    {
	   	 	    	frekvencia = 8900;
	   	 	    }
	   	 	    l_ack = radio.set_freq(frekvencia);
	   	 	    if ( l_ack != 0 )
	   	 		 {
	   	 			printf( "Communication error!\r\n" );
	   	 		 }
	   	 		 else
	   	 		 {
	   	 			 printf( "Current tuned frequency: %d.%dMHz\r\n", frekvencia / 100, frekvencia % 100 );
	   	 		 }
	   	 	    while(!but4);
	   	 	   }

	 };

	 //return 0;
}

/*#include <mbed.h>
#include "i2c-lib.h"
#include "si4735-lib.h"

//************************************************************************

// Direction of I2C communication
#define R 0b00000001
#define W 0b00000000

#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

DigitalOut g_led_PTA1( PTA1, 0 );
DigitalOut g_led_PTA2( PTA2, 0 );

DigitalIn but1( PTC9 );
DigitalIn but2( PTC10 );
DigitalIn g_but_PTC11( PTC11 );
DigitalIn g_but_PTC12( PTC12 );


///////////////////////////////////////////////////
uint8_t i2c_out_in( uint8_t t_adr, uint8_t *t_out_data, uint32_t t_out_len,uint8_t *t_in_data, uint32_t t_in_len ){
  i2c_start();

  uint8_t l_ack = i2c_output( t_adr | W );

  if ( l_ack == 0 ){
    for (int i = 0; i < t_out_len; i++ ) {
      l_ack |= i2c_output( t_out_data[ i ] ); // send all t_out_data
    }
  }
    //
  if ( l_ack != 0 ){ //eror ?
    i2c_stop();
    return l_ack;
  }
    //
  if ( t_in_data != nullptr ){
    i2c_start(); // repeated start

    l_ack |= i2c_output( t_adr | R );

    for (int i = 0; i < t_in_len; i++ ){
      i2c_ack();
      t_in_data[ i ] = i2c_input(); // receive all t_data_in
    }

    i2c_nack();
  }

  i2c_stop();

  return l_ack;
  }

////Vytvořte si třídu Expander s metodou void bar( uint8_t t_level );. Tato metoda zobrazí požadovaný počet (t_level) LED v řadě (sloupeček).

  class Expander{

  public:
   Expander(){

   }

   void bar(uint8_t t_leve){
    int led = 0;

    for(uint8_t i = 0; i < t_leve; i ++){
     led = (led << 1);
     led += 1;
    }
    uint8_t data_out[4] = {led};
    int l_ack = i2c_out_in(0x4E, data_out, 1, nullptr, 0);
   }
  };


//Implementujte si třídu Radio s metodami set_volume( .. ),  search_freq( .. ),

  class Radio{

  public:
   uint8_t set_volume(uint16_t sila){
    if(sila > 63){
     sila = 10;
    }
    uint8_t l_data_out[ 6 ] = { 0x12, 0x00, 0x40, 0x00, 0x00, sila };
    return i2c_out_in( SI4735_ADDRESS, l_data_out, 6, nullptr, 0 );
   }


   uint8_t search_freq(){
    uint8_t l_data_out[ 2 ] = { 0x21, 0b00001100};
    return i2c_out_in( SI4735_ADDRESS, l_data_out, 2, nullptr, 0 );
   }


   uint8_t set_freq( uint16_t t_freq ) {
     uint8_t l_data_out[ 5 ] = { 0x20, 0x00, t_freq >> 8, t_freq & 0xFF, 0 };
     return i2c_out_in( SI4735_ADDRESS, l_data_out, 5, nullptr, 0 );
   }

   uint8_t get_tune_status( uint8_t *t_data_status, uint32_t t_data_len ) {
     uint8_t l_data_out[ 2 ] = { 0x22, 0 };
     return i2c_out_in( SI4735_ADDRESS, l_data_out, 2, t_data_status, t_data_len );
   }
  };

//////////////////////////////////////////////////

int main( void ){
 //uint8_t l_S1, l_S2, l_RSSI, l_SNR, l_MULT, l_CAP;
 //uint8_t l_ack = 0;

 Radio rad;
 int l_ack;

 i2c_init();

 if( (l_ack = si4735_init() != 0)){
  printf("Initialization of SI4735 finish with error (%d)\r\n", l_ack);
  return 0;
 }
 else{
  printf("SI4735 initialized.\r!n");
 }

 l_ack = rad.set_freq(9100);
 l_ack = rad.set_volume(40);

 //i2c_start();
 if(!l_ack){
  printf("Ne");
 }

 Expander led;
 led.bar(4);

 int volume = 40;
 while(1) {


   if(!but1){
    l_ack = rad.set_freq(8900);
    while(!but1);
     }
   if(!but2){
    volume += 1;
    if (volume == 55) volume = 35;
       l_ack = rad.set_volume(volume);
       while(!but2);
        }
 };
 return 0;
}*/

