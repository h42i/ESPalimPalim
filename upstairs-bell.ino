#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Espanol.h>

#define DEBOUNCE_LOW 50
#define DEBOUNCE_HIGH 500

char *ssid     = "";
char *password = "";
char *broker   = "";
int port       = 1883;

int pin=2;
int state=HIGH;
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
    
    if(current!=state){

        if(bounce_counter==1){
            if(current==LOW){
                denada.publish("hasi/door/upstairs/bell", "on");
            }
            else{
                denada.publish("hasi/door/upstairs/bell", "off");
            }
            state=current;
        }

        if(bounce_counter==0){
            if(current==LOW){
              bounce_counter=DEBOUNCE_LOW;
            }
            if(current==HIGH){
              bounce_counter=DEBOUNCE_HIGH;
            }
        }
        else{
            bounce_counter--;
        }
    }
}
