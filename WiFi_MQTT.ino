/*
  WiFi_MQTT.ino
  WiFi connection to MQTT broker to publish sensor values
  Steven G Lamers 2025 - All Rights Reserved
  Uses the ArduinoMqttClient library
*/

#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h"

// #define DEBUG_MONITOR 1

char ssid[] = SECRET_SSID;  // your network SSID (name)
char pass[] = SECRET_PASS;  // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "stevenlamers.com";
int port = 1883;

const long interval = 10000;  // 10 seconds
unsigned long previousMillis = 0;

int count = 0;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT); 
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

#if DEBUG_MONITOR

  Serial.begin(9600);  //Initialize serial and wait for port to open
 
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
 
  // connect to WiFi network 
  Serial.print("Attempting to connect to WPA SSID: ");
  
  Serial.println(ssid);

#endif
 
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {

#if DEBUG_MONITOR
    Serial.print(".");
#endif
        // failed, retry
    delay(5000);
  }

#if DEBUG_MONITOR
  Serial.println("Connected to the network");
  Serial.println();
#endif

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  // mqttClient.setUsernamePassword("username", "password");

#if DEBUG_MONITOR
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
#endif

  if (!mqttClient.connect(broker, port)) {

#ifdef DEBUG_MONITOR
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
#endif

    while (1)
      ;
  }
     
#if DEBUG_MONITOR
  Serial.println("Connected to the MQTT broker!");
  Serial.println();
#endif

}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
   
    // save the last time a message was sent
    previousMillis = currentMillis;

    mqttClient.poll();

#if DEBUG_MONITOR
    Serial.print("Sending messages");
#endif

    // Blink the heatbeat LED
    digitalWrite(LED_BUILTIN, HIGH); 

    // message topic "arduino1234/A0"
    mqttClient.beginMessage("arduino1234/A0");
    // mqttClient.print( ( ( analogRead(A0) + 0.5) * 5.00 ) / 1024 ); // Voltage
    mqttClient.print( analogRead(A0) ); // Voltage
    mqttClient.endMessage();

    // message topic "arduino1234/A1"
    mqttClient.beginMessage("arduino1234/A1");
    mqttClient.print(analogRead(A1));
    mqttClient.endMessage();

    // message topic "arduino1234/A2"
    mqttClient.beginMessage("arduino1234/A2");
    mqttClient.print(analogRead(A2));
    mqttClient.endMessage();

    // message topic "arduino1234/A3"
    mqttClient.beginMessage("arduino1234/A3");
    mqttClient.print(analogRead(A3));
    mqttClient.endMessage();

    // message topic "arduino1234/A4"
    mqttClient.beginMessage("arduino1234/A4");
    mqttClient.print(analogRead(A4));
    mqttClient.endMessage();

    // message topic "arduino1234/A5"
    mqttClient.beginMessage("arduino1234/A5");
    mqttClient.print(analogRead(A5));
    mqttClient.endMessage();

    // message topic "arduino1234/A6"
    mqttClient.beginMessage("arduino1234/A6");
    mqttClient.print(analogRead(A6));
    mqttClient.endMessage();

    // message topic "arduino1234/A7"
    mqttClient.beginMessage("arduino1234/A7");
    mqttClient.print(analogRead(A7));
    mqttClient.endMessage();

    /// Digital out
    // message topic "arduino1234/D0" is PIN 2
    mqttClient.beginMessage("arduino1234/D0");
    ( digitalRead(2) == 1) ? mqttClient.print(1): mqttClient.print(0);
    mqttClient.endMessage();

    // message topic "arduino1234/D1" is PIN 3
    mqttClient.beginMessage("arduino1234/D1");
    ( digitalRead(3) == 1) ? mqttClient.print(1): mqttClient.print(0);
    mqttClient.endMessage();

    // message topic "arduino1234/D2" is PIN 4
    mqttClient.beginMessage("arduino1234/D2");
    ( digitalRead(4) == 1) ? mqttClient.print(1): mqttClient.print(0);
    mqttClient.endMessage();

    // message topic "arduino1234/D3" is PIN 5
    mqttClient.beginMessage("arduino1234/D3");
    ( digitalRead(5) == 1) ? mqttClient.print(1): mqttClient.print(0);
    mqttClient.endMessage();

#if DEBUG_MONITOR
    Serial.println();
#endif

      // Blink the heartbeat LED
      digitalWrite(LED_BUILTIN, LOW);
      
      // update digital counter
      count++;

      // Set/Reset digital outputs for next loop           
      digitalWrite(2, count & 0x1 );
      digitalWrite(3, count & 0x2 );
      digitalWrite(4, count & 0x4 );
      digitalWrite(5, count & 0x8 );
   
  }
}
