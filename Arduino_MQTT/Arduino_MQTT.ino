/**************************************************************
 *
 * For this example, you need to install PubSubClient library:
 *   https://github.com/knolleary/pubsubclient
 *   or from http://librarymanager/all#PubSubClient
 *
 * TinyGSM Getting Started guide:
 *   https://tiny.cc/tinygsm-readme
 *
 * For more MQTT examples, see PubSubClient library
 *
 **************************************************************
 * Use Mosquitto client tools to work with MQTT
 *   Ubuntu/Linux: sudo apt-get install mosquitto-clients
 *   Windows:      https://mosquitto.org/download/
 *
 * Subscribe for messages:
 *   mosquitto_sub -h test.mosquitto.org -t GsmClientTest/init -t GsmClientTest/ledStatus -q 1
 * Toggle led:
 *   mosquitto_pub -h test.mosquitto.org -t GsmClientTest/led -q 1 -m "toggle"
 *
 * You can use Node-RED for wiring together MQTT-enabled devices
 *   https://nodered.org/
 * Also, take a look at these additional Node-RED modules:
 *   node-red-contrib-blynk-ws
 *   node-red-dashboard
 *
 **************************************************************/

// Please select the corresponding model

// #define SIM800L_IP5306_VERSION_20190610
// #define SIM800L_AXP192_VERSION_20200327
// #define SIM800C_AXP192_VERSION_20200609
#define SIM800L_IP5306_VERSION_20200811
#include <esp_task_wdt.h>
//3 seconds WDT
#define WDT_TIMEOUT 60*60
#include "utilities.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// Select your modem:
#define TINY_GSM_MODEM_SIM800

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon

// Add a reception delay - may be needed for a fast processor at a slow baud rate
// #define TINY_GSM_YIELD() { delay(2); }

// Define how you're planning to connect to the internet
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false

// set GSM PIN, if any
#define GSM_PIN ""

const byte ledPin = 13;
//const byte oneon = 13;
//const byte oneoff = 12;
//const byte twoone = 25;
//const byte twooff = 14;
//const byte threeon = 33;
//const byte threeoff = 32;
//const byte fouron = 19;
//const byte fouroff = 18;
// Your GPRS credentials, if any
const char apn[] = "TM";
const char gprsUser[] = "";
const char gprsPass[] = "";

// MQTT details
const char *broker = "mqtthome.ddns.net";
const char* mqttUser = "panagiotis";
const char* mqttPassword = "16o29696";

const char *topicLed = "a";
const char *topicInit = "a";

#include <TinyGsmClient.h>
#include <PubSubClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif
TinyGsmClient client(modem);
PubSubClient mqtt(client);

int ledStatus = LOW;

uint32_t lastReconnectAttempt = 0;

void mqttCallback(char *topic, byte *payload, unsigned int len)
{
   Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < len; i++) {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  //Serial.println("----------------------------------------");

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "a") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);\
      pwm.setPWM(0, 4096, 0);
      pwm.setPWM(1, 4096, 0);
      pwm.setPWM(2, 4096, 0);
      pwm.setPWM(3, 4096, 0);
      pwm.setPWM(4, 4096, 0);
      pwm.setPWM(5, 4096, 0);
      pwm.setPWM(6, 4096, 0);
      pwm.setPWM(7, 4096, 0);
      pwm.setPWM(8, 4096, 0);
      pwm.setPWM(9, 4096, 0);
      pwm.setPWM(10, 4096, 0);
      pwm.setPWM(11, 4096, 0);
      pwm.setPWM(12, 4096, 0);
      pwm.setPWM(13, 4096, 0);
      pwm.setPWM(14, 4096, 0);
      pwm.setPWM(15, 4096, 0);
      
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      pwm.setPWM(0, 0, 4096);
      pwm.setPWM(1, 0, 4096);
      pwm.setPWM(2, 0, 4096);
      pwm.setPWM(3, 0, 4096);
      pwm.setPWM(4, 0, 4096);
      pwm.setPWM(5, 0, 4096);
      pwm.setPWM(6, 0, 4096);
      pwm.setPWM(7, 0, 4096);
      pwm.setPWM(8, 0, 4096);
      pwm.setPWM(9, 0, 4096);
      pwm.setPWM(10, 0, 4096);
      pwm.setPWM(11, 0, 4096);
      pwm.setPWM(12, 0, 4096);
      pwm.setPWM(13, 0, 4096);
      pwm.setPWM(14, 0, 4096);
      pwm.setPWM(15, 0, 4096);
//      digitalWrite(ledPin, LOW);
//      digitalWrite(oneoff, LOW);
//      digitalWrite(oneon, LOW);
//      digitalWrite(twoone, LOW);
//      digitalWrite(twooff, LOW);
//      digitalWrite(threeon, LOW);
//      digitalWrite(threeoff, LOW);
//      digitalWrite(fouron, LOW);
//      digitalWrite(fouroff, LOW);
    }
    else if(messageTemp == "on0"){
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("oneon");
      digitalWrite(ledPin, HIGH);
//      digitalWrite(oneoff, LOW);
      pwm.setPWM(0, 4096, 0);
      delay(100);
      pwm.setPWM(1, 0, 4096);
      delay(1000);
      pwm.setPWM(0, 0, 4096);
    }
    else if(messageTemp == "off0"){
      
//      Serial.println("oneoff");
      digitalWrite(ledPin, LOW);
      pwm.setPWM(1, 4096, 0);
      delay(100);
      pwm.setPWM(0, 0, 4096);
      delay(1000);
      pwm.setPWM(1, 0, 4096);
//      digitalWrite(oneoff, HIGH);
    }
    else if(messageTemp == "on1"){
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("twoon");
//      digitalWrite(twoone, HIGH);
//      digitalWrite(twooff, LOW);
        pwm.setPWM(2, 4096, 0);
        delay(100);
        pwm.setPWM(3, 0, 4096);
        delay(1000);
        pwm.setPWM(2, 0, 4096);
    }
    else if(messageTemp == "off1"){
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("twooff");
//      digitalWrite(twoone, LOW);
//      digitalWrite(twooff, HIGH);
        pwm.setPWM(3, 4096, 0);
        delay(100);
        pwm.setPWM(2, 0, 4096);
        delay(1000);
        pwm.setPWM(3, 0, 4096);
    }
    else if(messageTemp == "on2"){
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("threeon");
//      digitalWrite(threeon, HIGH);
//      digitalWrite(threeoff, LOW);
        pwm.setPWM(4, 4096, 0);
        delay(100);
        pwm.setPWM(5, 0, 4096);
        delay(1000);
        pwm.setPWM(4, 0, 4096);
    }
    else if(messageTemp == "off2"){
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("threeoff");
//      digitalWrite(threeon, LOW);
//      digitalWrite(threeoff, HIGH);
        pwm.setPWM(5, 4096, 0);
        delay(100);
        pwm.setPWM(4, 0, 4096);
        delay(1000);
        pwm.setPWM(5, 0, 4096);
    }
     else if(messageTemp == "on3"){
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("fouron");
//      digitalWrite(fouron, HIGH);
//      digitalWrite(fouroff, LOW);
        pwm.setPWM(6, 4096, 0);
        delay(100);
        pwm.setPWM(7, 0, 4096);
        delay(1000);
        pwm.setPWM(6, 0, 4096);
    }
    else if(messageTemp == "off3"){
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("fouroff");
//      digitalWrite(fouron, LOW);
//      digitalWrite(fouroff, HIGH);
        pwm.setPWM(7, 4096, 0);
        delay(100);
        pwm.setPWM(6, 0, 4096);
        delay(1000);
        pwm.setPWM(7, 0, 4096);
    }
    else if(messageTemp == "on4"){
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("fouron");
//      digitalWrite(fouron, HIGH);
//      digitalWrite(fouroff, LOW);
        pwm.setPWM(8, 4096, 0);
        delay(100);
        pwm.setPWM(9, 0, 4096);
        delay(1000);
        pwm.setPWM(8, 0, 4096);
    }
    else if(messageTemp == "off4"){
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("fouroff");
//      digitalWrite(fouron, LOW);
//      digitalWrite(fouroff, HIGH);
        pwm.setPWM(9, 4096, 0);
        delay(100);
        pwm.setPWM(8, 0, 4096);
        delay(1000);
        pwm.setPWM(9, 0, 4096);
    }
    else if(messageTemp == "on5"){
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("fouron");
//      digitalWrite(fouron, HIGH);
//      digitalWrite(fouroff, LOW);
        pwm.setPWM(10, 4096, 0);
        delay(100);
        pwm.setPWM(11, 0, 4096);
        delay(1000);
        pwm.setPWM(10, 0, 4096);
    }
    else if(messageTemp == "off5"){
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("fouroff");
//      digitalWrite(fouron, LOW);
//      digitalWrite(fouroff, HIGH);
        pwm.setPWM(11, 4096, 0);
        delay(100);
        pwm.setPWM(10, 0, 4096);
        delay(1000);
        pwm.setPWM(11, 0, 4096);
    }
    else if(messageTemp == "on6"){
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("fouron");
//      digitalWrite(fouron, HIGH);
//      digitalWrite(fouroff, LOW);
        pwm.setPWM(12, 4096, 0);
        delay(100);
        pwm.setPWM(13, 0, 4096);
        delay(1000);
        pwm.setPWM(12, 0, 4096);
    }
    else if(messageTemp == "off6"){
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("fouroff");
//      digitalWrite(fouron, LOW);
//      digitalWrite(fouroff, HIGH);
        pwm.setPWM(13, 4096, 0);
        delay(100);
        pwm.setPWM(12, 0, 4096);
        delay(1000);
        pwm.setPWM(13, 0, 4096);
    }
    else if(messageTemp == "on7"){
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("fouron");
//      digitalWrite(fouron, HIGH);
//      digitalWrite(fouroff, LOW);
        pwm.setPWM(14, 4096, 0);
        delay(100);
        pwm.setPWM(15, 0, 4096);
        delay(1000);
        pwm.setPWM(14, 0, 4096);
    }
    else if(messageTemp == "off7"){
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("----------------------------------------");
//      Serial.println("fouroff");
//      digitalWrite(fouron, LOW);
//      digitalWrite(fouroff, HIGH);
        pwm.setPWM(15, 4096, 0);
        delay(100);
        pwm.setPWM(14, 0, 4096);
        delay(1000);
        pwm.setPWM(15, 0, 4096);
    }
    
     
  }
  //Serial.println("----------------------------------------");
}

boolean mqttConnect()
{
    SerialMon.print("Connecting to ");
    SerialMon.print(broker);

    // Connect to MQTT Broker
    boolean status = mqtt.connect("GsmClientTest", mqttUser, mqttPassword);

    // Or, if you want to authenticate MQTT:
    //boolean status = mqtt.connect("GsmClientName", "mqtt_user", "mqtt_pass");

    if (status == false) {
        SerialMon.println(" fail");
        return false;
    }
    SerialMon.println(" success");
    mqtt.publish(topicInit, "GsmClientTest started");
    mqtt.subscribe(topicLed);
    return mqtt.connected();
}


void setup()
{
//    pinMode(oneon, OUTPUT);
//    pinMode(oneoff, OUTPUT);
//    pinMode(twoone, OUTPUT);
//    pinMode(twooff, OUTPUT);
//    pinMode(threeon, OUTPUT);
//    pinMode(threeoff, OUTPUT);
//    pinMode(fouron, OUTPUT);
//    pinMode(fouroff, OUTPUT);;
    
    // Set console baud rate
    SerialMon.begin(115200);

    delay(10);

    setupModem();

    SerialMon.println("Wait...");

    // Set GSM module baud rate and UART pins
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

    delay(6000);

    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    SerialMon.println("Initializing modem...");
    modem.restart();
    // modem.init();

    String modemInfo = modem.getModemInfo();
    SerialMon.print("Modem Info: ");
    SerialMon.println(modemInfo);

#if TINY_GSM_USE_GPRS
    // Unlock your SIM card with a PIN if needed
    if ( GSM_PIN && modem.getSimStatus() != 3 ) {
        modem.simUnlock(GSM_PIN);
    }
#endif

    SerialMon.print("Waiting for network...");
    if (!modem.waitForNetwork()) {
        SerialMon.println(" fail");
        delay(10000);
        return;
    }
    SerialMon.println(" success");

    if (modem.isNetworkConnected()) {
        SerialMon.println("Network connected");
    }

    // GPRS connection parameters are usually set after network registration
    SerialMon.print(F("Connecting to "));
    SerialMon.print(apn);
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
        SerialMon.println(" fail");
        delay(10000);
        return;
    }
    SerialMon.println(" success");

    if (modem.isGprsConnected()) {
        SerialMon.println("GPRS connected");
    }

    // MQTT Broker setup
    mqtt.setServer(broker, 1884);
    mqtt.setCallback(mqttCallback);

    //pwm controller
    pwm.begin();
    pwm.setPWMFreq(1000);  // Set to whatever you like, we don't use it in this demo!

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
    Wire.setClock(400000);
    esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch
    
}


int last = millis();
int minutes = 1; //how many minutes fore restarts
void loop()
{

    if (!mqtt.connected()) {
        SerialMon.println("=== MQTT NOT CONNECTED ===");
        // Reconnect every 10 seconds
        uint32_t t = millis();
        if (t - lastReconnectAttempt > 10000L) {
            lastReconnectAttempt = t;
            if (mqttConnect()) {
                lastReconnectAttempt = 0;
            }
        }
        delay(100);
        return;
    }

//    if (millis() - last >= 1000*60*minutes) {
//      Serial.println("Resetting WDT...");
//      esp_task_wdt_reset();
//      last = millis();
//      
//  }

    mqtt.loop();
}
