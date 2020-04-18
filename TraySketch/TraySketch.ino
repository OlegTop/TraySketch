/*
 *
board: ESP32 Dev Module
speed: 115200
CPU: 240MHz
Flash frq: 80MHz
Flash mode: QIO
Flash size: 4MB
Partition: Minimal SPIFFS (19MB APP with OTA; 190KB SPIFFS)
PSRAM: Disabled
 *
 * */

#include <SPI.h>                //либа для работы срегистрами, подробнее гугл
#include <WiFi.h>               //я тебе заодно нормальные функции поиска точек и подключния сделал (wifi_scan, wifi_connect), места не занимают, зато избвят от костылей, всегда можешь потом удалить
#include <Adafruit_NeoPixel.h>  //либа для управления адресной лентой
#include "func.h"               // отдельный файл со всеми функциями (все к слову в функциональном стиле, никакого ООП)
#include <HTTPClient.h>


//ниже конфигуровщик FreeRTOS, точнее ESP32 на уровне железа его поддерживает но ето лучше писать, это интерпритатор с ардуиной
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

//тут декларируем с какими функциями будет работать планировщик
void TaskUpdateParam( void *pvParameters );
void TaskLoop( void *pvParameters );

void setup() {

    xTaskCreatePinnedToCore( TaskUpdateParam,  "TaskButtoms",  2048,  NULL,  1,  NULL,  ARDUINO_RUNNING_CORE);  //ето не трогать, вообще не трогать. даже если сильно хочется
    xTaskCreatePinnedToCore( TaskLoop,  "TaskCase",  15084,  NULL,  2,  NULL,  ARDUINO_RUNNING_CORE);           //тут можешь с памятью играть (15084) но в целом тебе етого хватит трогать ничего не надо
    
}

void loop() {}//забудь про это

//функци ниже так же не для тебя. не обращай внимания. скажем так это системный уровень ядра. вся та магия что сама в фоне потоково считает весь вольтаж, клацает клапанами и переключает ленту
void TaskUpdateParam(void *pvParameters) {
    (void) pvParameters;

    vspi = new SPIClass(VSPI);
    vspi->begin();
    pixels.begin();

    pinMode(SS_d, OUTPUT);
    
    pinMode(L_update, OUTPUT);
    pinMode(V_in, INPUT);
    pinMode(V_stable, INPUT);

    digitalWrite(L_update, LOW);
 
    for (;;) {
fpause(44); digitalWrite(L_update, HIGH);
adc_read();
fpause(44); digitalWrite(L_update, LOW);
      }
}




//тебе сюда
void TaskLoop(void *pvParameters) {
    (void) pvParameters;

    Serial.begin(speed_com1, SERIAL_8N1);   //инициализация консоли
    pinMode(L_status, OUTPUT);              //индикатор на плате который загорается после окончания загрузки доски
    
    Serial.println(hello_msg());            //скажем так, приветствие с простейшим прелоадером системы и банальными проверками. все в консоль. можно удалить

    if(type_load > 40) {Serial.println("\n\nGo to [SlEEP MODE]\n\n");delay(44);esp_deep_sleep_start();} //защита от кривых рук или краша. если полетел блок питания или воткнул кривой блок питания то ESP32 уходит в глубокий сон
    digitalWrite(L_status, HIGH);



    String buf_rs_console = "";           //буфер хранения текста с консоли

    for (;;) {
        int pots_amount = 20;
        HTTPClient http;
        if(WiFi.status() != WL_CONNECTED){
        Serial.println("No connection!");
        ConnectToWiFi();
    }else {

            http.begin("vkram.shpp.me:5000/tray_size?token=413edd445c7e487da52ceef017071e69&tray_id=0"); //Specify the URL
            int httpCode = http.GET();//Make the request

            if (httpCode > 0) { //Check for the returning code
                String payload = http.getString();
                http.end(); //Free the resources
                Serial.println(payload);
                pots_amount= payload.toInt();
                Serial.print("Amount of pots = ");
                Serial.println(pots_amount);
            }
            else {
                Serial.println("Error on HTTP request");
            }
    }
    for(int i = 1; i<=pots_amount;i++){


    }
    fpause(4);}//не трожь меня

