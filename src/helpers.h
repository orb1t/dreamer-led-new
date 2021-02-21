#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

//#include "defs.h"
//#include "globals.h"
#include "SPIFFS.h"
#include "globals.h"

// #include <ArduinoJson.h>

template<typename Base, typename T>
inline bool instanceof(const T *) {
  return std::is_base_of<Base, T>::value;
}

byte setBit(byte b, int pos) {
  if (pos < 0 || pos > 7)
	return b;
  b |= (1 << pos);
  return b;
}

byte clearBit(byte b, int pos) {
  if (pos < 0 || pos > 7)
	return b;
  b &= ~(1 << pos);
  return b;
}

boolean isSet(byte b, int pos) {
  if (pos < 0 || pos > 7)
	return false;
  return (b >> pos & 1) == 1;
}

boolean isClear(byte b, int pos) {
  return !isSet(b, pos);
}

//int8_t evalFlag(uint8_t val, uint8_t flag){
//  int8_t  res = ( ( val >> ( flag - 1 ) ) && ( flag >> 1 > 0 ? flag >> 1 : 0 ) );
//  Serial.printf("\nevalFlag(%d, %d): %d\n", val, flag, res);
//  return res;
//}


static void show_free_mem(const char *pre=NULL) {
  if (pre) {
	Serial.println(pre);
  }
#ifdef ESP8266
  Serial.print( F("Heap Memory Available: ") ); Serial.println(system_get_free_heap_size());
#endif
#ifdef ESP32
  Serial.print("Heap/32-bit Memory Available     : ");
  Serial.print(heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
  Serial.print(" bytes total, ");
  Serial.print(heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL));
  Serial.println(" bytes largest free block");

  Serial.print("8-bit/malloc/DMA Memory Available: ");
  Serial.print(heap_caps_get_free_size(MALLOC_CAP_DMA));
  Serial.print(" bytes total, ");
  Serial.print(heap_caps_get_largest_free_block(MALLOC_CAP_DMA));
  Serial.println(" bytes largest free block");

  // https://thingpulse.com/esp32-how-to-use-psram/
#ifdef BOARD_HAS_PSRAM
  Serial.print("Total PSRAM used: ");
    Serial.print(ESP.getPsramSize() - ESP.getFreePsram());
    Serial.print(" bytes total, ");
    Serial.print(ESP.getFreePsram());
    Serial.println(" PSRAM bytes free");
#endif
#endif
}

void dumpHeap( const char* msg ){
  uint freeRAM = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
//		ESP_LOGI(TAG, "free RAM is %d.", freeRAM);
  Serial.printf("Free RAM is %d ( %s )\n", freeRAM, ( NULL != msg ? msg : "" ));
}

String sUUID(uint8_t len){
  String res;
  for ( uint8_t i = 0; i < len; i++ ){    
    res[i] = ( i % 2 == 0 ? (char)random(48, 57) : (char)random(97, 122) );
  }
    return res;
}

//funcPtrInfo funcPtrInfoBld(String name, int flag, funcPtr ptr){
//  funcPtrInfo res;
//  res.name = name;
//  res.flags = flag;
//  res.func = ptr;
//  return  res;
//}


// string: string to parse
// c: delimiter
// returns number of items parsed
String strings_for_split[10];


int split(String string, char del){
  Serial.printf("split(\"%s\", '%c'):\n", string.c_str(), del);
  String data = "";  int bufferIndex = 0;

  for (int i = 0; i < string.length(); ++i){
    char c = string[i];
    
    if (c != del){
      data += c;
    } else {
      data += '\0';
      #if DEBUG == 1
      Serial.printf("buf[%d]: %s\n", bufferIndex, data.c_str());
      #endif 
      strings_for_split[bufferIndex++] = data;
      data = "";
    }
  }
  if (string.length() > string.lastIndexOf(del)){
    data += '\0';
    #if DEBUG == 1
    Serial.printf("buf[%d]: %s\n", bufferIndex, data.c_str());
    #endif
    strings_for_split[bufferIndex++] = data;    
  }
  return bufferIndex;
}



const String CONFIG_IDF_TARGET = "esp32";

void dumpSystemInfo(){
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    Serial.println("Hardware info:");

    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
           CONFIG_IDF_TARGET.c_str(),
           chip_info.cores,
           String((chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "").c_str(),
           String((chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "").c_str());

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
           String((chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external").c_str());

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    //get chip id
    String chipId = String((uint32_t)ESP.getEfuseMac(), HEX);
    chipId.toUpperCase();

    Serial.printf("Chip id: %s\n", chipId.c_str());
}


//DynamicJsonDocument vectToJsonArray(std::vector<AuroraDrawable *> vect, JsonObject *root, String jsonArrayName){
//
//};



//DynamicJsonDocument vectToJsonArray(std::vector<T>* vect, String jsonArrayName) {
//  // compute the required size
//  const size_t CAPACITY = JSON_ARRAY_SIZE(vect.size())*32;
//
//// allocate the memory for the document
//  DynamicJsonDocument doc(CAPACITY);
//
//// create an empty array
//  JsonArray array = doc.to<JsonArray>();
//
//// add some values
////  array.add("hello");
////  array.add(42);
////  array.add(3.14);
////
////// serialize the array and send the result to Serial
////  serializeJson(doc, Serial);
////
////  JsonArray arr = ((JsonObject)*root).createNestedArray(jsonArrayName);
//  for (int i = 0; i < vect.size(); i++) {
//	Serial.println(vect[i]->name);
//	array.add(vect[i]->name);
//  }
//  return doc;
//}


#endif
