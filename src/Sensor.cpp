#include "Sensor.h"

Sensor::Sensor(int ad,SemaphoreHandle_t* mutex)
{
    this->ad=ad;
    this->mutex= mutex;
}

Sensor::~Sensor()
{
}

void Sensor::getVal(){
    int i =0;
    while (1)
    {
        if( xSemaphoreTake(*mutex,(TickType_t)5)==pdTRUE){
          vet[i] = (analogRead(ad)*100.00)/1023.00;
          xSemaphoreGive(*mutex);
        }
        vTaskDelay(100);
        i=(i+1)%vetSize;
    }
    
}
float Sensor::getMedia(){
    float media =0;
    if( xSemaphoreTake(*mutex,(TickType_t)5)==pdTRUE){
        for (size_t i = 0; i < vetSize; i++)
        {
            media+= vet[i];
        }
        xSemaphoreGive(*mutex);
    }
    return media/vetSize;
}
float Sensor::diff(Sensor* ss){
    float dif = ss->getMedia()-this->getMedia();
    return dif>0?dif:-dif;
}