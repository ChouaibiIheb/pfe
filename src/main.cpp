#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

#include "DHT.h"
#include <Adafruit_Sensor.h>
#define WIFI_SSID "Orange-206F"
#define WIFI_PASSWORD "RM66H5AA151"
#define API_KEY "AIzaSyAaW42iYiAQODvLaOhwgkulYKOQMa9jNbQ"

/* 2. Define the RTDB URL */
#define DATABASE_URL "https://sbapp-6c7c9-default-rtdb.firebaseio.com"
#define LED_BUILTIN 2

#define DHTPIN 4
#define DHTTYPE DHT11 

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String stringVal ="";
DHT dht(DHTPIN, DHTTYPE);

unsigned long sendDataPrevMillis = 0;
int intValue;
float floatValue;
bool signupOK = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin();
  Serial.begin(115200);
    Serial.println();

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
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  
// int gaz = analogRead(21);
float h = dht.readHumidity();
// Read temperature as Celsius (the default)
float t = dht.readTemperature();

  

   if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    // if (Firebase.RTDB.getString(&fbdo ,"/Lampe/state")) {
      
    //     stringVal = fbdo.stringData();
    //     Serial.println(stringVal);
    //     if(stringVal=="on"){ digitalWrite(LED_BUILTIN ,HIGH);
    //   Serial.println("high");}
    //     else if(stringVal=="off"){ digitalWrite(LED_BUILTIN ,LOW);
    //     Serial.println("low");}
    //     Serial.println(fbdo.stringData());
        
        
    // }
    if (Firebase.RTDB.setFloat(&fbdo,"edinnezin/Kitchen/capteurs/temperature",t)){
      Serial.println("PASSED");
       Serial.print("temperature ");
       Serial.println(t);
      
    }
    if (Firebase.RTDB.setFloat(&fbdo,"edinnezin/Kitchen/capteurs/humidity",h))
    {
      Serial.println("PASSED");
       Serial.print("humidity ");
       Serial.println(h);
    }
    // else if (Firebase.RTDB.setFloat(&fbdo,"edinnezin/Kitchen/gaz:",gaz))
    // {
    //   Serial.println("PASSED");
    //    Serial.print("gaz: ");
    //    Serial.println(gaz);
    // }
    
    
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    






  
}
}
