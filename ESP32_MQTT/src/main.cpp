/*Virtuino MQTT getting started example
 * Broker: HiveMQ (Secure connection)
 * Supported boards: ESP8266 / ESP32
 * Created by Ilias Lamprou, modified heavily by Tobias Schmalzried
 * Jul 13 2021
 */
#include <GSM.h>
#include <WiFi.h>
#include "Arduino.h"
//#include <PubSubClient.h>
//#include <WiFiClientSecure.h>
#include "EEPROM.h"
#define PINNUMBER ""
//---- WiFi settings

const char *ssid = "devolo-f4068d744d56";
const char *password = "CFUCLFLIXVPOJKEL";

//---- MQTT Broker settings
const char *mqtt_server = "e6c0f2b4d98b45a58474f291fbfdcec4.s1.eu.hivemq.cloud"; // replace with your broker url
//const char *mqtt_username = "B4ldur";
//const char *mqtt_password = "fg%2oR3!Sh6Ntu$Q#571HH$XBp";
//const int mqtt_port = 8883;

//---------network settings
#define GPRS_APN       "GPRS_APN" // replace with your GPRS APN
#define GPRS_LOGIN     "login"    // replace with your GPRS login
#define GPRS_PASSWORD  "password" // replace with your GPRS password

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

String date = "06042022";
String timeNow = "0856";

WiFiClientSecure espClient; // for no secure connection use WiFiClient instead of WiFiClientSecure
// WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

// initialize the library instances

GSMClient client;
GPRS gprs;
GSM gsmAccess;
// 

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

static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

/* Function prototypes */

void callback(char *, byte *, unsigned int);
void publishMessage(const char *, String, boolean);
void updatePaths();
void sendValueToBroker(float);
String getDate();
String getTime();

//=========== Setup WiFi, will be  replaced by GSM Module =========
void setup_wifi()
{
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}
void gsm()
{
  while(notConnected)
  {
    if((gsmAccess.begin(PINNUMBER)==GSM_READY) & (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD)==GPRS_READY))
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }
  Serial.println("connecting...");
  // if you get a connection, report back via serial:
  if (client.connect(server, port))
  {
    Serial.println("connected");
    // Make a HTTP request:
    //client.print("GET ");
    //client.print(path);
    //client.println(msg);
    //client.println();
  }
  else
  {
     Serial.println("connection failed");
  }
}
void gsmsend(const char *topic, String payload, boolean retained)
{
  client.print("GET ");
  client.print(path);
  client.write((byte *)payload.c_str(), 46)
  //client.publish(topic, (byte *)payload.c_str(), 46,);
  Serial.println("Message published [" + String(topic) + "]: " + payload);
}
//================= Reconnect to the broker ==============
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

//========================== setup ============================
void setup()
{
  Serial.begin(115200);
  while (!Serial)
    delay(1);
  //setup_wifi();
  gsm();
  EEPROM.begin(4);
  messagesSent = EEPROM.read(1);

  //espClient.setCACert(root_ca); // enable this line and the the "certificate" code for secure connection
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
      digitalWrite(BUILTIN_LED, LOW); // Turn the LED on
    else
      digitalWrite(BUILTIN_LED, HIGH); // Turn the LED off
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
  /* strcat(timePath, userId.c_str());
  strcat(timePath, "/time");
  strcat(valuePath, userId.c_str());
  strcat(valuePath, "/value");
  strcat(datePath, userId.c_str());
  strcat(datePath, "/date");
  strcat(messagePath, userId.c_str());
  strcat(messagePath, "/messageNumber");
  strcat(deviceIdPath, userId.c_str());
  strcat(deviceIdPath, "/userId"); */
  memset(valueString, '\0', 5);
  memset(numberString, '\0', 7);
  for (int i = 36; i < 46; i++)
  {
    message[i] = '\0';
  }
}

void sendValueToBroker(float value)
{
  /*  timeNow = getTime();
   date = getDate();
   // publishMessage(sensor1_topic, String(sensor1), true);
   publishMessage(timePath, timeNow, true);
   publishMessage(datePath, date, true);
   publishMessage(valuePath, String(value), true);
   publishMessage(messagePath, String(messagesSent), true);
   publishMessage(deviceIdPath, userId, true); */
  // publishMessage(sensor2_topic, String(sensor2), true);

  snprintf(numberString, 7, "%06d", messagesSent);
  strcat(message, numberString);
  valueInt = value * 1000; // convert to int to save some storage and avoid sprintf w/ float. Needs to be converted back.
  snprintf(valueString, 5, "%04d", valueInt);
  strcat(message, valueString);
  //publishMessage(path, message, true);
  gsmsend(path, message);
  updatePaths();
  messagesSent++;
  EEPROM.write(1, messagesSent);
  EEPROM.commit();
}

String
getTime()
{
  return "0956";
}

String getDate()
{
  return "06042022";
}
