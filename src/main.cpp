#include <Wire.h>
#include <ZX_Sensor.h>
#include <RestClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <authentication.h>

////////////////////////////
// ESP8266WiFi 
////////////////////////////
RestClient client = RestClient("192.168.86.101");

////////////////////////////
// SparkFun ZX Sensor
////////////////////////////
const int ZX_ADDR = 0x10;  // ZX Sensor I2C address
ZX_Sensor zx_sensor = ZX_Sensor(ZX_ADDR);
GestureType gesture;
uint8_t x_pos;
uint8_t z_pos;

////////////////////////////
// Operational variables
////////////////////////////
int sampleSize = 0;
int averagedZpos = 0;
char json[128];
String response;

////////////////////////////
// Tests & error reporting
// for ESP8266WiFi calls
////////////////////////////
void test_status(int statusCode){
  if(statusCode == 200){
    Serial.print("TEST RESULT: ok (");
    Serial.print(statusCode);
    Serial.println(")");
  }else{
    Serial.print("TEST RESULT: fail (");
    Serial.print(statusCode);
    Serial.println(")");
  }
}

void test_response(){
  if(response == "OK"){
    Serial.println("TEST RESULT: ok (response body)");
  }else{
    Serial.println("TEST RESULT: fail (response body = " + response + ")");
  }
  response = "";
}

////////////////////////////
// One time setup
////////////////////////////
void setup() {
  
  Serial.begin(9600);
  
  // Initialize ZX Sensor (configure I2C and read model ID)
  if ( zx_sensor.init() ) {
    Serial.println("ZX Sensor initialization complete");
  } else {
    Serial.println("Something went wrong during ZX Sensor init!");
  }

  //ssid & pass from authentication.h
  client.begin(ssid, pass);

}

////////////////////////////
// RESTFUL Stuff
////////////////////////////
void sendPOST(char post[]){
  test_status(client.put("/api/9Oc9yMiCkXA1VoNrf-TrnP1BLZA29d-Di6bWaO75/groups/1/action", post, &response));
  test_response();
  Serial.println("PUT sent");
}

////////////////////////////
// Start of program loop
////////////////////////////
void loop() {
  Serial.println("LOOP");
  // If a gesture is detected, handle accordingly
  if ( zx_sensor.gestureAvailable() ) {
    gesture = zx_sensor.readGesture();
    switch ( gesture ){
      case RIGHT_SWIPE:
        Serial.println("Right swipe detected!");
        sprintf(json, "{\"on\":true}");
        sendPOST(json);
        break;
      case LEFT_SWIPE:
        Serial.println("Left swipe detected!");
        sprintf(json, "{\"on\":false}");
        sendPOST(json);
        break;
      default:
        break;
    }
  }
  
  // If there is position data available, read and print it
  if ( zx_sensor.positionAvailable() ) {
    x_pos = zx_sensor.readX();
    if ( x_pos != ZX_ERROR ) {
      Serial.print("X: ");
      Serial.print(x_pos);
    }
    z_pos = zx_sensor.readZ();
    if ( z_pos != ZX_ERROR ) {
      Serial.print(" Z: ");
      Serial.println(z_pos);
    }
    
    averagedZpos += z_pos;
    sampleSize++;

  }
  
  if(sampleSize > 10){
    
    averagedZpos /= sampleSize;
    
    // format json & insert z_pos variable
    sprintf(json, "{\"bri\":%u}", z_pos);
    sendPOST(json);
    
    sampleSize = 0;
  }
}