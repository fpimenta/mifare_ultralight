/*
   Typical pin layout used:
   -----------------------------------------------------------------------------------------
               MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
               Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin          Pin           Pin       Pin        Pin              Pin
   -----------------------------------------------------------------------------------------
   RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
   SPI SS      SDA(SS)      10            53        D10        10               10
   SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
   SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
   SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
*/
#include "Arduino.h"
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(9600);		// Initialize serial communications with the PC
  while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();			// Init SPI bus
  mfrc522.PCD_Init();		// Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
  Serial.println("Select 1 or 2:");
  Serial.println("1. Read");
  Serial.println("2. Write");
}

void readUltralight() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) return;

  // Dump debug info about the card; PICC_HaltA() is automatically called
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}

// New code
void writeUltralight() {

  MFRC522::StatusCode status;

  // Hex info of the card                           
  uint16_t *hexbytes[16] = [040244CA,
                           C2784B81,
                           70480800,
                           06040100,
                           0010378C,
                           20402080,
                           68DD0A9F,
                           DEED79A8,
                           16FED385,
                           D9D83E6A,
                           B83E5152,
                           FA6E737E,
                           541E9BCC,
                           61B83DCE,
                           BA600466,
                           D678DFBF];


  Serial.print("Writing");                          
  for (int page = 0; page < 16; page++) {
    Serial.print(page); Serial.println(" successfully written");
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Ultralight_Write(page, hexbytes[page], 4);
//    if (status != MFRC522::STATUS_OK) {
//      Serial.println("");
//      Serial.print(F("MIFARE_Write() failed: ")); Serial.println(mfrc522.GetStatusCodeName(status));
//      delay(100);
//      return;
//    }
//    else{
//      Serial.println("");
//      Serial.println("Page" + page + "sent.");
//    }
  }
  if ((page == 16) && (status == MFRC522::STATUS_OK)) Serial.println("Successfull");
  else {
    Serial.println("");
    Serial.print("MIFARE_Write failed: "); Serial.println(mfrc522.GetStatusCodeName(status));
  }
  delay(100);
  
}
// End of new code

void loop() {

  char ch = Serial.read();

  if (ch == "1") readUltralight();
  else if (ch == "2") writeUltralight();
  else if (ch == "");
  else {
    Serial.println();
    Serial.println ("Not found");
    Serial.println();
    Serial.println("Select 1 or 2:");
    Serial.println("1. Read");
    Serial.println("2. Write");
  }

  ch = "";
  delay(100);
}
