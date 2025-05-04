#include <LiquidCrystal_PCF8574.h>  //I2C library
#include <Adafruit_PN532.h>   //NFC library 
#include <Wire.h>  // enables communication with I2C devices
#include "DFRobotDFPlayerMini.h"   //MP3 player library for DFPlayer Mini

#define SWITCH_ONE 14
#define SWITCH_TWO 13
#define SWITCH_THREE 12
#define SWITCH_FOUR 18
#define SWITCH_FIVE 32
#define SWITCH_SIX 15
#define ENTER_BUTTON 4
#define SUBMIT_BUTTON 5
#define UNDO_BUTTON 2
#define SDA_PIN 21
#define SCL_PIN 22
#define DFPLAYER_RX 17   // Connect to DFPlayer RX
#define DFPLAYER_TX 16   // Connect to DFPlayer TX

HardwareSerial mySerial(1);  //Use UART1 for DFPlayer
DFRobotDFPlayerMini player;

LiquidCrystal_PCF8574 lcd(0x27);  // 20x4 LCD at address 0x27
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

String lastTagID = "";

bool switchOneState = false;
bool switchTwoState = false;
bool switchThreeState = false;
bool switchFourState = false;
bool switchFiveState = false;
bool switchSixState = false;
bool enterPressed = false;
bool submitPressed = false;
bool undoPressed = false;

String lastReadUID = "";
String storedOutput = "";
String nfcvalue= "";
bool nfcPrinted = false; // Track if NFC value is printed

void setup() {
    Serial.begin(115200);

    mySerial.begin(9600, SERIAL_8N1, DFPLAYER_RX, DFPLAYER_TX);
    delay(1000);
    
    if (player.begin(mySerial)) {
        Serial.println("DFPlayer Mini Ready...");
        player.volume(20);  // Set volume (0-30)
    } else {
        Serial.println("DFPlayer Mini Initialization Failed!");
    }

    Wire.begin(SDA_PIN, SCL_PIN);
    lcd.begin(20, 4);
    lcd.setBacklight(1);  // Turn on backlight (0 to turn off)
    lcd.setCursor(0, 0);
    lcd.print("Braille Input:");

    // Initialize PN532 NFC Module
    nfc.begin();
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        Serial.println("NFC Module Not Found!");
        lcd.setCursor(0, 1);
        lcd.print("NFC Error!");
    }
    nfc.SAMConfig();


    pinMode(SWITCH_ONE, INPUT_PULLUP);
    pinMode(SWITCH_TWO, INPUT_PULLUP);
    pinMode(SWITCH_THREE, INPUT_PULLUP);
    pinMode(SWITCH_FOUR, INPUT_PULLUP);
    pinMode(SWITCH_FIVE, INPUT_PULLUP);
    pinMode(SWITCH_SIX, INPUT_PULLUP);
    pinMode(ENTER_BUTTON, INPUT_PULLUP);
    pinMode(SUBMIT_BUTTON, INPUT_PULLUP);
    pinMode(UNDO_BUTTON, INPUT_PULLUP);

    Serial.println("NFC Reader Ready...");
}

void loop() {
  // Detect switch states and print them
  if (digitalRead(SWITCH_ONE) == LOW && !switchOneState) { switchOneState = true; Serial.println("Switch One: ON"); }
  if (digitalRead(SWITCH_TWO) == LOW && !switchTwoState) { switchTwoState = true; Serial.println("Switch Two: ON"); }
  if (digitalRead(SWITCH_THREE) == LOW && !switchThreeState) { switchThreeState = true; Serial.println("Switch Three: ON"); }
  if (digitalRead(SWITCH_FOUR) == LOW && !switchFourState) { switchFourState = true; Serial.println("Switch Four: ON"); }
  if (digitalRead(SWITCH_FIVE) == LOW && !switchFiveState) { switchFiveState = true; Serial.println("Switch Five: ON"); }
  if (digitalRead(SWITCH_SIX) == LOW && !switchSixState) { switchSixState = true; Serial.println("Switch Six: ON"); }

  // Detect enter button press
  if (digitalRead(ENTER_BUTTON) == LOW && !enterPressed) {
      enterPressed = true;
      delay(500);

      String output = "";
      if (switchOneState && !switchTwoState && !switchThreeState && !switchFourState && !switchFiveState && !switchSixState) { Serial.println("Output: A"); output = "A"; player.playFolder(1, 1);}  // Play 'A' from folder "01"
      else if (switchOneState && switchTwoState && !switchThreeState && !switchFourState && !switchFiveState && !switchSixState) { Serial.println("Output: B"); output = "B"; player.playFolder(1, 2);}  // Play 'A' from folder "01"
      else if (switchOneState && !switchFourState && !switchTwoState && switchThreeState && !switchFiveState && !switchSixState) { Serial.println("Output: C"); output = "C"; player.playFolder(1, 3);}
      else if (switchOneState && switchFourState && switchFiveState && !switchTwoState && !switchThreeState && !switchSixState) { Serial.println("Output: D"); output = "D"; player.playFolder(1, 4);}
      else if (switchOneState && switchFiveState && !switchTwoState && !switchThreeState && !switchFourState && !switchSixState) { Serial.println("Output: E"); output = "E"; player.playFolder(1, 5);}
      else if (switchOneState && switchTwoState && switchFourState && !switchThreeState && !switchFiveState && !switchSixState) { Serial.println("Output: F"); output = "F"; player.playFolder(1, 6);}
      else if (switchOneState && switchTwoState && switchFourState && switchFiveState && !switchThreeState && !switchSixState) { Serial.println("Output: G"); output = "G"; player.playFolder(1, 7);}   
      else if (switchOneState && switchTwoState && switchFiveState && !switchThreeState && !switchFourState && !switchSixState) { Serial.println("Output: H"); output = "H"; player.playFolder(1, 8);}
      else if (switchTwoState && switchFourState && !switchOneState && !switchThreeState && !switchFiveState && !switchSixState) { Serial.println("Output: I"); output = "I"; player.playFolder(1, 9);}
      else if (switchTwoState && switchFourState && switchFiveState && !switchOneState && !switchThreeState && !switchSixState) { Serial.println("Output: J"); output = "J"; player.playFolder(1, 10);}
      else if (switchOneState && switchThreeState && !switchTwoState && !switchFourState && !switchFiveState && !switchSixState) { Serial.println("Output: K"); output = "K"; player.playFolder(1, 11);}
      else if (switchOneState && switchTwoState && switchThreeState && !switchFourState && !switchFiveState && !switchSixState) { Serial.println("Output: L"); output = "L"; player.playFolder(1, 12);}
      else if (switchOneState && switchThreeState && switchFourState && !switchTwoState && !switchFiveState && !switchSixState) { Serial.println("Output: M"); output = "M"; player.playFolder(1, 13);}
      else if (switchOneState && switchThreeState && switchFourState && switchFiveState && !switchTwoState && !switchSixState) { Serial.println("Output: N"); output = "N"; player.playFolder(1, 14);}
      else if (switchOneState && switchThreeState && switchFiveState && !switchTwoState && !switchFourState && !switchSixState) { Serial.println("Output: O"); output = "O"; player.playFolder(1, 15);}
      else if (switchOneState && switchTwoState && switchThreeState && switchFourState && !switchFiveState && !switchSixState) { Serial.println("Output: P"); output = "P"; player.playFolder(1, 16);}
      else if (switchOneState && switchTwoState && switchThreeState && switchFourState && switchFiveState && !switchSixState) { Serial.println("Output: Q"); output = "Q"; player.playFolder(1, 17);}
      else if (switchOneState && switchTwoState && switchThreeState && switchFiveState && !switchFourState && !switchSixState) { Serial.println("Output: R"); output = "R"; player.playFolder(1, 18);}
      else if (switchTwoState && switchThreeState && switchFourState && !switchOneState && !switchFiveState && !switchSixState) { Serial.println("Output: S"); output = "S"; player.playFolder(1, 19);}
      else if (switchTwoState && switchThreeState && switchFourState && switchFiveState && !switchOneState && !switchSixState) { Serial.println("Output: T"); output = "T"; player.playFolder(1, 20);}
      else if (switchOneState && switchThreeState && switchSixState && !switchTwoState && !switchFourState && !switchFiveState) { Serial.println("Output: U"); output = "U"; player.playFolder(1, 21);}
      else if (switchOneState && switchTwoState && switchThreeState && switchSixState && !switchFourState && !switchFiveState) { Serial.println("Output: V"); output = "V"; player.playFolder(1, 22);}
      else if (switchTwoState && switchFourState && switchFiveState && switchSixState && !switchOneState && !switchThreeState) { Serial.println("Output: W"); output = "W"; player.playFolder(1, 23);}
      else if (switchOneState && switchThreeState && switchFourState && switchSixState && !switchTwoState && !switchFiveState) { Serial.println("Output: X"); output = "X"; player.playFolder(1, 24);}
      else if (switchOneState && switchThreeState && switchFourState && switchFiveState && switchSixState && !switchTwoState) { Serial.println("Output: Y"); output = "Y"; player.playFolder(1, 25);}
      else if (switchOneState && switchThreeState && switchFiveState && switchSixState && !switchTwoState && !switchFourState) { Serial.println("Output: Z"); output = "Z"; player.playFolder(1, 26);}

      
      if (output != "") {
        if (nfcvalue != "") {
            storedOutput += output;
            Serial.println("Stored Output: " + storedOutput);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Stored: ");
            lcd.setCursor(0, 1);
            lcd.print(storedOutput);
        } else {
            Serial.println("Output: "+ output);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Output: ");
            lcd.setCursor(0, 1);
            lcd.print(storedOutput);
        }
      } else {
        player.playFolder(1, 32);  // Wrong combination
        Serial.println("Output: Wrong Combination");
      }

      switchOneState = switchTwoState = switchThreeState = false;
      switchFourState = switchFiveState = switchSixState = false;
  }

  // Handle undo button press
  if (digitalRead(UNDO_BUTTON) == LOW && !undoPressed) {
    undoPressed = true;
    delay(500);

    if (storedOutput.length() > 0) {
      storedOutput.remove(storedOutput.length() - 1);
      Serial.println("Undo: " + storedOutput);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Stored: ");
      lcd.setCursor(0, 1);
      lcd.print(storedOutput);
    }
  }
    // Handle submit button press
  if (digitalRead(SUBMIT_BUTTON) == LOW && !submitPressed) {
    submitPressed = true; 
    delay(500);

    Serial.println("Final Stored Output: " + storedOutput);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Final: ");
    lcd.setCursor(0, 1);
    lcd.print(storedOutput);

    if (nfcvalue == "") {
        // No NFC scanned yet
        lcd.setCursor(0, 2);
        lcd.print("Scan NFC Tag!");
        player.playFolder(1, 33);  // audio "Scan the card"
    } else if (storedOutput == nfcvalue) {
        // Correct match
        if (storedOutput == "APPLE") {
            lcd.setCursor(0, 2); lcd.print("Apple!");
            player.playFolder(1, 27);
        } else if (storedOutput == "ORANGE") {
            lcd.setCursor(0, 2); lcd.print("Orange!");
            player.playFolder(1, 28);
        } else if (storedOutput == "PINEAPPLE") {
            lcd.setCursor(0, 2); lcd.print("Pineapple!");
            player.playFolder(1, 29);
        } else if (storedOutput == "GRAPES") {
            lcd.setCursor(0, 2); lcd.print("Grapes!");
            player.playFolder(1, 30);
        }
    } else {
        // Mismatch
        lcd.setCursor(0, 2);
        lcd.print("Wrong Guess!");
        player.playFolder(1, 31);
    }
    storedOutput = "";
  }
  // Reset button states
  if (digitalRead(SUBMIT_BUTTON) == HIGH) submitPressed = false;
  if (digitalRead(ENTER_BUTTON) == HIGH) enterPressed = false;
  if (digitalRead(UNDO_BUTTON) == HIGH) undoPressed = false;

  checkForNfcTag();
}

void checkForNfcTag() {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 50); // timeout = 50ms

  if (success) {
    String tagID = "";
    for (int i = 0; i < uidLength; i++) {
      tagID += String(uid[i], HEX);
    }

    // If same tag as last time, ignore
    if (tagID == lastTagID) return;
    lastTagID = tagID;  // update last seen tag

    Serial.print("Scanned NFC Tag ID: ");
    Serial.println(tagID);

    // Tag handling
    if (tagID == "1d526e0b1080") {
      nfcvalue = "APPLE";
      lcd.clear(); lcd.setCursor(0, 0); lcd.print("NFC: APPLE");
    } else if (tagID == "1263ce55") {
      nfcvalue = "ORANGE";
      lcd.clear(); lcd.setCursor(0, 0); lcd.print("NFC: ORANGE");
    } else if (tagID == "1dc46e0b1080") {
      nfcvalue = "PINEAPPLE";
      lcd.clear(); lcd.setCursor(0, 0); lcd.print("NFC: PINEAPPLE");
    } else if (tagID == "52d36655") {
      nfcvalue = "GRAPES";
      lcd.clear(); lcd.setCursor(0, 0); lcd.print("NFC: GRAPES");
    } else {
      lcd.clear(); lcd.setCursor(0, 0); lcd.print("Unknown Tag");
    }

    delay(1000);  // pause after scan
  } else {
    // If no tag is present, reset lastTagID to allow detection again
    lastTagID = "";
  }
}
