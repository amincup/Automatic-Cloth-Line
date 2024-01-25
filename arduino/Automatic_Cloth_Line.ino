#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Servo.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "FlyingRainbousFlapflaphySUS"
#define WIFI_PASSWORD "12345sampai9"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDCvmB0M0qSmtF14QfJlOIRZmwuQ-Sa6eI"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "http://jemuran-awikwok-default-rtdb.asia-southeast1.firebasedatabase.app/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
//var declaration
Servo servo;
int sensorValue = analogRead(A0);
#define senRainPin A0
#define ledPin 6
int dataRain;
int dataLDR;

void setup(){
  Serial.begin(9600);
  //program starts here

  //wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
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

  //servo  
  servo.attach(3);
  servo.write(0);
  delay(1000);
  }

void loop(){
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    //program starts here
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    dataRain = rainSensor();
    dataLDR = ldrSensor();

    Serial.print("Analog output: ");
    Serial.println(dataRain);
    Serial.println(dataLDR);
    delay(500);

    //dataRain > 100 || dataLDR < 100
    //servo, jd kalo ujan atau gelap ke posisi narik 0 derajat, kondisi sisanya dia bakal lebarin jemuran ke 90 derajat
    if(dataLDR < 2)
      servo.write(0);
    else
      servo.write(90);
    }

    //up to Firebase
    if (Firebase.RTDB.setInt(&fbdo, "monitor/hujan", dataLDR)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setInt(&fbdo, "monitor/cahaya", dataRain)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  
}
///////////////////////////////////

//fungsi untuk sensor
int rainSensor() {
  //This function returns the analog data to calling function
    int sensorValue = analogRead(senRainPin);  // Read the analog value from sensor
    int outputValue = map(sensorValue, 0, 1023, 255, 0); // map the 10-bit data to 8-bit data
    analogWrite(ledPin, outputValue); // generate PWM signal
    return outputValue;             // Return analog rain value
}

int ldrSensor(){
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    float voltage = sensorValue * (5.0 / 1023.0);
    if (voltage > 3)
      Serial.println("Terang");
    else if(voltage > 2)
      Serial.println("Redup");
    else if(voltage < 1)
      Serial.println("Gelap");
    else
      Serial.println("Gelap");
      
  // print out the value you read:
    Serial.println(sensorValue);
    delay(100);
    return voltage;
}