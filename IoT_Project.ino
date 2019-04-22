#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

// Including libraries
#include <ESP8266WiFi.h>
#include <HX711.h>

//Defination

#define calibration_factor -110 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define DOUT  D1
#define CLK  D2
HX711 scale(DOUT, CLK);

#define FIREBASE_HOST "smartcontainer-35d53.firebaseio.com"
#define FIREBASE_AUTH "8RMjOeX78v3hXNTBpQuHL4CJrcU2xYGkk0MNldpT"
#define WIFI_SSID "Place your Wifi ssid"
#define WIFI_PASSWORD "Place your wifi password"

//Setup

void setup() {

  // Smart Container Project...
  
  Serial.begin(9600);
  scale.set_scale(calibration_factor); 
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0



  // connect to wifi.
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Waiting.");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  ESP.wdtDisable();
}

float weight=0.0f;

void loop() {
  weight=scale.get_units();  //scale.get_units() returns a float
 if(weight<=5)
  {
   weight=0.0f;
  }
 Serial.print(weight);
 Serial.println();

  // update value
  
 ESP.wdtFeed();
  Firebase.setFloat("container1/Sugar",weight);
  
  // handle error
  
  if (Firebase.failed()) {
      Serial.print("setting /Sugar failed:");
      Serial.println(Firebase.error());
      return;
  }
  Serial.print("pushed\t");
  yield();
}
