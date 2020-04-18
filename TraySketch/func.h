//
// Created by ad on 26.03.2020.
//

#ifndef AI_FRESH_01_FUNC_H
#define AI_FRESH_01_FUNC_H

#include "const.h"
#include "values.h"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LED_size, LED_pin, NEO_GRB + NEO_KHZ800);

//Задерка с тактовым множителем
void fpause(int in){
    vTaskDelay(in*delay_size);
}

//Конвертация uint64 в String
String uint64_to_string(uint64_t input) {
    String result = "";
    uint8_t base = 10;

    do {
        char c = input % base;
        input /= base;

        if (c < 10)
            c +='0';
        else
            c += 'A' - 10;
        result = c + result;
    } while (input);
    return result;
}

//Получение подстроки из строки по разбиению 
String getValue(String d, char se, int in){
  int f=0;
  int s[]={0,-1};
  int ma=d.length()-1;
  for(int i=0;i<=ma&&f<=in;i++){
    if(d.charAt(i)==se || i==ma){
      f++;
      s[0]=s[1]+1;
      s[1]=(i==ma)?i+1:i;
      }
      }
      return f>in?d.substring(s[0], s[1]):"";
 }
 
//Запись данных в регистры
void req_write(byte data1, byte data2, byte data3) {

    vspi->beginTransaction(SPISettings(speed_spi, MSBFIRST, SPI_MODE0));
    digitalWrite(SS_d, LOW);   
    vspi->transfer(data1);
    vspi->transfer(data2);
    vspi->transfer(data3);
    digitalWrite(SS_d, HIGH);
    vspi->endTransaction();
}

//Чтение аналоговых пинов
float input_pin_read(byte pin){
  float v_ret = -4;

  float volt_const = 3.3/(analogRead(V_stable));
  v_ret = (analogRead(pin))*volt_const;
  if(v_ret > 0.15) v_ret += 0.15;

  return v_ret;
  }

//Генерация сообщения привествия
String hello_msg(){
      String ret_msg = "\n\n";

    ret_msg += name_point+" ver:"+ver_modul+"\n";
    ret_msg += "\n";
    ret_msg += "MAC: "+uint64_to_string(ESP.getEfuseMac())+"\n";
    ret_msg += "DRAM: "+String(ESP.getFreeHeap())+"\n";
    ret_msg += "Speed COM:"+String(speed_com1)+"\n";

    float input_v_in = input_pin_read(V_in);
    input_v_in *= 2;

    if(input_v_in > 5.4){
        ret_msg += "[ERROR]{"+String(input_v_in)+"} Превышение внешнего вольтажа!!!\n";
        type_load = 44;
    }else if(input_v_in < 4.2){
        ret_msg += "[ERROR]{"+String(input_v_in)+"} Внешний вольтаж слишком низкий!!!\n";
        type_load = 45;
    }

    ret_msg += "Volt input: "+String(input_v_in)+" V\n";

    ret_msg += "\n[TYPE LOAD] "+String(type_load)+"\n\n";

      return ret_msg;
  }

//Сканированиие Wi-Fi
String wifi_scan(){

    WiFi.disconnect();

    int n = WiFi.scanNetworks();
    String ret_arr = "-1";

    if (n == 0) {
        ret_arr = "0;";

    } else {
        ret_arr = "1;"+String(n);

        for (int i = 0; i < n; ++i) {
            ret_arr += ";"+String(i)+",";
            ret_arr += String(WiFi.SSID(i))+",";
            ret_arr += String(WiFi.RSSI(i))+",";
            ret_arr += String(WiFi.encryptionType(i));
        }
    }

    return ret_arr;
}

//Подключение к точке доступа
void ConnectToWiFi()
{

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, WiFiPassword);
    Serial.print("Connecting to "); Serial.println(SSID);

    uint8_t i = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(500);

        if ((++i % 16) == 0)
        {
            Serial.println(F(" still trying to connect"));
        }
    }
    Serial.println();
    Serial.print(F("Connected. My IP address is: "));
    Serial.println(WiFi.localIP());
}

//Установка регистров в определенную позицию
void set_req_pos(){

    byte set_byte1 = (byte)((req_out[7] << 0)
                    | (req_out[6] << 1)
                    | (req_out[5] << 2)
                    | (req_out[4] << 3)
                    | (req_out[3] << 4)
                    | (req_out[2] << 5)
                    | (req_out[1] << 6)
                    | (req_out[0] << 7));
    byte set_byte2 = (byte)((req_out[15] << 0)
                    | (req_out[14] << 1)
                    | (req_out[13] << 2)
                    | (req_out[12] << 3)
                    | (req_out[11] << 4)
                    | (req_out[10] << 5)
                    | (req_out[9] << 6)
                    | (req_out[8] << 7));
    byte set_byte3 = (byte)((req_out[23] << 0)
                    | (req_out[22] << 1)
                    | (req_out[21] << 2)
                    | (req_out[20] << 3)
                    | (req_out[19] << 4)
                    | (req_out[18] << 5)
                    | (req_out[17] << 6)
                    | (req_out[16] << 7));

    byte buf_byte1 = (byte)((buf_req_out[7] << 0)
                    | (buf_req_out[6] << 1)
                    | (buf_req_out[5] << 2)
                    | (buf_req_out[4] << 3)
                    | (buf_req_out[3] << 4)
                    | (buf_req_out[2] << 5)
                    | (buf_req_out[1] << 6)
                    | (buf_req_out[0] << 7));
    byte buf_byte2 = (byte)((buf_req_out[15] << 0)
                    | (buf_req_out[14] << 1)
                    | (buf_req_out[13] << 2)
                    | (buf_req_out[12] << 3)
                    | (buf_req_out[11] << 4)
                    | (buf_req_out[10] << 5)
                    | (buf_req_out[9] << 6)
                    | (buf_req_out[8] << 7));
    byte buf_byte3 = (byte)((buf_req_out[23] << 0)
                    | (buf_req_out[22] << 1)
                    | (buf_req_out[21] << 2)
                    | (buf_req_out[20] << 3)
                    | (buf_req_out[19] << 4)
                    | (buf_req_out[18] << 5)
                    | (buf_req_out[17] << 6)
                    | (buf_req_out[16] << 7));         

    if(set_byte1 != buf_byte1 || set_byte2 != buf_byte2 || set_byte3 != buf_byte3){
        req_write(set_byte1, set_byte2, set_byte3);
        for (int i = 0; i < 24; ++i) buf_req_out[i] = req_out[i];
    }

}

//Чтение входов аналоговой матрицы
void adc_read(){

unsigned char mpl_cnl[16][4]={{0,0,0,0},{1,0,0,0},{0,1,0,0},{1,1,0,0},{0,0,1,0},{1,0,1,0},{0,1,1,0},{1,1,1,0},{0,0,0,1},{1,0,0,1},{0,1,0,1},{1,1,0,1},{0,0,1,1},{1,0,1,1},{0,1,1,1},{1,1,1,1}};
float buf_adc_v_in[16];

for(int i=0; i<16; i++){

  req_out[23] = mpl_cnl[i][0];
  req_out[22] = mpl_cnl[i][1];
  req_out[21] = mpl_cnl[i][2];
  req_out[20] = mpl_cnl[i][3];
  set_req_pos();
  fpause(4);
  buf_adc_v_in[i] = input_pin_read(V_adc_1);
  
}


  adc_v_in[0] = buf_adc_v_in[15];
  adc_v_in[1] = buf_adc_v_in[14];
  adc_v_in[2] = buf_adc_v_in[13];
  adc_v_in[3] = buf_adc_v_in[12];
  adc_v_in[4] = buf_adc_v_in[11];
  adc_v_in[5] = buf_adc_v_in[6];
  adc_v_in[6] = buf_adc_v_in[7];
  adc_v_in[7] = buf_adc_v_in[8];
  adc_v_in[8] = buf_adc_v_in[9];
  adc_v_in[9] = buf_adc_v_in[10];
  adc_v_in[10] = buf_adc_v_in[4];
  adc_v_in[11] = buf_adc_v_in[3];
  adc_v_in[12] = buf_adc_v_in[2];
  adc_v_in[13] = buf_adc_v_in[1];
  adc_v_in[14] = buf_adc_v_in[0];
  adc_v_in[15] = buf_adc_v_in[5];  
  adc_v_in[16] = input_pin_read(Va_17);
  adc_v_in[17] = input_pin_read(Va_18);
  adc_v_in[18] = input_pin_read(Va_19);
  adc_v_in[19] = input_pin_read(Va_20); 

  for(int i = 0; i < 20; i++){
    int color_a = adc_v_in[i]*44.44+1;
    pixels.setPixelColor(i, pixels.Color(led_stat[i][0], led_stat[i][1], color_a)); 
  }
  pixels.show(); 
}

//Установка состояний реле
void set_relay(int num_relay, int stat){

  int s_rele[20] = {19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};

  if(stat > 1) stat = 1;
  if(stat < 0) stat = 0;

  if(num_relay < 1) num_relay = 0;
  if(num_relay > 20) num_relay = 20;

  if(stat == 1){
    req_out[s_rele[num_relay-1]] = 1;
    led_stat[num_relay-1][0] = 255;
    Serial.println("Relay-"+String(num_relay)+": ON");
    
  }else{
    led_stat[num_relay-1][0] = 0;
    req_out[s_rele[num_relay-1]] = 0;
    Serial.println("Relay-"+String(num_relay)+": OFF");
  }

  

}

//Установка состояний индикаторов
void set_matrix_led(int num_led, int stat){
    if(stat > 1) stat = 1;
  if(stat < 0) stat = 0;

  if(num_led < 1) num_led = 0;
  if(num_led > 20) num_led = 20;

    if(stat == 1){
    led_stat[num_led-1][1] = 255;
    Serial.println("LED-"+String(num_led)+": ON");
    
  }else{
    led_stat[num_led-1][1] = 0;
    Serial.println("LED-"+String(num_led)+": OFF");
  }
}

#endif //AI_FRESH_01_FUNC_H
