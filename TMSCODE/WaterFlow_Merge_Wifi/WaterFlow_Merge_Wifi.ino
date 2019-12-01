#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306();
#define BUTTON_A 15
#define BUTTON_B 32
#define BUTTON_C 14
#define LED      13

#if (SSD1306_LCDHEIGHT != 32)
 #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

OneWire ds(13);

const char* ssid = "Muthu";
const char* password = "1234567890";

byte sensorInterrupt = 25;  // 0 = digital pin 2
byte sensorPin       = 14;
int ZeroCounter = 0;
float TotalLitres = 0;

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 8;

volatile byte pulseCount;  

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

unsigned long oldTime;

void setup()
{
  
  // Initialize a serial connection for reporting values to the host
  Serial.begin(9600); 
Serial.println("OLED FeatherWing test");
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  Serial.println("OLED begun");
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.setCursor(0,0);
  display.display();
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    display.clearDisplay();
    display.setCursor(0,0);
    Serial.println("Connecting to WiFi..");
    display.println("Connecting to WiFi..");
    display.display();
    delay(2000);
  }
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("WiFi network Connected "+ (String) ssid);
  display.display();
  Serial.println("Connected to the WiFi network");
  delay(2000);
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  
  
  Serial.println("IO test");

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP); 
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  pinMode(sensorInterrupt, INPUT_PULLUP);
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
}

/**
 * Main program loop
 */
void loop()
{
   
   if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    // Disable the interrupt while calculating flow rate and sending the value to
    // the host
    detachInterrupt(sensorInterrupt);
        
    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    
    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    oldTime = millis();
    
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;
    
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
      
    unsigned int frac;
    
    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print(".");             // Print the decimal point
    // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
    frac = (flowRate - int(flowRate)) * 10;
    Serial.print(frac, DEC) ;      // Print the fractional part of the variable
    Serial.print("L/min");
    // Print the number of litres flowed in this second
    Serial.print("  Current Liquid Flowing: ");             // Output separator
    Serial.print(flowMilliLitres);
    Serial.print("mL/Sec");

    // Print the cumulative total of litres flowed since starting
    Serial.print("  Output Liquid Quantity: ");             // Output separator
    Serial.print(totalMilliLitres);
    Serial.println("mL"); 

    // Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

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
//  delay(10000);     // maybe 750ms is enough, maybe not
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
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Temperature: "+Temperature+(char) 247+"C");
  display.print("Water Used: ");
  display.print(totalMilliLitres);
  display.print("ml");
  display.setCursor(0,0);
  yield();
  display.display();
  display.clearDisplay();
  if( flowRate == 0)
  {
    ZeroCounter++;
  }
  else
  {
    ZeroCounter = 0;
  }
  if (ZeroCounter >= 5 && totalMilliLitres !=0)
  {
    TotalLitres = (float)totalMilliLitres/1000;
    Serial.println(TotalLitres);
    HTTPClient http;
  String Request = "http://192.168.43.162:8080/NeerMonitor/api/neer/setUpdate?deviceId=franklu&temp="
                    + Temperature
                    + "&counter="+ (String) (TotalLitres); 
  http.begin(Request);
  http.addHeader("Content-Type", "application/json"); //Specify content-type header
  int httpCode = http.POST(""); 
  Serial.println(httpCode);
  if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
        display.clearDisplay();
        display.setCursor(0,0);
        display.println("Data Sent to Server");
        display.display();  
  }else {
      display.clearDisplay();
      display.setCursor(0,0);
      Serial.println("Error on HTTP request");
      display.println("Error on HTTP request");
      display.display(); 
  }
  http.end();
  totalMilliLitres = 0;
  }
  }
}

/*
Insterrupt Service Routine
 */
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
