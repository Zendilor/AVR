#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define MAIN_CLK  8000000   ///частота генератора

///************** PORTC **************
#define COMPRESSOR_CONTROL  (1 << 1)    ///PC0 управление компрессором
#define VALVE_CONTROL       (1 << 0)    ///PC1 управление клапаном

#define LED_PROG_ON 		PORTB&=~(1<<1);
#define LED_PROG_OFF		PORTB|= (1<<1);

#define LED_PMIN_ON			PORTB&=~(1<<0); 
#define LED_PMIN_OFF		PORTB|= (1<<0);

#define LED_PMAX_ON			PORTD&=~(1<<7);
#define LED_PMAX_OFF		PORTD|= (1<<7);

#define LED_WORK_ON   		PORTB&=~(1<<2); 
#define LED_WORK_OFF		PORTB|= (1<<2);

#define KEY_PMAX    (1 << 5)    ///PD5 выбор давления
#define KEY_PMIN    (1 << 6)    ///PD6 выбор давления

///************** PORTD **************
#define BUZ         	(1 << 5)    ///PC5 управление буззером
#define KEY_START_STOP  (1 << 6)    ///PB6 стоп11111111111111111111
#define KEY_CYCLE   	(1 << 7)    ///PB7 выбор количества циклов

#define PRESSURE_SENSOR     (1 << 7)    ///ADC7 вход АЦП - измерение давления

const uint16_t Pmax_duration = 15000;   ///длительность цикла высокого давления
const uint16_t Pmin_duration = 15000;   ///длительность цикла низкого давления
const uint16_t Poff_duration = 5000;    ///длительность цикла нулевого давления
const uint16_t BEEP_duration = 500;




#define POFF    0
#define PMIN    1
#define PMAX    2

///чередование циклов давления
const uint8_t cycle_config[] = {PMIN, POFF, PMAX, POFF, PMIN, POFF, PMIN, POFF,
                                PMIN, POFF, PMIN, POFF, PMAX, POFF, PMAX, POFF};

///таблица значений давления ([10, 15, 20, 25, 30, 35, 40], kPa)
///пересчитаная в коды АЦП (P = (0.018 * P + 0.04) * 1024) Vref = Vcc = 5.0 V

//const uint16_t pressure_value[] = {225, 317, 410, 502, 594, 686, 778};
const uint16_t pressure_value[] = {250, 340, 435, 530, 620, 720, 815};
//#define DELTA   55
//#define DELTA_2 65
#define DELTA   10
#define DELTA_2 60

const uint16_t pressure_value_max[] = {225, 317, 410, 502, 594, 686, 778};
const uint16_t pressure_value_min[] = {220, 302, 395, 487, 579, 671, 763};

struct config{
  uint8_t Pmax_index;         ///значение Pmax
  uint8_t Pmin_index;         ///значение Pmin
  uint8_t cycle_count;        ///количиство циклов
  uint8_t cycle_position;     ///позиция в цикле
  uint16_t actual_pressure;   ///давление
  uint8_t state;              ///текущее состояния
  uint8_t LED_mode; 		  ///Вибір індикації
  uint8_t position_hand_work;  ///позиція в ручні програмі
};

#endif // CONFIG_H_INCLUDED