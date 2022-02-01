#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#include <string.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "devolo-f4068d744d56"
#define WIFI_PASSWORD "CFUCLFLIXVPOJKEL"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCK7xzDTTzflkvvpLtaEuUSZdBR3Qq-2R4"

// Insert RTDB URL */
#define DATABASE_URL "https://smart-booze-default-rtdb.europe-west1.firebasedatabase.app/"

// Unique Device ID (uuid v4)
#define DEVICE_ID "b591a4f6-f4e3-4a5b-ab5a-cd3a261b406e"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
String device_id_str = String(DEVICE_ID);
String path = "devices/" + device_id_str + "/Measurement";
String totalMeasurementPath = "devices/" + device_id_str + "/" + "totalMeasurements";
char measurementPath[64] = "";
char numberArray[7] = "";
char timePath[100] = "";
char valuePath[100] = "";
char datePath[100] = "";
int meassurementsPerformed = 0;
int startMillis = 0;

void clearArrays();

void setup()
{
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("ok");
    signupOK = true;
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  if (Firebase.ready() && signupOK)
  {
    if (Firebase.RTDB.getInt(&fbdo, totalMeasurementPath))
    {
      Serial.println("Loaded Init Value");
      meassurementsPerformed = fbdo.to<int>();
    }
    else
    {
      Serial.println("FAILED: " + fbdo.errorReason());
      meassurementsPerformed = 0;
    }
  }
  strcat(measurementPath, path.c_str()); // this string is always 56 chars long at that point because uuid is always the same length(36 chars)
}

void loop()
{
  startMillis = millis();
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {

    sendDataPrevMillis = millis();

    meassurementsPerformed++;
    strcat(timePath, measurementPath);
    strcat(timePath, "/time");
    strcat(valuePath, measurementPath);
    strcat(valuePath, "/value");
    strcat(datePath, measurementPath);
    strcat(datePath, "/date");
    if (Firebase.RTDB.setInt(&fbdo, totalMeasurementPath, meassurementsPerformed))
    {
      sprintf(numberArray, "%06d", meassurementsPerformed);
      strcat(measurementPath, numberArray);
      Serial.println("set total");
    }
    else
    {
      Serial.println("FAILED: " + fbdo.errorReason());
    }

    /* Set the alcohol value at the specific time */
    if (Firebase.RTDB.setFloat(&fbdo, valuePath, 0.05))
    {
      Serial.println("Set value");
    }
    else
    {
      Serial.println("FAILED: " + fbdo.errorReason());
    }

    /* Set time of measurement*/
    if (Firebase.RTDB.setString(&fbdo, timePath, "1200"))
    {
      Serial.println("Set time");
    }
    else
    {
      Serial.println("FAILED: " + fbdo.errorReason());
    }

    /* Set date of measurement*/
    if (Firebase.RTDB.setString(&fbdo, datePath, "06042022"))
    {
      Serial.println("Set date");
    }
    else
    {
      Serial.println("FAILED: " + fbdo.errorReason());
    }
    Serial.printf("Took %d ms\n", (millis() - startMillis));
    clearArrays();
  }

  delay(1000);
}

void clearArrays()
{
  measurementPath[56] = '\0';
  measurementPath[57] = '\0';
  measurementPath[58] = '\0';
  measurementPath[59] = '\0';
  measurementPath[60] = '\0';
  measurementPath[61] = '\0';

  memset(valuePath, '\0', 100);
  memset(datePath, '\0', 100);
  memset(timePath, '\0', 100);
}
