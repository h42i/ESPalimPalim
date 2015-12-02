#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Espanol.h>

#define DEBOUNCE 500

char *ssid     = "";
char *password = "";
char *broker   = "";
int port       = 1883;

int pin=2;
int bounce_counter=0;

Espanol denada(ssid, password, broker, port, NULL);

void setup()
{
    pinMode(pin, INPUT_PULLUP);
}

void loop()
{
    denada.loop();
    
    int current=digitalRead(pin);
    if(current==LOW){
        if(bounce_counter==0){
            denada.publish("hasi/door/upstairs/bell", "on");
        }
        bounce_counter=DEBOUNCE;
    }
    else if(bounce_counter>0){
        bounce_counter--;
    }
    if(bounce_counter==1){
        if(current==HIGH){
            denada.publish("hasi/door/upstairs/bell", "off");
        }
    }
}
