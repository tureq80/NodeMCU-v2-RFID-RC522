#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <SPI.h>
#include "MFRC522.h"
#include <ESP8266HTTPClient.h>

/* wiring the MFRC522 to ESP8266 (ESP-12) PIN CONNECTED
RST     = GPIO5   -> D1
SDA(SS) = GPIO4   -> D2
MOSI    = GPIO13  -> D7
MISO    = GPIO12  -> D6
SCK     = GPIO14  -> D5
GND     = GND     -> GND
3.3V    = 3.3V    -> 3.3V
*/

#define RST_PIN  5  // RST-PIN for RC522 - RFID - SPI - Modul GPIO5 
#define SS_PIN  4  // SDA-PIN for RC522 - RFID - SPI - Modul GPIO4 

const char *ssid =  "WIFISSID";     // change to your Network - cannot be longer than 32 characters!
const char *pass =  "PASSWORDWIFI"; // change to your password

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);    // Initialize serial communications
  delay(250);
  Serial.println(F("Booting...."));
  
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
  
  WiFi.begin(ssid, pass);

  
  int retries = 0;
  while ((WiFi.status() != WL_CONNECTED) && (retries < 10)) {
    retries++;
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    //Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("WiFi connected"));
   digitalWrite(LED_BUILTIN, HIGH);
  }
  
  Serial.println(F("Ready!"));
  Serial.println(F("Scan Card:"));
  
}

void loop() { 
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

HTTPClient http;
String content= "";
byte letter;

// Helper routine to dump a byte array as hex values to Serial

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
    
  }



Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "AA BB CC DD") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("A ON");
    Serial.println();
    http.begin("http://url.com/code/ID1"); //HTTP
    int httpCode = http.GET();
    http.end();
    delay(2000);
  }
 
if (content.substring(1) == "EE FF 00 00")  {
    Serial.println("A OFF");
     http.begin("http://url.com/code/ID2"); //HTTP
     int httpCode = http.GET();
     http.end();
    delay(2000);
  }



}
