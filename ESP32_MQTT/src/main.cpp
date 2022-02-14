/*Virtuino MQTT getting started example
 * Broker: HiveMQ (Secure connection)
 * Supported boards: ESP8266 / ESP32
 * Created by Ilias Lamprou, modified heavily by Tobias Schmalzried
 * Jul 13 2021
 */
#include "Arduino.h"
#include "EEPROM.h"
#include <GSM.h>
#include <PubSubClient.h>

const char pin[] = "PIN of SIM";
const char apn[] = "apn";
const char login[] = "login";
const char password[] = "password";

GSMClient net;
GPRS gprs;
GSM gsmAccess;

//---- MQTT Broker settings
const char *mqtt_server = "e6c0f2b4d98b45a58474f291fbfdcec4.s1.eu.hivemq.cloud"; // replace with your broker url
const char *mqtt_username = "B4ldur";
const char *mqtt_password = "fg%2oR3!Sh6Ntu$Q#571HH$XBp";
const int mqtt_port = 8883;

//---------network settings
PubSubClient client(net);

//---- Device Settings
// set when device is initialized
String userId = "29bb3021-e9ce-44d1-9a38-5fe98e89ac83";
int messagesSent = 0;

// Path settings
char timePath[100] = "";
char valuePath[100] = "";
char datePath[100] = "";
char messagePath[100] = "";
char deviceIdPath[100] = "";
const char *path = "devices";
// placeholder for the outgoing message
char message[46] = "";
char valueString[5] = "";
char numberString[7] = "";
int valueInt = 0;

unsigned long lastMsg = 0;

char server[] = "e6c0f2b4d98b45a58474f291fbfdcec4.s1.eu.hivemq.cloud"; // the base URL
//char path[] = "/latest.txt"; // the path
int port = 80; // the port, 80 for HTTP

#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

int sensor1 = 0;
float value = 0;
int command1 = 0;

const char *sensor1_topic = "sensor1";
const char *sensor2_topic = "sensor2";
const char *userIdTopic = "setUserId";

const char *command1_topic = "command1";

/* Function prototypes */

void publishMessage(const char *, String, boolean);
void updatePaths();
void sendValueToBroker(float);

//============= Called when new message arrives. Will be used in Setup process only ===========

void callback(char *topic, byte *payload, unsigned int length)
{
  String incommingMessage = "";
  for (int i = 0; i < length; i++)
    incommingMessage += (char)payload[i];

  Serial.println("Message arrived [" + String(topic) + "]" + incommingMessage);

  //--- check the incomming message
  if (strcmp(topic, command1_topic) == 0)
  {
    if (incommingMessage.equals("1"))
    {
      digitalWrite(LED_BUILTIN, LOW); // Turn the LED on
    }
    else
    {
      digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off
    }
  }

  else if (strcmp(topic, userIdTopic) == 0)
  {
    // userId = incommingMessage;
  }

  //  check for other commands
  /*  else  if( strcmp(topic,command2_topic) == 0){
      if (incommingMessage.equals("1")) {  } // do something else
   }
   */
}

void connectToNetwork()
{
  // connection state
  bool connected = false;
  Serial.print("connecting to cellular network ...");
  // After starting the modem with gsmAccess.begin()
  // attach to the GPRS network with the APN, login and password
  while (!connected)
  {
    if ((gsmAccess.begin(pin) == GSM_READY) &&
        (gprs.attachGPRS(apn, login, password) == GPRS_READY))
    {
      connected = true;
      Serial.println("connected to network!");
    }
    else
    {
      Serial.print(".");
      delay(1000);
    }
  }

  Serial.println("\nconnected!");
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-"; // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
    {
      Serial.println("connected");

      client.subscribe(command1_topic); // subscribe the topics here
      // client.subscribe(command2_topic);   // subscribe the topics here
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds"); // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//============= Called when new message arrives. Will be used in Setup process only ===========

void updateDeviceId(String newID)
{
  userId = newID;
  updatePaths();
}

//================= publishing as strings ===========
void publishMessage(const char *topic, String payload, boolean retained)
{
  client.publish(topic, (byte *)payload.c_str(), 46, true);
  Serial.println("Message published [" + String(topic) + "]: " + payload);
}

void updatePaths()
{

  memset(valueString, '\0', 5);
  memset(numberString, '\0', 7);
  for (int i = 36; i < 46; i++)
  {
    message[i] = '\0';
  }
}

void sendValueToBroker(float value)
{

  snprintf(numberString, 7, "%06d", messagesSent);
  strcat(message, numberString);
  valueInt = value * 1000; // convert to int to save some storage and avoid sprintf w/ float. Needs to be converted back.
  snprintf(valueString, 5, "%04d", valueInt);
  strcat(message, valueString);
  //publishMessage(path, message, true);
  updatePaths();
  messagesSent++;
  EEPROM.write(1, messagesSent);
}

//========================== setup ============================
void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    delay(1);
  }

  messagesSent = EEPROM.read(1);
  connectToNetwork();
  updatePaths();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  strcat(message, userId.c_str());
}

//======================== loop =========================
void loop()
{

  if (!client.connected())
    reconnect();
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 10000)
  {
    lastMsg = now;
    sendValueToBroker(0.8);
  }
}
