#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Espanol.h>

#define DEBOUNCE_LOW 50
#define DEBOUNCE_HIGH 500

const String ssid     = "HaSi-Kein-Internet-Legacy";
const String password = "";
const String broker   = "atuin";
int port              = 1883;

int pin = 2;
int state = HIGH;
int bounce_counter = 0;

void setup()
{
    Serial.begin(115200);

    pinMode(pin, INPUT_PULLUP);

    Espanol.setDebug(true);
    Espanol.begin(ssid, password, "espalimpalim", broker, port);
}

void loop()
{
    Espanol.loop();

    int current = digitalRead(pin);

    if (current != state)
    {
        if (bounce_counter == 1)
        {
            if (current == LOW)
            {
                Espanol.publish("hasi/door/upstairs/bell", "on");
            }
            else
            {
                Espanol.publish("hasi/door/upstairs/bell", "off");
            }

            state = current;
        }

        if (bounce_counter == 0)
        {
            if (current == LOW)
            {
                bounce_counter = DEBOUNCE_LOW;
            }

            if (current == HIGH)
            {
                bounce_counter = DEBOUNCE_HIGH;
            }
        }
        else
        {
            bounce_counter--;
        }
    }
}
