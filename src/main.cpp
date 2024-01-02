#include <Arduino.h>

#define VERSION "Version 23/10/23"
#define nameprefix "HOME"
#define moduletype "OVEN CONTROLLER"

#include <WebServer.h>
#include "SPIFFS.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include "Wire.h"
#include <WiFiManager.h>
#include "OTA.h"
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
//#include <Fonts/FreeSans9pt7b.h>
#include "max6675.h"
#include <SPI.h>

//test github 
//test2 github

#define TFT_CS 5
#define TFT_RST 23 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 16
#define TFT_MOSI 19
#define TFT_SCLK 18
#define TFT_BACKLIGHT 4 // Display backlight pin
#define BLACK ST77XX_BLACK
#define WHITE ST77XX_WHITE
#define BLUE ST77XX_BLUE
#define GREEN ST77XX_GREEN
#define GREY 0x8410
Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

const uint8_t wifi_OK[] PROGMEM = {
    0xF0, 0x00, 0x08, 0x00, 0xE4, 0x00, 0x12, 0x00, 0xCA, 0x00, 0x2A, 0x00};
const uint8_t wifi_NOK[] PROGMEM = {
    0xF0, 0x40, 0x08, 0x40, 0xE4, 0x40, 0x12, 0x40, 0xCA, 0x00, 0x2A, 0x40};


#define btnLEFT 1
#define btnRIGHT 2
#define btnLEFTLONG 3
#define btnRIGHTLONG 4
#define btnSELECT 1
#define btnTIMEOUT -1
#define btnNONE 0
#define btnSHORT 1
#define btnLONG 2

#define rotarySwitch 17
#define rotaryS1 21
#define rotaryS2 22

#define heating 2
#define cooling 1
#define idle 0

#define relayOn 1
#define relayOff 0

#define local true
#define remote false

#define commandPrefix "cmnd"
#define dataPrefix "data"
#define infoPrefix "info"

int rotaryCounter= 0;
int rotaryCurrentStateCLK;
int rotaryPreviousStateCLK;
String encdir ="";

int thermoDO1 = 2;
int thermoCS1 = 15;
int thermoCLK1 = 13;
int thermoDO2 = 25;
int thermoCS2 = 26;
int thermoCLK2 = 27;

MAX6675 thermocouple1(thermoCLK1, thermoCS1, thermoDO1);
MAX6675 thermocouple2(thermoCLK2, thermoCS2, thermoDO2);
unsigned long previousTimeTempRead = 0;

char fullhostname[40];
WiFiClient client;
WiFiClient client2;
PubSubClient clientEsp(client2);
WiFiServer server(80); // Set web server port number to 80
String HTTPrequest;    // Variable to store the HTTP request
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;
byte refreshRate = 5;

StaticJsonDocument<512> doc;
char myPayload[250];
char myTopic[40];
String myPayloadString = "";
unsigned long previousTimeMQTT = 0;
unsigned long previousTimeHistory = 0;
//unsigned long previousTimeThingsSpeak = 0;

//int analogReadValue =0;

struct controle
{
  boolean present;
  int tempGemeten;
  boolean setPoint;
  int setPointValue;
  int tolerance;
  int power;
  //char deviceAddress[20];
  int tempHistory[250];
  int tempHistoryCounter;
  int tempHistoryOverrun;
};
struct controle sensor1; 
struct controle sensor2; 
struct controle sensor1_local;
struct controle sensor1_remote;
typedef struct
{
  int buttonPressed;
  int buttonPin;
  unsigned long millisPressed;
  boolean buttonReleased;
  unsigned long millisReleased;
  boolean sensor1SettingsSelect =false;
} buttonData;
static buttonData button;

struct server
{
  long channelNumber;
  char APIKey[32];
  unsigned int intervalThingSpeak;
  char serverMQTT[32];
  int port;
  char username[32];
  char passwordMQTT[32];
  char topic[32];
  unsigned int intervalMQTT;
};
struct server myServer;

struct connection
{
  unsigned int sensorConnect;
  unsigned int refreshRate;
  boolean connectToWIFI;
  boolean connectToMQTT;
  boolean connectToThingSpeak;
};
struct connection mySystem;

#include <display.h>
#include <temperature.h>
#include <file_handling.h>
#include <MQTT_handling.h>
#include <HTTP_handling.h>
#include <server_pages.h>


void IRAM_ATTR pushButtonPressedLeft()
{
  button.buttonPressed = btnLEFT;
  button.millisPressed = millis();
}

void IRAM_ATTR pushButtonPressedRight()
{
  if (digitalRead(35) == 0){
    if (button.buttonPressed != btnRIGHT){
      button.buttonPressed = btnRIGHT;
      button.millisPressed = millis();
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Define pinmodes");
  //pinMode(sensor1HeatPin, OUTPUT);
  //digitalWrite(sensor1HeatPin, relayOff);
  //pinMode(switchSensor1Settings, INPUT_PULLDOWN_16); //(enable it with INPUT_PULLDOWN).
  //pinMode(switchSensor1Settings, INPUT); // pinMode(BTN_LEFT, INPUT_PULLUP);
  //pinMode(btnPinSELECT, INPUT); // pinMode(BTN_LEFT, INPUT_PULLUP);
  //attachInterrupt(btnPinSELECT, pushButtonPressed, FALLING);
  //pinMode(0, INPUT_PULLUP);
  pinMode(rotarySwitch,INPUT);
  pinMode(rotaryS1,INPUT);
  pinMode(rotaryS2,INPUT);

  pinMode(35, INPUT);
  //attachInterrupt(0, pushButtonPressedLeft, FALLING);
  attachInterrupt(17, pushButtonPressedLeft, FALLING);
  attachInterrupt(35, pushButtonPressedRight, FALLING);
  loadDataFromFile(); 
  //selectSensor1Settings();
  setupDisplay();
  //checkForReset();
  //WiFi.begin("xxx","xxx");
  WiFi.begin("57_home","wonderfulcurtain962");
  //myServer.connectToWIFI = false;
  setupHostName();
  setupWifiManager();
  //showWIFIconnection();
  //showStatus();
  outlineMainscreen();
  // put your setup code here, to run once:
  //int result = myFunction(2, 3);
}




void loop() {
client = server.available();
  if (client)
  {
    handleRequest();
  }
  checkTemperature();
  if (mySystem.connectToWIFI){
    if (mySystem.connectToMQTT){
      connectMQTT();
      clientEsp.loop();
      publishWithMQTT();
    }
  }
  if ((currentTime - previousTimeHistory) > (5000)){
    previousTimeHistory= currentTime;
    updateGraph(sensor1.tempHistoryCounter);
    sensor1.tempHistoryCounter++;
    sensor2.tempHistoryCounter++;
  }
  //buttonCheck();
  rotaryCheck();
  ArduinoOTA.handle();
}
