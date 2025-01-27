#include <Arduino.h>

#define VERSION "Version 27/01/25"
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
#include "max6675.h"
#include <SPI.h>
#include <ESP32RotaryEncoder.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include "Free_Fonts.h" // Include the header file attached to this sketch


TFT_eSPI display = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
TFT_eSprite mainscreen = TFT_eSprite(&display);

const uint8_t wifi_OK[] PROGMEM = {
    0xF0, 0x00, 0x08, 0x00, 0xE4, 0x00, 0x12, 0x00, 0xCA, 0x00, 0x2A, 0x00};
const uint8_t wifi_NOK[] PROGMEM = {
    0xF0, 0x40, 0x08, 0x40, 0xE4, 0x40, 0x12, 0x40, 0xCA, 0x00, 0x2A, 0x40};


volatile float dutycycle =50;
volatile float periode = 5;
volatile boolean heatingPower=false;
volatile boolean overrun=false;
volatile unsigned long timeOn;
volatile unsigned long timePeriode;
volatile unsigned long timerCount;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
hw_timer_t *heatingTimer = NULL;

#define relaisPin 33
#define buttonNone -255

int menuPositie;

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

// Change these to the actual pin numbers that
// you've connected your rotary encoder to
const uint8_t DI_ENCODER_A   = 21;
const uint8_t DI_ENCODER_B   = 22;
const int8_t  DI_ENCODER_SW  = 17;

RotaryEncoder rotaryEncoder( DI_ENCODER_A, DI_ENCODER_B, DI_ENCODER_SW);


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
  unsigned long millisPressed;
  boolean longPress = false;
  boolean buttonPressed = false;
  long rotaryValue;
  boolean rotaryTurned = false;
  boolean sensor1SettingsSelect =false;
} buttonData;
static buttonData button;

char *mainMenu_table[] = {"test menu", "Sensor 2", "Menukeuze 3", "Menukeuze 4","Menukeuze 5"};
char *jaNee_table[] = {"Ja", "Nee"};

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



#include <temperature.h>
#include <file_handling.h>
#include <MQTT_handling.h>
#include <HTTP_handling.h>
#include <server_pages.h>
#include <display.h>



void rotaryCallback( long value )
{
  button.rotaryTurned = true;
  button.rotaryValue = value;
}

void buttonCallback( unsigned long duration )
{
  button.buttonPressed = true;
  //button.millisPressed = duration;
  if (duration > 1000){
    button.longPress = true;
  } else {
    button.longPress = false;
  }
  Serial.printf( "down for %lu ms %d\n", duration,button.longPress );
}

void IRAM_ATTR heatingPowercheck(){
    timerCount++;

    if ((timerCount == timeOn)&& (timerCount!=timePeriode)){
      heatingPower = false;
      overrun = true;
      digitalWrite(relaisPin,heatingPower);
    } else if (timerCount == timePeriode){
      heatingPower = true;
      overrun = true;
      timerCount =0;
      digitalWrite(relaisPin,heatingPower);
    }
}


void setup() {
  Serial.begin(115200);
  loadDataFromFile(); 
  setupDisplay();
  WiFi.begin("57_home","wonderfulcurtain962");
  //myServer.connectToWIFI = false;
  setupHostName();
  setupWifiManager();
  //showWIFIconnection();
  //showStatus();
  myServer.intervalMQTT = 15;

  pinMode(relaisPin, OUTPUT);
  periode = 5;
  dutycycle =50;
  resetHeatingTimer();
  heatingTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(heatingTimer, &heatingPowercheck, true);
  timerAlarmWrite(heatingTimer, 10000, true); //100ms
  timerAlarmEnable(heatingTimer);
  timerStart(heatingTimer);
  rotaryEncoder.setEncoderType( EncoderType::HAS_PULLUP);
	rotaryEncoder.setBoundaries( 1, 3, true );
	rotaryEncoder.onTurned( &rotaryCallback );
	rotaryEncoder.onPressed( &buttonCallback );
	rotaryEncoder.begin(1);
  menuPositie = 0;
  digitalWrite(relaisPin,1);
  outlineMainscreen();
}




void loop() {
  client = server.available();
  if (client)
  {
    handleRequest();
  }
  if (mySystem.connectToWIFI){
    if (mySystem.connectToMQTT){
      connectMQTT();
      clientEsp.loop();
      publishWithMQTT();
    }
  }
  /*
  if ((currentTime - previousTimeHistory) > (5000)){
    previousTimeHistory= currentTime;
    //updateGraph(sensor1.tempHistoryCounter);
    sensor1.tempHistoryCounter++;
    sensor2.tempHistoryCounter++;
  }
  */
  if ( overrun ){
    checkTemperature();
    outlineMainscreen();
    overrun =false;
  }
  if (button.buttonPressed){
    int keuze =mainMenu();
    outlineMainscreen();
    rotaryEncoder.setBoundaries( 1, 3, true );
    rotaryEncoder.setEncoderValue(1);
    button.buttonPressed = false;
    rotaryEncoder.enable();
  }
  ArduinoOTA.handle();
}
