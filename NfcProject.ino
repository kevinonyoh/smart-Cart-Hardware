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



PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
LiquidCrystal_I2C lcd(0x27, 16, 2);


std::vector<std::string> myVector;


void setup(void) {
 Serial.begin(115200);
 nfc.begin();
 lcd.begin();
 
 lcd.backlight();
 lcd.print("Ensure to scan shopping card before shopping");
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

}
 
void loop(void) {
 
 if (nfc.tagPresent()){
  

  String val = pn532Value();
  
  std::string convertedVal = val.c_str();
 
    if(myVector.empty()){
         
         myVector.push_back(convertedVal);

         lcd.clear();
         lcd.print("Card scan");
         lcd.setCursor(0, 1);
         lcd.print("successfully");

    } else {
         
         std::string firstElement = myVector.front();
         String str2 = firstElement.c_str();

        if (val == str2 ) {

          lcd.clear();
         lcd.print("shopping");
         lcd.setCursor(0, 1);
         lcd.print("finished");
         delay(2000);
         
         lcd.clear();
         lcd.print("Transaction");
         lcd.setCursor(0, 1);
         lcd.print("processing");


           String concatenatedString = "{data:[";

         
          for (const auto& str : myVector) {
            concatenatedString += str.c_str();
            concatenatedString += ","; 
          }
          concatenatedString += "]}";
          
          Serial.println(concatenatedString);

        } else {

          myVector.push_back(convertedVal);
        
        int openBracePos = val.indexOf('{');
        int closeBracePos = val.indexOf('}');
        int commaPos = val.indexOf(',');

        String dataString = val.substring(openBracePos + 1, closeBracePos);
        String dataString2 = val.substring(openBracePos + 1, commaPos-1);
        String dataString3 = val.substring(commaPos + 1, closeBracePos-1);

        
         lcd.clear();
         lcd.print("product:"+dataString2);
         lcd.setCursor(0, 1);
         lcd.print("price:"+dataString3);

        }
        
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
        for (int c = 0; c < payloadLength; c++) 
        {
          payloadAsString += (char)payload[c];
        }

  
        String str = String(payloadAsString); 
        return str;  
      
      }

    }

 }
}



