/*
 * mySpectrum_High_Freq_LCD_Not_Optimized.c
 *
 * Created: 12-Jan-18 12:56:03 AM
 * Author : Ealham
 */ 
/*
 * Lcd_ADC_Microphone.c
 *
 * Created: 10-Jan-18 4:42:06 PM
 * Author : Ealham
 */ 

#define F_CPU 1000000UL //
/*
//Shudhu PORTA diyei LCD dekhano jachcey
#define D4 eS_PORTA1
#define D5 eS_PORTA2
#define D6 eS_PORTA3
#define D7 eS_PORTA4
#define RS eS_PORTA5 //28
#define EN eS_PORTA6 //29
*/

#include <avr/io.h>
#include <util/delay.h>

#include <string.h>
#include <stdlib.h>
#include <math.h>

//#include "lcd.h"


#define sampleSize 20
#define outputLine 10
#define  sampleRate 4000
char buffer[20];

int Samples[sampleSize];
int Count_Index;

long Cosine[]={
	10000 ,10000 ,10000 ,10000 ,10000 ,10000 ,10000 ,10000 ,10000 ,10000 ,10000 ,10000 ,10000 ,10000 ,10000 ,10000 ,10000 ,10000 ,10000 ,10000 ,
	10000 ,9510 ,8090 ,5877 ,3090 ,0 ,-3090 ,-5877 ,-8090 ,-9510 ,-10000 ,-9510 ,-8090 ,-5877 ,-3090 ,0 ,3090 ,5877 ,8090 ,9510 ,
	10000 ,8090 ,3090 ,-3090 ,-8090 ,-10000 ,-8090 ,-3090 ,3090 ,8090 ,10000 ,8090 ,3090 ,-3090 ,-8090 ,-10000 ,-8090 ,-3090 ,3090 ,8090 ,
	10000 ,5877 ,-3090 ,-9510 ,-8090 ,0 ,8090 ,9510 ,3090 ,-5877 ,-10000 ,-5877 ,3090 ,9510 ,8090 ,0 ,-8090 ,-9510 ,-3090 ,5877 ,
	10000 ,3090 ,-8090 ,-8090 ,3090 ,10000 ,3090 ,-8090 ,-8090 ,3090 ,10000 ,3090 ,-8090 ,-8090 ,3090 ,10000 ,3090 ,-8090 ,-8090 ,3090 ,
	10000 ,0 ,-10000 ,0 ,10000 ,0 ,-10000 ,0 ,10000 ,0 ,-10000 ,0 ,10000 ,0 ,-10000 ,0 ,10000 ,0 ,-10000 ,0 ,
	10000 ,-3090 ,-8090 ,8090 ,3090 ,-10000 ,3090 ,8090 ,-8090 ,-3090 ,10000 ,-3090 ,-8090 ,8090 ,3090 ,-10000 ,3090 ,8090 ,-8090 ,-3090 ,
	10000 ,-5877 ,-3090 ,9510 ,-8090 ,0 ,8090 ,-9510 ,3090 ,5877 ,-10000 ,5877 ,3090 ,-9510 ,8090 ,0 ,-8090 ,9510 ,-3090 ,-5877 ,
	10000 ,-8090 ,3090 ,3090 ,-8090 ,10000 ,-8090 ,3090 ,3090 ,-8090 ,10000 ,-8090 ,3090 ,3090 ,-8090 ,10000 ,-8090 ,3090 ,3090 ,-8090 ,
	10000 ,-9510 ,8090 ,-5877 ,3090 ,0 ,-3090 ,5877 ,-8090 ,9510 ,-10000 ,9510 ,-8090 ,5877 ,-3090 ,0 ,3090 ,-5877 ,8090 ,-9510
};

long Sine[]={
	0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
	0 ,3090 ,5877 ,8090 ,9510 ,10000 ,9510 ,8090 ,5877 ,3090 ,0 ,-3090 ,-5877 ,-8090 ,-9510 ,-10000 ,-9510 ,-8090 ,-5877 ,-3090 ,
	0 ,5877 ,9510 ,9510 ,5877 ,0 ,-5877 ,-9510 ,-9510 ,-5877 ,0 ,5877 ,9510 ,9510 ,5877 ,0 ,-5877 ,-9510 ,-9510 ,-5877 ,
	0 ,8090 ,9510 ,3090 ,-5877 ,-10000 ,-5877 ,3090 ,9510 ,8090 ,0 ,-8090 ,-9510 ,-3090 ,5877 ,10000 ,5877 ,-3090 ,-9510 ,-8090 ,
	0 ,9510 ,5877 ,-5877 ,-9510 ,0 ,9510 ,5877 ,-5877 ,-9510 ,0 ,9510 ,5877 ,-5877 ,-9510 ,0 ,9510 ,5877 ,-5877 ,-9510 ,
	0 ,10000 ,0 ,-10000 ,0 ,10000 ,0 ,-10000 ,0 ,10000 ,0 ,-10000 ,0 ,10000 ,0 ,-10000 ,0 ,10000 ,0 ,-10000 ,
	0 ,9510 ,-5877 ,-5877 ,9510 ,0 ,-9510 ,5877 ,5877 ,-9510 ,0 ,9510 ,-5877 ,-5877 ,9510 ,0 ,-9510 ,5877 ,5877 ,-9510 ,
	0 ,8090 ,-9510 ,3090 ,5877 ,-10000 ,5877 ,3090 ,-9510 ,8090 ,0 ,-8090 ,9510 ,-3090 ,-5877 ,10000 ,-5877 ,-3090 ,9510 ,-8090 ,
	0 ,5877 ,-9510 ,9510 ,-5877 ,0 ,5877 ,-9510 ,9510 ,-5877 ,0 ,5877 ,-9510 ,9510 ,-5877 ,0 ,5877 ,-9510 ,9510 ,-5877 ,
	0 ,3090 ,-5877 ,8090 ,-9510 ,10000 ,-9510 ,8090 ,-5877 ,3090 ,0 ,-3090 ,5877 ,-8090 ,9510 ,-10000 ,9510 ,-8090 ,5877 ,-3090
};


double DFT_Real,DFT_Imag;
double FinalResult[outputLine];

int rowEnable[]={1,0b00000011,0b00000111,0b00001111,0b00011111,0b00111111,0b01111111,0b11111111}; //1 maney sobar nicher ta enable hoisey
int columnGrnd[]={0b01111111,0b10111111,0b11011111,0b11101111,0b11110111,0b11111011,0b11111101,0b11111110};
int barSize;

void timer_Init()
{
	TCCR1B = (1<<CS10)|(1<<WGM12);
	//wgm12 maney OCR1A er sathey match korlei TCNT1 Zero tey back korbey
	//CS12-CS11-CS10
	// 0   0     1 -> no Prescaling
	
	/*
		18 3 160-167
		19 3 160-167
	*/
	OCR1A=146;//	21 is the theoretically calculated value
					

}

void adc_Init()
{
	
	ADMUX = 0b01000000;
	ADCSRA = 0b10000011; // prescalar 8
	
}



int adc_Read()
{
	int adcLow,adcHigh,adcResult;
	ADCSRA |= (1<<ADSC);
	while( ADCSRA & (1<<ADSC) ) ;

	adcLow  = ADCL;
	adcHigh = ADCH;

	adcResult = adcHigh & 3;
	adcResult = adcHigh<<8;
	adcResult = adcResult|adcLow;
	
	return adcResult;
}

void Show()
{
	for (int i=0;i<8;i++)
	{
		barSize=(int)(FinalResult[i+1]/35);
		if (barSize>7)
		PORTB=rowEnable[7];
		else
		PORTB=rowEnable[barSize];
		PORTD=columnGrnd[i];
		_delay_ms(1);
		
	}
	
	PORTD=0xFF;
}

void ShowBottomUp()
{
	for (int i=0;i<8;i++)
	{
		PORTD=0xFF;
		for (int k=1;k<=8;k++)
		{
			//FinalResult[k]/
			PORTB=rowEnable[i];	
		}
	}
}

int main(void)
{
	DDRB=0xFF;
	DDRD=0xFF;
	
    
	
	adc_Init();
	timer_Init();
	
	
	//PORTB=0xFF;
	//PORTD=0b11111110;
	//while(1);

	/*
			The OCF1A bit is set (one) when a compare match occurs
			between the Timer/Counter1 and the data in OCR1A.  [(maney miley geley 1 hobe) nailey zeroi thakbe]
			OCF1A is cleared by hardware when executing the 
			corresponding interrupt handling vector.

			****Alternatively, OCF1A is cleared by writing a logic one to the flag****
	*/
	
	
    while (1) 
    {

		//for (int i=0;i<2;i++)
		//Show();
		


		TCNT1=0;
		TIFR |= 1<<OCF1A; //OCIF all-clear
		for (int i=0;i<sampleSize;i++)
		{
			while( (TIFR & (1<<OCF1A)) == 0 ) ;

			//512 hochcey mid ... er baam-daan e utha-nama korbey
			Samples[i]=adc_Read()-512;
			TIFR |= 1<<OCF1A;
		}

		
		
		Show();

		Count_Index=0;
		for (int k=0;k<outputLine;k++)
		{
			
			DFT_Real=DFT_Imag=0;

			for (int i=0;i<sampleSize;i++)
			{
				
				DFT_Real+=Samples[i]*Cosine[Count_Index];
				DFT_Imag-=Samples[i]*Sine[Count_Index];

				Count_Index++;

				
			}
			DFT_Real/=sampleSize;
			DFT_Real/=10000;
			DFT_Imag/=sampleSize;
			DFT_Imag/=10000;
			
			if (DFT_Real<-1) DFT_Real*=-1;
			if (DFT_Imag<-1) DFT_Imag*=-1;

			FinalResult[k]=sqrt(DFT_Real*DFT_Real+DFT_Imag*DFT_Imag);
			
			
		}
		
		
		Show();
		
		
		
		
		
		
    }

	
}

