



// ***************************************
// ********** Global Variables ***********
// ***************************************


//Globals for Wifi Setup and OTA
#include <credentials.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//WiFi Credentials
#ifndef STASSID
#define STASSID "your_ssid"
#endif
#ifndef STAPSK
#define STAPSK  "your_password"
#endif
const char* ssid = STASSID;
const char* password = STAPSK;

//MQTT
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#ifndef AIO_SERVER
#define AIO_SERVER      "your_MQTT_server_address"
#endif
#ifndef AIO_SERVERPORT
#define AIO_SERVERPORT  0000 //Your MQTT port
#endif
#ifndef AIO_USERNAME
#define AIO_USERNAME    "your_MQTT_username"
#endif
#ifndef AIO_KEY
#define AIO_KEY         "your_MQTT_key"
#endif
#define MQTT_KEEP_ALIVE 150
unsigned long previousTime;

//Initialize and Subscribe to MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe fog = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/perilouspassage.fogcontrol");

//Input & Output
#define offRelay D7
#define onRelay D3
#define unused1 D5
#define unused2 D6

//Variables
int fogDuration = 3000;
float mqttConnectFlag = 0.0;




// ***************************************
// *************** Setup *****************
// ***************************************


void setup() {

  //Relay Setup
  pinMode(offRelay, OUTPUT);
  pinMode(onRelay, OUTPUT);
  pinMode(unused1, OUTPUT);
  pinMode(unused2, OUTPUT);
  digitalWrite(offRelay, HIGH);
  digitalWrite(onRelay, HIGH);
  digitalWrite(unused1, HIGH);
  digitalWrite(unused2, HIGH);

  //Initialize Serial, WiFi, & OTA
  wifiSetup();

  //Initialize MQTT
  mqtt.subscribe(&fog);

  //All set
  delay(1000);
  Serial.println("Setup Complete!");
}




// ***************************************
// ************* Da Loop *****************
// ***************************************


void loop() {

  //OTA & MQTT
  ArduinoOTA.handle();
  MQTT_connect();

  //Recieve MQTT
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(0.01))) {
    uint16_t valueOperational = atoi((char *)fog.lastread);
    Serial.println(valueOperational);

    //Forever On
    if (valueOperational == 1) {
       digitalWrite(onRelay, LOW);
       delay(400);
       digitalWrite(onRelay, HIGH);
    }
    //Forever Off
    else if (valueOperational == 2) {
      digitalWrite(offRelay, LOW);
      delay(400);
      digitalWrite(offRelay, HIGH);
    }
    //Release timed amount of Fog for Sid's house
    else if (valueOperational == 3){
      digitalWrite(onRelay, LOW);
      delay(400);
      digitalWrite(onRelay, HIGH);
      delay(fogDuration);
      digitalWrite(offRelay, LOW);
      delay(400);
      digitalWrite(offRelay, HIGH);
    }
    delay(10);
  }
  delay(10);
}




// ***************************************
// ********** Backbone Methods ***********
// ***************************************


void MQTT_connect() {

  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    if (mqttConnectFlag == 0) {
      //Serial.println("Connected");
      mqttConnectFlag++;
    }
    return;
  }
  Serial.println("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      //while (1);
      Serial.println("Wait 5 secomds to reconnect");
      delay(5000);
    }
  }
}

void wifiSetup() {

  //Serial
  Serial.begin(115200);
  delay(300);
  Serial.println();
  Serial.println();
  Serial.println("****************************************");
  Serial.println("Booting");

  //WiFi and OTA
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.setHostname("PerilousPassage-FogControl");                                                          /** TODO **/
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
