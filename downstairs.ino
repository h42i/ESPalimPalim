#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Espanol.h>

#define DEBOUNCE 500

char *ssid     = "";
char *password = "";
char *broker   = "atlas";
int port       = 1883;

int door_pin=14;
int bell_pin=4;
int last=LOW;
int bounce_counter=0;

Espanol denada(ssid, password, broker, port, callback);

int lock=0;

void callback(char* topic, byte* payload, unsigned int length)
{
  if(lock==0){
    int time=0;
    if(payload[0]>48 && payload[0]<58){
        time=payload[0]-48;
        if(time>9){
          time=9;
        }
    }
    
    lock=1;
    
    digitalWrite(door_pin, HIGH);
    delay(time*1000);
    digitalWrite(door_pin, LOW);

    delay(10000);
    lock=0;
  }
}

void setup()
{
    pinMode(bell_pin, INPUT_PULLUP);
    pinMode(door_pin, OUTPUT);
    denada.subscribe("hasi/door/downstairs");
}

void loop()
{
    denada.loop();
    
    char *topic="hasi/door/downstairs/bell";

    int current=digitalRead(bell_pin);
    if(current!=last){
        bounce_counter=DEBOUNCE;
    }
    else if(bounce_counter>0){
        bounce_counter--;
    }
    if(bounce_counter==1){
        if(current==HIGH){
            denada.publish(topic, "on");
        }
        else{
            denada.publish(topic, "off");
        }
    }
    last=current;
}
