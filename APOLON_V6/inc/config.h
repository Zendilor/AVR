#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define MAIN_CLK  8000000   ///������� ����������

///************** PORTC **************
#define COMPRESSOR_CONTROL  (1 << 1)    ///PC0 ���������� ������������
#define VALVE_CONTROL       (1 << 0)    ///PC1 ���������� ��������

#define LED_PROG_ON 		PORTB&=~(1<<1);
#define LED_PROG_OFF		PORTB|= (1<<1);

#define LED_PMIN_ON			PORTB&=~(1<<0); 
#define LED_PMIN_OFF		PORTB|= (1<<0);

#define LED_PMAX_ON			PORTD&=~(1<<7);
#define LED_PMAX_OFF		PORTD|= (1<<7);

#define LED_WORK_ON   		PORTB&=~(1<<2); 
#define LED_WORK_OFF		PORTB|= (1<<2);

#define KEY_PMAX    (1 << 5)    ///PD5 ����� ��������
#define KEY_PMIN    (1 << 6)    ///PD6 ����� ��������

///************** PORTD **************
#define BUZ         	(1 << 5)    ///PC5 ���������� ��������
#define KEY_START_STOP  (1 << 6)    ///PB6 ����11111111111111111111
#define KEY_CYCLE   	(1 << 7)    ///PB7 ����� ���������� ������

#define PRESSURE_SENSOR     (1 << 7)    ///ADC7 ���� ��� - ��������� ��������

const uint16_t Pmax_duration = 15000;   ///������������ ����� �������� ��������
const uint16_t Pmin_duration = 15000;   ///������������ ����� ������� ��������
const uint16_t Poff_duration = 5000;    ///������������ ����� �������� ��������
const uint16_t BEEP_duration = 500;




#define POFF    0
#define PMIN    1
#define PMAX    2

///����������� ������ ��������
const uint8_t cycle_config[] = {PMIN, POFF, PMAX, POFF, PMIN, POFF, PMIN, POFF,
                                PMIN, POFF, PMIN, POFF, PMAX, POFF, PMAX, POFF};

///������� �������� �������� ([10, 15, 20, 25, 30, 35, 40], kPa)
///������������ � ���� ��� (P = (0.018 * P + 0.04) * 1024) Vref = Vcc = 5.0 V

//const uint16_t pressure_value[] = {225, 317, 410, 502, 594, 686, 778};
const uint16_t pressure_value[] = {250, 340, 435, 530, 620, 720, 815};
//#define DELTA   55
//#define DELTA_2 65
#define DELTA   10
#define DELTA_2 60

const uint16_t pressure_value_max[] = {225, 317, 410, 502, 594, 686, 778};
const uint16_t pressure_value_min[] = {220, 302, 395, 487, 579, 671, 763};

struct config{
  uint8_t Pmax_index;         ///�������� Pmax
  uint8_t Pmin_index;         ///�������� Pmin
  uint8_t cycle_count;        ///���������� ������
  uint8_t cycle_position;     ///������� � �����
  uint16_t actual_pressure;   ///��������
  uint8_t state;              ///������� ���������
  uint8_t LED_mode; 		  ///���� ���������
  uint8_t position_hand_work;  ///������� � ���� �������
};

#endif // CONFIG_H_INCLUDED