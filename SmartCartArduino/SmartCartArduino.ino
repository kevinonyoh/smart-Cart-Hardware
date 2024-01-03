//including the library for I2C communication
#include <Wire.h>
//including the library for I2C communication with the PN532 module, library for the PN532 module and the library for the NFC
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <ArduinoSTL.h>
#include <vector>
#include <string>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>



PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
LiquidCrystal_I2C lcd(0x27, 16, 2);


String cardValue;
std::vector<std::string> myVector;

float total = 0;
int i= 0;


void setup(void) {
 
 nfc.begin();
 lcd.begin();

 
 

 
 
 lcd.backlight();
 lcd.print("Ensure to scan");
 lcd.setCursor(0, 1);
 lcd.print("shopping card");
 delay(2000);
 lcd.clear();

 lcd.print("before shopping");
 delay(2000);
 lcd.clear();

 lcd.print("Have a great");
 lcd.setCursor(3, 1);
 lcd.print("time!");
 
Serial.begin(9600);
Serial.flush();
}
 
void loop(void) {

  if( i == 0){
 
 if (nfc.tagPresent()){
  

  String val = pn532Value();
  

int commaPosition = val.indexOf(',');
String firstPart = val.substring(0, commaPosition);
String secondPart = val.substring(commaPosition + 1);

 
    if(myVector.empty()){
         cardValue = val;
          String solu = "\""+firstPart+"\"";
          String cardJson = solu;
          std::string convertedVal = cardJson.c_str();
          
            myVector.push_back(convertedVal);

         lcd.clear();
         lcd.print("Card scan");
         lcd.setCursor(0, 1);
         lcd.print("successfully");

    } else {
         
         
        if (val == cardValue ) {

          lcd.clear();
         lcd.print("shopping");
         lcd.setCursor(0, 1);
         lcd.print("finished");
         delay(2000);


         
         lcd.clear();
         lcd.print("Transaction");
         lcd.setCursor(0, 1);
         lcd.print("processing");
          delay(2000);


          String concatenatedString = "{\"data\":[";
           
           
         
          for (const auto& str : myVector) {

             concatenatedString += str.c_str();
              concatenatedString += ","; 

          }
          String finalResult = (String)total;
          String sum = "\""+finalResult+"\"";
          concatenatedString += sum+"]}";
        
          Serial.println(concatenatedString);

            i=1;

        } else {
          String ProductJson = "\""+firstPart+"\"";
          
          
          
          std::string convertedVal = ProductJson.c_str();
          myVector.push_back(convertedVal);
          total += secondPart.toFloat();
       
         lcd.clear();
         lcd.print("product:"+firstPart);
         lcd.setCursor(0, 1);
         lcd.print("price:"+secondPart);

        }
        
    }
  
     
  }
  }else {
 if (Serial.available() > 0) {
            
            String receivedData = Serial.readStringUntil('\n'); 
             
             int position = receivedData.indexOf(' ');
             String part1 = receivedData.substring(0, position);
             String part2 = receivedData.substring(position + 1);

             lcd.clear();
             lcd.print(part1);
             lcd.setCursor(0, 1);
             lcd.print(part2);
             delay(10000);

             myVector.clear();
             total = 0;
             cardValue = "";
             i=0;
             setup();
             
             
    }
  }
   delay(2000);
     
 }



String pn532Value() {
    
 if (nfc.tagPresent()){
  NfcTag tag = nfc.read();
  
  if (tag.hasNdefMessage())
    {
      
      NdefMessage message = tag.getNdefMessage();
      int recordCount = message.getRecordCount();
      for (int i = 0; i < recordCount; i++)
      {
        
        NdefRecord record = message.getRecord(i);

        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);


        String payloadAsString = ""; 
        for (int c = 3; c < payloadLength; c++) 
        {
          payloadAsString += (char)payload[c];
        }

  
        String str = String(payloadAsString); 
        return str;  
      
      }

    }

 }
}


