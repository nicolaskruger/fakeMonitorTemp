#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <Arduino.h>
#define vetSize 10
class Sensor
{
private:
    /* data */
    
    float vet[vetSize] = {0,0,0,0,0,0,0,0,0,0};
    SemaphoreHandle_t* mutex;
    int ad;
public:
    Sensor(int ad,SemaphoreHandle_t* mutex);
    ~Sensor();
    void getVal();
    float getMedia();
    float diff(Sensor *ss);
};


