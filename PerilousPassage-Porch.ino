



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

//FastLED
#include <FastLED.h>
#define LED_PIN     D3
#define NUM_LEDS    1
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100

//MP3 Player
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>
DFRobotDFPlayerMini myDFPlayer;
SoftwareSerial mySoftwareSerial(D4, D2);  //Pins for MP3 Player Serial (RX, TX)

DFRobotDFPlayerMini myDFPlayer2;
SoftwareSerial mySoftwareSerial2(D0, D8);  //Pins for MP3 Player Serial (RX, TX)

//MQTT//MQTT
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
Adafruit_MQTT_Subscribe eyeReset = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/perilouspassage.reseteye");
Adafruit_MQTT_Publish houseMusic = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/perilouspassage.housemusic");
Adafruit_MQTT_Publish fogControl = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/perilouspassage.fogcontrol");
Adafruit_MQTT_Publish mechControl = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/perilouspassage.mechcontrol");

//Input-Output
#define sensor D7
#define doorbell D5
#define eyeball D6
#define speakerRelay D1

//Variables
float mqttConnectFlag = 0.0;
int numberOfPorchSounds = 2;
int currentPorchSound = 1;
int numberOfDoorbellSounds = 3;
int currentDoorbellSound = 1;
unsigned long previousMillis = 0;
unsigned long currentMillis;
unsigned long previousMusicMillis = 0;
unsigned long currentMusicMillis;
int isSongPlaying = 0;




// ***************************************
// *************** Setup *****************
// ***************************************


void setup() {

  //Initialize Doorbell
  pinMode(doorbell, INPUT_PULLUP );
  pinMode(speakerRelay, OUTPUT);
  digitalWrite(speakerRelay, HIGH);

  //Initialize RGB
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 800);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  delay(10);
  setPixel(0, 180, 0, 255);
  delay(10);
  FastLED.show();
  FastLED.show();

  //Initialize Serial, WiFi, & OTA
  wifiSetup();

  //Initialize MQTT
  mqtt.subscribe(&eyeReset);
  MQTT_connect();

  //Initialize MP3 - Ghost Face Animatronic
  mySoftwareSerial.begin(9600);
  delay(1000);
  myDFPlayer.begin(mySoftwareSerial);
  Serial.println("DFPlayer initialized!");
  myDFPlayer.setTimeOut(500); //Timeout serial 500ms
  myDFPlayer.volume(0); //Volume 0-30
  myDFPlayer.EQ(0); //Equalization normal
  delay(250);
  myDFPlayer.volume(28);
  myDFPlayer.play(4); //Boot Sound
  delay(2000);
  myDFPlayer.stop();

  //Change Background audio speaker to AUX IN
  digitalWrite(speakerRelay, LOW);
  delay(250);
  digitalWrite(speakerRelay, HIGH);
  delay(250);

  //Initialize MP3 - Background Audio
  mySoftwareSerial2.begin(9600);
  delay(1000);
  myDFPlayer2.begin(mySoftwareSerial2);
  Serial.println("DFPlayer initialized!");
  myDFPlayer2.setTimeOut(500); //Timeout serial 500ms
  myDFPlayer2.volume(0); //Volume 0-30
  myDFPlayer2.EQ(0); //Equalization normal
  delay(250);
  myDFPlayer2.volume(25);
  myDFPlayer2.play(3); //Boot Sound
  delay(2000);
  myDFPlayer2.stop();

  //All set
  Serial.println("Setup Complete!");
}




// ***************************************
// ************* Da Loop *****************
// ***************************************


void loop() {

  //OTA
  ArduinoOTA.handle();
  MQTT_connect();

  //****** OOkin' Loopin' ******//

  //Recieve MQTT for Eye Reset
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(0.01))) {
    uint16_t valueOperational = atoi((char *)eyeReset.lastread);
    Serial.println(valueOperational);
    if (valueOperational == 1) {
      //Change eye to green
      delay(10);
      setPixel(0, 0, 255, 0);
      delay(10);
      FastLED.show();
      FastLED.show();
    }
    delay(10);
  }

  //Check for doorbell
  delay(10);
  int doorbellReading = digitalRead(doorbell);
  if (doorbellReading == LOW) {
    delay(10);
    int doorbellReading2 = digitalRead(doorbell);
    if (doorbellReading2 == LOW) {
      delay(10);
      int doorbellReading3 = digitalRead(doorbell);
      if (doorbellReading3 == LOW) {
        
        currentDoorbellSound++;
        if (currentDoorbellSound > numberOfDoorbellSounds) {
          currentDoorbellSound = 1;
        }
        myDFPlayer.play(currentDoorbellSound);
        delay(2500); //Let the talking begin
        mechControl.publish(1); //Send ghostface flying
        Serial.print("Doorbell song ");
        Serial.println(currentDoorbellSound);
        delay(19000); //Time needed to reset jumping ghost face
      }
    }
  }

  //Search for Guests, Triple check to prevent false readings.
  // 1) Send MQTT to FogControl to kick on fog
  // 2) Send MQTT to HouseMusic to start music in the house
  // 3) Start PorchMusic
  // 4) Change Eye to red
  //
  int sensorReading = digitalRead(sensor);
  if (sensorReading == HIGH) {
    delay(50);
    int sensorReading2 = digitalRead(sensor);
    if (sensorReading2 == HIGH) {
      delay(50);
      int sensorReading3 = digitalRead(sensor);
      if (sensorReading3 == HIGH) {
        Serial.println(" [ Guests detected! ] ");
        delay(100);

        //Play Porch Music, change eye to red, send fog and house music mqtt
        //The isSongPlaying check prevents these from occuring repeatedly until
        //the porch audio is done
        isBackgroundMusicPlaying();
        if (isSongPlaying == 0) {
          currentPorchSound++;
          if (currentPorchSound > numberOfPorchSounds) {
            currentPorchSound = 1;
          }
          myDFPlayer2.play(currentPorchSound);
          delay(10);
          isSongPlaying = 1;
          previousMusicMillis = millis();
          Serial.print("Porch song ");
          Serial.println(currentPorchSound);
          delay(100);

          //Change eye to Red
          delay(10);
          setPixel(0, 255, 0, 0);
          delay(10);
          FastLED.show();
          FastLED.show();

          //Send MQTT for Fog Control
          fogControl.publish(3);
          delay(100);

          //Send MQTT for House Music
          houseMusic.publish(1);
          delay(100);
        }
        isBackgroundMusicPlaying();
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

void isBackgroundMusicPlaying() { //Prevents porch songs from playing nonstop
  if (isSongPlaying == 1) {
    unsigned long currentMusicMillis = millis();
    if ((currentMusicMillis - previousMusicMillis) > 33 * 1000) { //33 seconds is length of porch music
      previousMusicMillis = currentMusicMillis;
      isSongPlaying = 0;
    }
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
  ArduinoOTA.setHostname("PerilousPassage_Porch");
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
