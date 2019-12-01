#include <OneWire.h>
#include <WiFi.h>
#include<HTTPClient.h>
#include <SPI.h>

byte mac[] = { 0x44, 0x03, 0x2C, 0x49, 0x86, 0x26 };
byte ip[] = { 10, 183, 98, 106 };
byte server[] = { 10, 183, 71, 254 };
OneWire ds(13); // on pin 3

const char* ssid = "ASUS";
const char* password = "1234567890";

void setup(void) {
  //Ethernet.begin(mac, ip);
  Serial.begin(9600);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
}
 
void loop(void) {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;
  if ( !ds.search(addr)) {
      ds.reset_search();
      return;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end
  delay(10000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);         // Read Scratchpad
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i]);
    Serial.print(" ");
  }
  Serial.print("\n");
  //Serial.print(data);
  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  // test most sig bit
  if (SignBit) // negative
  {
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25
 
  Whole = Tc_100 / 100;  // separate off the whole and fractional portions
  Fract = Tc_100 % 100;
 
//  if (SignBit) // If its negative
//  {
//     Serial.print("-");
//  }
//  Serial.print(Whole);
//  Serial.print(".");
//  if (Fract < 10)
//  {
//     Serial.print("0");
//  }
//  Serial.print(Fract);

  String Temperature = (String) Whole + "."
                + (String) Fract;
  Serial.print(Temperature);
  Serial.print("\n");
  HTTPClient http;
  String Request = "http://192.168.1.74:8080/NeerMonitor/api/neer/setTemp?deviceId=asdf&temp="
                    + Temperature
                    + "&counter=25.55"; 
  http.begin(Request);
  http.addHeader("Content-Type", "application/json"); //Specify content-type header
  int httpCode = http.POST(""); 
  Serial.println(httpCode);
  if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
 
  }else {
 
      Serial.println("Error on HTTP request");
 
  }
  http.end();
  
}
