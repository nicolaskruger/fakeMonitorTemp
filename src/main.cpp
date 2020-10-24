#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "Sensor.h"

#define AdSize 2
#define LED 2

SemaphoreHandle_t mutexSerial ;
SemaphoreHandle_t mutexAd[2] ;
int adRead[] = {A0,A1};

Sensor* s1;
Sensor* s2;
Sensor** s[]={&s1,&s2};


void func(void * f);
void funcS1(void* f);
void funcS2(void* f);
void (*funcS[])(void*)={&funcS1,&funcS2};
void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  mutexSerial = xSemaphoreCreateMutex();
  xSemaphoreGive(mutexSerial);
  for (size_t i = 0; i < AdSize; i++)
  {
    mutexAd[i] = xSemaphoreCreateMutex();
    xSemaphoreGive(mutexAd[i]);
    *s[i] = new Sensor(adRead[i],&mutexAd[i]);
    xTaskCreate(funcS[i], // Task function
              "Notification", // Task name
              128, // Stack size
              NULL,
              3, // Priority
              NULL ); // TaskHandle
  }
  
  xTaskCreate(func, // Task function
              "Notification", // Task name
              128, // Stack size
              NULL,
              3, // Priority
              NULL ); // TaskHandle
  
}

void loop() {

}
void func(void* f){
  while (true)
  {
    if( xSemaphoreTake(mutexSerial,(TickType_t)5)==pdTRUE){
      Serial.print("temp 1: ");
      Serial.print(s1->getMedia());
      Serial.print(" temp 2: ");
      Serial.println(s2->getMedia());
      xSemaphoreGive(mutexSerial);
      if(s2->diff(s1)>=10){
        digitalWrite(LED,HIGH);
      }else{
        digitalWrite(LED,LOW);
      }
    }
    vTaskDelay(100);
  }
}
void funcS1(void *f){
  s1->getVal();
}
void funcS2(void* f){
  s2->getVal();
}
