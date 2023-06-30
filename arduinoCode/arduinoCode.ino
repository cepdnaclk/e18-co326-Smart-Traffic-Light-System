#include <WiFi.h>
#include <PubSubClient.h>
#include "time.h"

// date and time settings
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;
const char* ntpServer = "pool.ntp.org";

// Replace the next variables with your SSID/Password combination
const char* ssid = "Mi 9T";
const char* password = "123456789";

// Add your MQTT Broker IP address,
const char* mqtt_server = "test.mosquitto.org";

// mqtt client
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// pins of the ultrasonic sensors
const int trigPinSensor1 = 5;
const int echoPinSensor1 = 18;

const int trigPinSensor2 = 15;
const int echoPinSensor2 = 2;

const int trigPinSensor3 = 15;
const int echoPinSensor3 = 2;


//define sound speed in cm/uS
#define SOUND_SPEED 0.034

// pins for the LEDs
#define UoP_CO326_Group7_Actuator_LED_RED1 32
#define UoP_CO326_Group7_Actuator_LED_ORANGE1 25
#define UoP_CO326_Group7_Actuator_LED_GREEN1 27

#define UoP_CO326_Group7_Actuator_LED_RED2 32
#define UoP_CO326_Group7_Actuator_LED_ORANGE2 25
#define UoP_CO326_Group7_Actuator_LED_GREEN2 27

// what LED is on right now
// R - Red
// O - Orange
// G - Green
char currentLED_1 = 'R';
char currentLED_2 = 'R';

// callback for MQTT
// This runs when there is a new message from MQTT
void callback(char* topic, byte* message, unsigned int length) {
  // print the message for debugging
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");

  // convert the byte messge into a char array
  String messageTemp;
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  Serial.println(messageTemp);


  // if its to change the colour. Change colour
  if (messageTemp == "Red1") {
    TurnOffAllLEDS_1();

    TurnOnRED_1();
  } else if (messageTemp == "Orange1") {
    TurnOffAllLEDS_1();

    TurnOnORANGE_1();
  } else if (messageTemp == "Green1") {
    TurnOffAllLEDS_1();

    TurnOnGREEN_1();
  }else if (messageTemp == "Red2") {
    TurnOffAllLEDS_2();

    TurnOnRED_2();
  }else if (messageTemp == "Orange2") {
    TurnOffAllLEDS_2();

    TurnOnORANGE_2();
  }else if (messageTemp == "Green2") {
    TurnOffAllLEDS_2();

    TurnOnGREEN_2();
  }
}

// conect to wifi
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


// if MQTT is not connected. Connect
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("co326_group7_smart_trafic_light_system_node_1")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("group7/control");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// get sensor reading from sensor 1
// returns a float as the distance in centimeters
float getUltrasonic1Reading() {
  long duration;
  float distanceCm;
  digitalWrite(trigPinSensor1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinSensor1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinSensor1, LOW);
  duration = pulseIn(echoPinSensor1, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;
  Serial.print("Ultransonic Sensor 1 Distance (cm): ");
  Serial.println(distanceCm);

  return distanceCm;
}

// get sensor reading from sensor 2
// returns a float as the distance in centimeters
float getUltrasonic2Reading() {
  long duration;
  float distanceCm;
  digitalWrite(trigPinSensor2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinSensor2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinSensor2, LOW);
  duration = pulseIn(echoPinSensor2, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;
  Serial.print("Ultransonic Sensor 2 Distance (cm): ");
  Serial.println(distanceCm);

  return distanceCm;
}

// get sensor reading from sensor 3
// returns a float as the distance in centimeters
float getUltrasonic3Reading() {
  long duration;
  float distanceCm;
  digitalWrite(trigPinSensor3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinSensor3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinSensor3, LOW);
  duration = pulseIn(echoPinSensor3, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;
  Serial.print("Ultransonic Sensor 2 Distance (cm): ");
  Serial.println(distanceCm);

  return distanceCm;
}

// Functions to turn on and off LEDs
void TurnOnRED_1() {
  digitalWrite(UoP_CO326_Group7_Actuator_LED_RED1, HIGH);
  currentLED_1 = 'R';
}

void TurnOffRED_1() {
  digitalWrite(UoP_CO326_Group7_Actuator_LED_RED1, LOW);
  currentLED_1 = 'N';
}

void TurnOnORANGE_1() {
  digitalWrite(UoP_CO326_Group7_Actuator_LED_ORANGE1, HIGH);
  currentLED_1 = 'O';
}

void TurnOffORANGE_1() {
  digitalWrite(UoP_CO326_Group7_Actuator_LED_ORANGE1, LOW);
  currentLED_1 = 'N';
}

void TurnOnGREEN_1() {
  digitalWrite(UoP_CO326_Group7_Actuator_LED_GREEN1, HIGH);
  currentLED_1 = 'G';
}

void TurnOffGREEN_1() {
  digitalWrite(UoP_CO326_Group7_Actuator_LED_GREEN1, LOW);
  currentLED_1 = 'N';
}

void TurnOffAllLEDS_1() {
  TurnOffRED_1();
  TurnOffORANGE_1();
  TurnOffGREEN_1();
  currentLED_1 = 'N';
}

void TurnOnRED_2() {
  digitalWrite(UoP_CO326_Group7_Actuator_LED_RED2, HIGH);
  currentLED_2 = 'R';
}

void TurnOffRED_2() {
  digitalWrite(UoP_CO326_Group7_Actuator_LED_RED2, LOW);
  currentLED_2 = 'N';
}

void TurnOnORANGE_2() {
  digitalWrite(UoP_CO326_Group7_Actuator_LED_ORANGE2, HIGH);
  currentLED_2 = 'O';
}

void TurnOffORANGE_2() {
  digitalWrite(UoP_CO326_Group7_Actuator_LED_ORANGE2, LOW);
  currentLED_2 = 'N';
}

void TurnOnGREEN_2() {
  digitalWrite(UoP_CO326_Group7_Actuator_LED_GREEN2, HIGH);
  currentLED_2 = 'G';
}

void TurnOffGREEN_2() {
  digitalWrite(UoP_CO326_Group7_Actuator_LED_GREEN2, LOW);
  currentLED_2 = 'N';
}

void TurnOffAllLEDS_2() {
  TurnOffRED_2();
  TurnOffORANGE_2();
  TurnOffGREEN_2();
  currentLED_2 = 'N';
}

String getLocalTimeAsString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "Failed to obtain time";
  }

  char buffer[150];
  strftime(buffer, sizeof(buffer), "%B %d %Y %H:%M:%S", &timeinfo);

  return String(buffer);
}

// runs this code once when program starts
void setup() {
  // set serial port baud rate
  Serial.begin(115200);

  // set ultrasoinc sensor pins mode
  pinMode(trigPinSensor1, OUTPUT);
  pinMode(echoPinSensor1, INPUT);
  pinMode(trigPinSensor2, OUTPUT);
  pinMode(echoPinSensor2, INPUT);

  // set LED pings
  pinMode(UoP_CO326_Group7_Actuator_LED_RED1, OUTPUT);
  pinMode(UoP_CO326_Group7_Actuator_LED_ORANGE1, OUTPUT);
  pinMode(UoP_CO326_Group7_Actuator_LED_GREEN1, OUTPUT);

  pinMode(UoP_CO326_Group7_Actuator_LED_RED2, OUTPUT);
  pinMode(UoP_CO326_Group7_Actuator_LED_ORANGE2, OUTPUT);
  pinMode(UoP_CO326_Group7_Actuator_LED_GREEN2, OUTPUT);

  // connect to wifi
  setup_wifi();

  // set mqtt configurations
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

// run this code in a loop
void loop() {
  // if mqtt is not conected. connect to mqtt broker
  if (!client.connected()) {
    reconnect();
  }

  // get readings from the sensors
  int sensor1 = getUltrasonic1Reading();
  int sensor2 = getUltrasonic2Reading();
  int sensor3 = getUltrasonic3Reading();

  String dateAndTime = getLocalTimeAsString();

  // create the mqtt msg to send
  // String msg = String(currentLED) + "," + String(sensor1) + "," + String(sensor2) + "," + dateAndTime;
  String msg = "{\"deviceName\" : \"trafficLightsNumber1\" , \"timeMeasured\" : \"" + dateAndTime + "\", \"distanceSensor1\" : \"" + String(sensor1) + "\", \"distanceSensor2\" : \"" + String(sensor2) + "\", \"distanceSensor3\" : \"" + String(sensor3) + "\", \"currentColor1\" : \"" + String(currentLED_1) + "\", \"currentColor2\" : \""  + String(currentLED_2) + "\"}";

  char msgCharAray[750] = {};
  msg.toCharArray(msgCharAray, 749);
  Serial.println(msg);


  client.publish("group7/sensors", msgCharAray);

  client.loop();


  delay(1000);
}