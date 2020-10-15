#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "config.h"

void adc_ini(void);
void port_ini(void);
void timer_ini(void);
void set_led(uint8_t);
void get_key(void);
void key_porcess(uint8_t);
void current_state_timeout(void);
void LED_config(void);

struct config apollo = {2, 0, 1, 0, 0, 0, 0, 0};
volatile static uint16_t current_state_time = 0;
volatile unsigned char flag_VALVE_CONTROL = 0;


ISR(TIMER0_COMPA_vect){
	static uint8_t beep_count;
	static uint16_t blink_timeout;
	//static uint16_t counter_VALVE_CONTROL = 500;
	static uint16_t counter_VALVE_CONTROL = 750;
	get_key();
//	if((apollo.state)&&(apollo.cycle_count!=10))
    	++current_state_time;

	//if (++blink_timeout > 250){
	if (++blink_timeout > 360){
			LED_config();
		blink_timeout = 0;
	}

	if (!apollo.state)
	return;

	if((apollo.state == 1)&&(apollo.cycle_count==10)){

		if(flag_VALVE_CONTROL){

			if(counter_VALVE_CONTROL!=0){

				counter_VALVE_CONTROL--;
				PORTC |= VALVE_CONTROL;
				PORTC &= ~COMPRESSOR_CONTROL;
			}
			else{

				PORTC &= ~VALVE_CONTROL;
				//counter_VALVE_CONTROL = 500;
				counter_VALVE_CONTROL = 750;
				flag_VALVE_CONTROL = 0;
			}
		}
		else if(apollo.actual_pressure < (pressure_value[apollo.position_hand_work] - DELTA)){

			PORTC &= ~VALVE_CONTROL;
			PORTC |= COMPRESSOR_CONTROL;
		}

		else if(apollo.actual_pressure > (pressure_value[apollo.position_hand_work] + DELTA)){

			PORTC &= ~COMPRESSOR_CONTROL;
		}
	}

	if((apollo.state == 1)&&(apollo.cycle_count!=10)){	// apollo.cycle_count == 1-9 10- ����� ��������
		switch(cycle_config[apollo.cycle_position]){

			case(PMAX):
			LED_PROG_OFF;
			LED_PMIN_OFF;
			LED_PMAX_OFF;

			LED_PMAX_ON;

			if(apollo.actual_pressure < (pressure_value[apollo.Pmax_index] - DELTA)){

				PORTC &= ~VALVE_CONTROL;
				PORTC |= COMPRESSOR_CONTROL;
				PORTD |= 0x1F;
				PORTC |= 0x0C;
				set_led(apollo.Pmax_index-1);

			}
			else if(apollo.actual_pressure > (pressure_value[apollo.Pmax_index] + DELTA)){
				PORTD |= 0x1F;
				PORTC |= 0x0C;
				set_led(apollo.Pmax_index-1);
				PORTC &= ~(COMPRESSOR_CONTROL | VALVE_CONTROL);
			}

			if (current_state_time > Pmax_duration){
				current_state_timeout();
				current_state_time = 0;
			}
			break;

			case(PMIN):
				LED_PROG_OFF;
				LED_PMIN_OFF;
				LED_PMAX_OFF;

				LED_PMIN_ON;

			if(apollo.actual_pressure < (pressure_value[apollo.Pmin_index] - DELTA)){

				PORTC &= ~VALVE_CONTROL;
				PORTC |= COMPRESSOR_CONTROL;
				PORTD |= 0x1F;
				PORTC |= 0x0C;
				set_led(apollo.Pmin_index+1);
			}

			else if(apollo.actual_pressure > (pressure_value[apollo.Pmin_index] + DELTA)){
				PORTC &= ~(COMPRESSOR_CONTROL | VALVE_CONTROL);
				PORTD |= 0x1F;
				PORTC |= 0x0C;
				set_led(apollo.Pmin_index+1);
			}

			if (current_state_time > Pmin_duration){

				current_state_timeout();
				current_state_time = 0;
			}
			break;

			case(POFF):
				LED_PROG_OFF;
				LED_PMIN_OFF;
				LED_PMAX_OFF;

				LED_PROG_ON

			PORTC &= ~COMPRESSOR_CONTROL;
			PORTC |= VALVE_CONTROL;
			PORTD |= 0x1F;
			PORTC |= 0x0C;
			set_led(apollo.cycle_count);

			if (current_state_time > Poff_duration){

				current_state_timeout();
				current_state_time = 0;
			}
			break;

			default:
			current_state_timeout();
			current_state_time = 0;
			break;
		}
	}

	if (apollo.state == 2){

		PORTC &= ~COMPRESSOR_CONTROL;
		PORTC |= VALVE_CONTROL;

		if(current_state_time < BEEP_duration){
			apollo.LED_mode = 0;
			PORTC |= BUZ;
		}

		if(current_state_time > BEEP_duration){

			PORTC &= ~BUZ;
		}

		if(current_state_time > BEEP_duration * 2){

			current_state_time = 0;
			beep_count++;

			if(beep_count > 3){

				PORTC &= ~BUZ;
				beep_count = 0;
				apollo.state = 0;
				apollo.cycle_position = 0;
				apollo.cycle_count = 1;
				PORTC &= ~VALVE_CONTROL;
			//	LED_config();
			}
		}
	}
}



ISR(ADC_vect){
	apollo.actual_pressure=ADC;
}

int main(void){
	cli();			//disable all interrupts
	port_ini();
	timer_ini();

	adc_ini();
	sei();
	LED_config();
	while(1)
	{
		asm("nop");
		asm("nop");
	}
}

void adc_ini(void){
	ADMUX=0x47;		//Vref = AVcc, MUX->ADC7
	ADCH=0x00;
	ADCL=0x00;
	ADCSRB=0x00;	//free runing mode
	ADCSRA=0xEF;  	//128
}

void port_ini(void){
	DDRB =  0x3F;
	PORTB = 0xC5;

	DDRD =  0x9F;
	PORTD = 0xFF;


	DDRC =  0xFF;
	PORTC = 0x0C;
}

 void set_led(uint8_t number){
  PORTD|=((1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4));
  PORTC|=((1<<2)|(1<<3));
  switch(number){

   case 0:
   {
   	PORTD&=~((1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4));
   	PORTC&=~(1<<2);
   }
   break;

   case 1:
   {
    PORTD&=~((1<<1)|(1<<2));
   }
   break;

   case 2:
   {
    PORTD&=~((1<<0)|(1<<1)|(1<<3)|(1<<4));
    PORTC&=~(1<<3);
   }
   break;

   case 3:
   {
    PORTD&=~((1<<0)|(1<<1)|(1<<2)|(1<<3));
    PORTC&=~(1<<3);
   }
   break;

   case 4:
   {
    PORTD&=~((1<<1)|(1<<2));
    PORTC&=~((1<<2)|(1<<3));
   }
   break;

   case 5:
   {
    PORTD&=~((1<<0)|(1<<2)|(1<<3));
    PORTC&=~((1<<2)|(1<<3));
   }
   break;

   case 6:
   {
    PORTD&=~((1<<0)|(1<<2)|(1<<3)|(1<<4));
    PORTC&=~((1<<2)|(1<<3));
   }
   break;

   case 7:
   {
    PORTD&=~((1<<0)|(1<<1)|(1<<2));
   }
   break;

   case 8:
   {
    PORTD&=~((1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4));
    PORTC&=~((1<<2)|(1<<3));
   }
   break;

   case 9:
   {
    PORTD&=~((1<<0)|(1<<1)|(1<<2)|(1<<3));
    PORTC&=~((1<<2)|(1<<3));
   }
   break;
  }
}

void timer_ini(void){
	TCCR0A = 0b10;    //CTC
	TCCR0B = 0b11;    //Fclk  = Fsys/64 ;
	OCR0A  = 125;
	TIMSK0 = (1 << OCIE0A);
}

void current_state_timeout(void){

	apollo.cycle_position++;
	if (apollo.cycle_position >= sizeof(cycle_config)){

		apollo.cycle_position = 0;
		apollo.cycle_count--;

		if (apollo.cycle_count == 0){

			apollo.state = 2;
			apollo.cycle_count = 1;
		}
	}
}

void key_porcess(uint8_t key){
		switch(key){

			case(1):	//Stop //start
			if(!apollo.state){
				apollo.state = 1;
				apollo.LED_mode = 6;
				if (apollo.cycle_count == 10){
					apollo.LED_mode = 0;
				}
				current_state_time = 0; //��������� �������
			}
			else if(apollo.state == 1){
				apollo.state = 2;
				apollo.LED_mode = 0;
			}
			break;

			case(2):     ///Pmin
			if((apollo.LED_mode == 2)||(apollo.cycle_count == 10)){

				if(apollo.cycle_count == 10){
					if(apollo.position_hand_work){
						apollo.position_hand_work--;
						flag_VALVE_CONTROL = 1;
					}
				}
				else{
					if (apollo.Pmin_index)
					apollo.Pmin_index = 0;

					else
					apollo.Pmin_index++;
				}
			}
			apollo.LED_mode = 2;

			break;

			case(3):        ///Pmax+
			if((apollo.LED_mode == 1)||(apollo.cycle_count == 10)){

				if(apollo.cycle_count == 10){
					if(apollo.position_hand_work < 6)
					apollo.position_hand_work++;

				}
				else
				{

					if (++apollo.Pmax_index > 6)
					apollo.Pmax_index = 2;
				}
			}

			apollo.LED_mode = 1;

			break;

			case(4):	//Cycle_count
			if(apollo.LED_mode == 0){
				if(!apollo.state){

					if (++apollo.cycle_count > 10)
					apollo.cycle_count = 1;
				}
			}
			apollo.LED_mode = 0;

			break;

			default:
			break;
	}
}


#define DELAY 50

void get_key(void){
	static unsigned int debounce = 0;
	static unsigned int counter_debounce_time = 0;
	static unsigned int delay_button_on = 0;

	if((!counter_debounce_time)&&(!delay_button_on)) // ������� ���� ������ ����� ������� ������
	{
		if (!(((PIND & 0x60) == 0x60) && ((PINB & 0xC0) == 0xC0))) // ���� ����� ������ ������
		counter_debounce_time++;
	}

	if(counter_debounce_time != 0)
	{
		counter_debounce_time++;
		if (!(((PIND & 0x60) == 0x60) && ((PINB & 0xC0) == 0xC0))) // ���� ����� ������ ������
		debounce++;

		if (counter_debounce_time >= DELAY)	{

			if(debounce >= 30){

				if(!(PINB & KEY_START_STOP))   	key_porcess(1); //key = 1;    //Start/Stop
				else if(!(PIND & KEY_PMIN)){
					if (!(apollo.LED_mode == 6)){
					key_porcess(2); //key = 2;    //Pmin
					}
				}
				else if(!(PIND & KEY_PMAX)){
					if (!(apollo.LED_mode == 6)){
						key_porcess(3); //key = 3;    //Pmax
					}
				}
				else if(!(PINB & KEY_CYCLE)){
					if (!(apollo.LED_mode == 6)){
						key_porcess(4); //key = 4;    //Cycle_count
					}

				}
			}

			debounce = 0;
			counter_debounce_time = 0;
			delay_button_on = 1;
		}
	}

	if(delay_button_on){
		if(++delay_button_on>=600){
			delay_button_on = 0;
		}
		if(((PIND & 0x60) == 0x60) && ((PINB & 0xC0) == 0xC0)){
			delay_button_on = 0;
		}
	}
}



void LED_config(void){

	LED_PROG_OFF;
	LED_PMIN_OFF;
	LED_PMAX_OFF;
	LED_WORK_OFF;

	if(apollo.state)
		LED_WORK_ON;

	switch(apollo.LED_mode){
		case(0):
		if(apollo.cycle_count==10)
			set_led(0);
		else
			set_led(apollo.cycle_count);
		LED_PROG_ON;
		break;

		case(1):
		if(apollo.cycle_count==10)
			set_led(apollo.position_hand_work+1);

		else
		set_led(apollo.Pmax_index-1);

		LED_PMAX_ON;
		break;

		case(2):
		if(apollo.cycle_count==10)
			set_led(apollo.position_hand_work+1);
		else
			set_led(apollo.Pmin_index+1);

		LED_PMIN_ON;
		break;
/*
		case(3):
		set_led();
		break;
*/
	}
}
