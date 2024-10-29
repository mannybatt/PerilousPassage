



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
Adafruit_MQTT_Subscribe musicTime = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/perilouspassage.housemusic");
Adafruit_MQTT_Publish eyeReset = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/perilouspassage.reseteye");

//MP3 Player
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>
//GhostFace
DFRobotDFPlayerMini myDFPlayer1;
SoftwareSerial mySoftwareSerial_1(3, D2);  //Pins for MP3 Player Serial (RX, TX)      /*********************TX/RX NEEDS CHECKING**********************/
//Background Audio
DFRobotDFPlayerMini myDFPlayer2;
SoftwareSerial mySoftwareSerial_2(D5, D4);  //Pins for MP3 Player Serial (RX, TX)

//FastLED
#include <FastLED.h>
#define LED_PIN D6
#define NUM_LEDS 7
#define BRIGHTNESS 255
#define LED_TYPE WS2811
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 30

//Input-Output
//#define sensor D3
#define sensor A0
#define phone D1

//Variables
int desiredBrightness = 254;
byte r = 0;
float mqttConnectFlag = 0.0;
unsigned long previousMillis = 0;
unsigned long currentMillis;
int numberOfGhostfaceSounds = 3;
int currentGhostfaceSound = 1;
int numberOfPhoneSounds = 7; //Actually 4, but we use 7 because to play from ghostFace speaker
int currentPhoneSound = 4;
int numberOfBGSounds = 1;
int currentBGSound = 1;




// ***************************************
// *************** Setup *****************
// ***************************************


void setup() {

  //Initialize GPIO
  pinMode(phone, INPUT_PULLUP);
  pinMode(sensor, INPUT);

  //Initialize RGB
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 900);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  delay(1000);
  ghostFaceLight(1);

  //Initialize Serial, WiFi, & OTA
  wifiSetup();

  //Initialize MQTT
  mqtt.subscribe(&musicTime);
  MQTT_connect();

  //Initialize MP3 - GhostFace/Phone
  mySoftwareSerial_1.begin(9600);
  delay(1000);
  myDFPlayer1.begin(mySoftwareSerial_1);
  Serial.println("DFPlayer - GhostFace initialized!");
  myDFPlayer1.setTimeOut(500); //Timeout serial 500ms
  myDFPlayer1.volume(0); //Volume 0-30
  myDFPlayer1.EQ(0); //Equalization normal
  delay(250);
  myDFPlayer1.volume(21);
  myDFPlayer1.play(8); //Boot Sound
  delay(2000);
  myDFPlayer1.stop();
  myDFPlayer1.volume(30);

  //Initialize MP3 - Background
  mySoftwareSerial_2.begin(9600);
  delay(1000);
  myDFPlayer2.begin(mySoftwareSerial_2);
  Serial.println("DFPlayer initialized!");
  myDFPlayer2.setTimeOut(500); //Timeout serial 500ms
  myDFPlayer2.volume(0); //Volume 0-30
  myDFPlayer2.EQ(0); //Equalization normal
  delay(250);
  myDFPlayer2.volume(25);
  myDFPlayer2.play(2); //Boot Sound
  delay(2000);
  myDFPlayer2.stop();

  //Setup Complete
  ghostFaceLight(0);
  eyeReset.publish(1);
  Serial.println("Setup Complete!");

}




// ***************************************
// ************* Da Loop *****************
// ***************************************


void loop() {

  //OTA & MQTT
  ArduinoOTA.handle();
  MQTT_connect();

  //**** The Great Spoop Loop!! ****//

  //Check for MQTT from Sid's Porch that starts the background music
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(0.01))) {
    uint16_t valueOperational = atoi((char *)musicTime.lastread);
    Serial.println(valueOperational);

    //Activate background music when people enter the porch
    if (valueOperational == 1) {
      playHallwayMusic();
    }
    delay(10);
  }

  //Check for phone lift
  delay(10);
  int phoneRead = digitalRead(phone);
  if (phoneRead == LOW) {
    delay(100);
    int phoneRead2 = digitalRead(phone);
    if (phoneRead2 == LOW) {
      delay(100);
      activatePhoneMode();
    }
  }
  delay(10);

  //Check for Ghostface Sensor
  int read1 = analogRead(sensor);
  Serial.println(read1);
  if (analogRead(sensor) > 700) {
    delay(100);
    int sensorReading = analogRead(sensor);
    if (sensorReading > 700) {
      delay(100);
      int sensorReading2 = analogRead(sensor);
      if (sensorReading2 > 700) {
        delay(100);
        int sensorReading3 = analogRead(sensor);
        if (sensorReading3 > 700) {
          delay(10);
          activateGhostFace();
          delay(100);
          read1 = analogRead(sensor);
          delay(100);
        }
      }
    }
  }

  //Ping Timer for keeping MQTT Publish alive
  unsigned long currentTime = millis();
  if ((currentTime - previousTime) > MQTT_KEEP_ALIVE * 1000) {
    previousTime = currentTime;
    if (! mqtt.ping()) {
      mqtt.disconnect();
    }
  }

  delay(100);
}




// ***************************************
// ********** Backbone Methods ***********
// ***************************************


void playHallwayMusic() {

  delay(10);
  currentBGSound++;
  if (currentBGSound > numberOfBGSounds) {
    currentBGSound = 1;
  }
  myDFPlayer2.play(1);
  Serial.print("House Bacground song ");
  Serial.println(currentBGSound);
  delay(100);
}

void activateGhostFace() {

  //Sound plays first
  delay(10);
  currentGhostfaceSound++;
  if (currentGhostfaceSound > numberOfGhostfaceSounds) {
    currentGhostfaceSound = 1;
  }
  myDFPlayer1.play(currentGhostfaceSound);
  Serial.print("Ghostface song ");
  Serial.println(currentGhostfaceSound);
  delay(100);

  //Lights Happen Shortly After
  ghostFaceLight(1);
  delay(7000);

  //Dim Lights
  ghostFaceLight(0);
  delay(1000);
  eyeReset.publish(1);
}

void activatePhoneMode() {

  delay(10);
  currentPhoneSound++;
  if (currentPhoneSound > numberOfPhoneSounds) {
    currentPhoneSound = 4;
  }
  myDFPlayer1.play(currentPhoneSound);
  Serial.print("Phone song ");
  Serial.println(currentPhoneSound);
  delay(5000);

  int in = digitalRead(phone);
  Serial.print("phone status:  ");
  Serial.println(in);
  while (in == 0) {
    delay(250);
    in = digitalRead(phone);
    Serial.print("phone status:  ");
    Serial.println(in);
  }
  myDFPlayer1.stop();

}

void ghostFaceLight(int mode) {

  //This is hard coded to prevent led timing weirdness with how far lights are from brain
  if (mode == 1) {
    setPixel(0, 255, 0, 0);
    setPixel(1, 255, 0, 0);
    setPixel(2, 255, 0, 0);
    setPixel(3, 255, 0, 0);
    setPixel(4, 255, 0, 0);
    setPixel(5, 255, 0, 0);
    setPixel(6, 255, 0, 0);
    delay(7);
    FastLED.show();
    FastLED.show();
  }
  else if (mode == 0) {
    //for (int i = 0; i < 7; i++) {
    setPixel(1, 0, 0, 0);
    setPixel(2, 0, 0, 0);
    setPixel(3, 0, 0, 0);
    setPixel(4, 0, 0, 0);
    setPixel(5, 0, 0, 0);
    setPixel(6, 0, 0, 0);
    setPixel(0, 0, 0, 0);
    delay(7);
    FastLED.show();
    FastLED.show();
  }
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
}

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
  delay(200);
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
  ArduinoOTA.setHostname("PerilousPassage_House");
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
