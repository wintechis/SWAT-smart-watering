#include <Arduino.h>
#include <WiFi.h>

/*
void setup() {
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
}

void loop() {
  digitalWrite(relay, HIGH);
  Serial.println("All relays ON");
  delay(5000);

  digitalWrite(relay, LOW);
  Serial.println("All relays OFF");
  delay(1000);
}
*/

/*
 WiFi Web Server LED Blink
 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.
 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off
 This example is written for a network using WPA2 encryption. For insecure
 WEP or WPA, change the Wifi.begin() call and use Wifi.setMinSecurity() accordingly.
 Circuit:
 * WiFi shield attached
 * LED attached to pin 5
 created for arduino 25 Nov 2012
 by Tom Igoe
ported for sparkfun esp32 
31.01.2017 by Jan Hendrik Berlin
 
 */


const char* ssid     = "ASUS-prodab";
const char* password = "prodab-pw";
int relay1_1 = 12;
int relay1_2 = 14;
int relay2_1 = 13;
int relay2_2 = 15;

WiFiServer server(80);

void setup()
{
    Serial.begin(9600);
    pinMode(5, OUTPUT);      // set the LED pin mode

    pinMode(relay1_1, OUTPUT);
    pinMode(relay1_2, OUTPUT);
    pinMode(relay2_1, OUTPUT);
    pinMode(relay2_2, OUTPUT);

    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;
bool power = true;
bool direction = true;

void loop(){
  if(power && direction) {
    digitalWrite(relay2_1, LOW);
    digitalWrite(relay2_2, LOW);
    delay(200);
    digitalWrite(relay1_1, HIGH);
    digitalWrite(relay1_2, HIGH);
  } else if(power && !direction) {
    digitalWrite(relay1_1, LOW);
    digitalWrite(relay1_2, LOW);
    delay(200);
    digitalWrite(relay2_1, HIGH);
    digitalWrite(relay2_2, HIGH);
  } else {
    digitalWrite(relay1_1, LOW);
    digitalWrite(relay1_2, LOW);
    digitalWrite(relay2_1, LOW);
    digitalWrite(relay2_2, LOW);

  }

 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    String body = "";
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/plain");
            client.println();
            client.println(body);

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /properties/power")) {
          if(power) {
            body = "true";
          } else {
            body = "false";
          }
        }
        if (currentLine.endsWith("PUT /properties/power")) {
          String body = client.readString();
          if(body.endsWith("true")) {
            Serial.println("PUT power True");
            power = true;
            client.println("HTTP/1.1 204 No Content");
            client.println();
          } else if(body.endsWith("false")) {
            Serial.println("PUT power false");
            power = false;
            client.println("HTTP/1.1 204 No Content");
            client.println();
          } else {
            Serial.println("Bad Request power");
            client.println("HTTP/1.1 400 Bad Request");
            client.println();
            client.println();
          }
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /properties/direction")) {
          if(direction) {
            body = "true";
          } else {
            body = "false";
          }
        }
        if (currentLine.endsWith("PUT /properties/direction")) {
          String body = client.readString();
          if(body.endsWith("true")) {
            Serial.println("PUT direction True");
            direction = true;
            client.println("HTTP/1.1 204 No Content");
            client.println();
          } else if(body.endsWith("false")) {
            Serial.println("PUT direction false");
            direction = false;
            client.println("HTTP/1.1 204 No Content");
            client.println();
          } else {
            Serial.println("Bad Request power");
            client.println("HTTP/1.1 400 Bad Request");
            client.println();
            client.println();
          }
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}