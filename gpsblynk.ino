// khai báo thư viện 
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
// thiế lập kết nối app blynk + internet
char auth[] = "VlZnuXhZs30CNlqbGeLN8oJopJPXpqsu"; // Blynk authentication key
char ssid[] = "PTB_TV"; // Name of your WiFi SSID
char pass[] = "HCMUTE18146"; // WiFi Password
char server[] = "blynk-cloud.com";
#define buttun D7 // khai báo nút nhấn chân D7
WidgetLCD lcd(V1); // V1 hiển thị LCD 
WidgetMap myMap(V0); // V0 hiển thị vị tri trên ản đồ 
String GPSLabel = "BLYNK"; //Labeling location on MAP
SimpleTimer timer;
static const int RXPin = 5, TXPin = 4;   // GPIO 4=D2(két nối Tx của GPS) và GPIO 5=D1(kết nôi Rx of GPS)
static const uint32_t GPSBaud = 9600; 
TinyGPSPlus gps;                           
SoftwareSerial ss(RXPin, TXPin);     // Serial connection to the GPS module

void setup() {
  Serial.begin(9600);       
  ss.begin(GPSBaud);
  pinMode(buttun, INPUT);
  //Connect Blynk
  Blynk.begin(auth, ssid, pass, server, 8080);
  //Serial.println("Activating GPS");
  timer.setInterval(1000L, periodicUpdate);
  timer.setInterval(60*1000, reconnectBlynk);
}

//Show GPS lat and lng on LCD
void periodicUpdate() {
  String line1, line2;
  //LCD
  lcd.clear();
  if (gps.location.isValid() && (gps.location.age() < 3000)) {
    //position current
    line1 = String("lat: ") + String(gps.location.lat(), 6);
    line2 = String("lng: ") + String(gps.location.lng(), 6);
    lcd.print(0, 0, line1);
    lcd.print(0, 1, line2);
    //update location on map
    myMap.location(2, gps.location.lat(), gps.location.lng(), GPSLabel);
  } else {
    //position is lost
    lcd.print(0, 0, "Chờ kết nối GPS");
  }
}

void updateGPS() {
  //read data from GPS module
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }
}

void reconnectBlynk() {
  if (!Blynk.connected()) {
    Serial.println("Lost connection");
    if(Blynk.connect()) Serial.println("Reconnected");
    else Serial.println("Not reconnected");
  }
}

void loop() {
  timer.run();
  butunValue();
  if(Blynk.connected()) { Blynk.run(); }
  updateGPS();
}
void butunValue(void)
{
  if (digitalRead(buttun)) 
  { 
    Blynk.notify("Cảnh báo nguy hiểm khẩn cấp !!!");  
  }
}
