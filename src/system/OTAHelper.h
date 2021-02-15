#ifndef _OTAHELPER_H_
#define _OTAHELPER_H_

#include "SPIFFS.h"
#include <Update.h>


void updateFW(){
  File file = SPIFFS.open("/fw.bin");
  if(file){
    Serial.println("Got /fw.bin starting update process");
//    Serial.println("Starting update..");        
    
    size_t fileSize = file.size();
    
    if(!Update.begin(fileSize)){       
       Serial.println("Cannot do the update");
       return;
    };
    
    Update.writeStream(file);
    
    if(Update.end()){       
      Serial.println("Successful update");  
    }else {       
      Serial.println("Error Occurred: " + String(Update.getError()));
      return;
    }
     
    file.close();
    SPIFFS.remove("/fw.bin");
    
    Serial.println("Reset in 4 seconds...");
    delay(4000);
    
    ESP.restart();
      
  } else {
    Serial.println("No /fw.bin for update");
  }  
}

 #endif
