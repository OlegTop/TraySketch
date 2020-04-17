//
// Created by ad on 26.03.2020.
//

#ifndef AI_FRESH_01_CONST_H
#define AI_FRESH_01_CONST_H 

/*Скорость обмена COM1*/            const int speed_com1 = 115200;
/*Кол-во тактов в паузе*/           const short int delay_size = 4;

/*Кол-во адресных диодов на шине*/  const int LED_size = 20;
/*Пин LED-шины*/                    const byte LED_pin = 16;

/*vSPI 1кГц*/                       static const int speed_spi = 1000; SPIClass * vspi = NULL; const byte SS_d = 13;   

/*Пин LED-status*/                  const byte L_status = 21;
/*Пин LED-update*/                  const byte L_update = 22;

/*Пин входного вольтажа*/           const byte V_in = 34;
/*Пин стабилизированого вольтажа*/  const byte V_stable = 36;

/*Пин ADC-1 вольтажа*/              const byte V_adc_1 = 39;
/*Пин A-17 вольтажа*/               const byte Va_17 = 32;
/*Пин A-18 вольтажа*/               const byte Va_18 = 33;
/*Пин A-19 вольтажа*/               const byte Va_19 = 26;
/*Пин A-20 вольтажа*/               const byte Va_20 = 27;

/*Отображаемое имя устройства*/     const String name_point = "AI-Fresh";
/*Версия прошивки*/                 const String ver_modul = "01";


#endif //AI_FRESH_01_CONST_H
