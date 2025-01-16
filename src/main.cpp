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
//#include <Adafruit_GFX.h>
//#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
//#include <Fonts/FreeSans9pt7b.h>
#include "max6675.h"
#include <SPI.h>
#include <ESP32RotaryEncoder.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip


/*
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
*/
TFT_eSPI display = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
//TFT_eSPI box = TFT_eSPI(); 
TFT_eSprite box = TFT_eSprite(&display);

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

// GPIO and low level buttons
#define btnTIMEOUT -1
#define btnNONE 0
#define btnESC 4
#define btnRIGHT 36
#define btnLEFT 34
#define btnSELECT 39

// program defined buttons
#define buttonNone -1
#define buttonLeft 1
#define buttonCenter 2
#define buttonRight 3
#define buttonEscape 4




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
  boolean buttonPressed = false;
  boolean sensor1SettingsSelect =false;
  long rotaryValue;
  boolean rotaryTurned = false;
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

char *mainMenu_table[] = {"programma kiezen", "sprinkler kiezen", "relais kiezen", "programma wijzigen","test"};

#include <display.h>
#include <temperature.h>
#include <file_handling.h>
#include <MQTT_handling.h>
#include <HTTP_handling.h>
#include <server_pages.h>




void rotaryCallback( long value )
{
	//Serial.printf( "Value: %ld\n", value );
  button.rotaryTurned = true;
  button.rotaryValue = value;
}

void buttonCallback( unsigned long duration )
{
	//Serial.printf( "boop! button was down for %lu ms\n", duration );
  button.buttonPressed = true;
  button.millisPressed = duration;
}


void setup() {
  Serial.begin(115200);
  box.createSprite(150,100);
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
  myServer.intervalMQTT = 15;
  //rotaryEncoder.setEncoderType( EncoderType::FLOATING );
  rotaryEncoder.setEncoderType( EncoderType::HAS_PULLUP);

	// Range of values to be returned by the encoder: minimum is 1, maximum is 10
	// The third argument specifies whether turning past the minimum/maximum will
	// wrap around to the other side:
	//  - true  = turn past 10, wrap to 1; turn past 1, wrap to 10
	//  - false = turn past 10, stay on 10; turn past 1, stay on 1
	rotaryEncoder.setBoundaries( 1, 10, true );

	// The function specified here will be called every time the knob is turned
	// and the current value will be passed to it
	rotaryEncoder.onTurned( &rotaryCallback );

	// The function specified here will be called every time the button is pushed and
	// the duration (in milliseconds) that the button was down will be passed to it
	rotaryEncoder.onPressed( &buttonCallback );

	// This is where the inputs are configured and the interrupts get attached
	rotaryEncoder.begin(1);
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
  if (button.buttonPressed){
    Serial.printf( "boop! button was down for %lu ms\n", button.millisPressed );
    //rotaryEncoder.disable();
    int keuze =mainMenu();
    outlineMainscreen();
    //testMenu();
    //button.buttonPressed = false;
    rotaryEncoder.enable();
  }
   if (button.rotaryTurned){
    Serial.printf( "rotary was turned to %lu \n", button.rotaryValue );
    //rotaryEncoder.disable();
    //int keuze =mainMenu();
    
    int keuze =mainMenu();
    outlineMainscreen();
    button.rotaryTurned = false;
    rotaryEncoder.enable();
  } 
  ArduinoOTA.handle();
}
