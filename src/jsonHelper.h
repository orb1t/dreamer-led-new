//
// Created by orb1t on 09.12.20.
//

#ifndef INC_201122_212358_ESP32DEV_JSONHELPER_H
#define INC_201122_212358_ESP32DEV_JSONHELPER_H

#include <Game.h>



//template<typename Base, typename T>
char* vectorToJsonArray( mediaTypeInfo* vector, const char* name, AsyncResponseStream* strm) {
  const size_t CAPACITY = JSON_ARRAY_SIZE(vector->arr.size()) * 2; // allocate the memory for the document
  DynamicJsonDocument doc(CAPACITY);
  JsonArray array;// = NULL;

  if ( NULL != name ) {
	  array = doc.createNestedArray(name);
  } else {
	  array = doc.to<JsonArray>();// create an empty array
  }
  for (String* name : vector->arr){
	  array.add( String(vector->path) + "/" + *name + vector->ext);
  }

  char* buf = NULL;
  if ( NULL != strm ){
  	serializeJson(doc, *strm);
  } else {
    int len1 = measureJson(doc); // Compute the length of the minified JSON document
    len1 *= 1.2;
    buf = new char(len1);
    serializeJson(doc, buf, len1-1);// Produce a minified JSON document
  }
  doc.clear();
  return buf;
}

char* vectorToJsonArray(std::vector<Game*> vector, const char* name, AsyncResponseStream* strm) {
  const size_t CAPACITY = JSON_ARRAY_SIZE(vector.size()) * 2;
  DynamicJsonDocument doc(CAPACITY);
  JsonArray array;// = NULL;
  if ( NULL != name ) {
	  array = doc.createNestedArray(name);
  } else {
	// create an empty array
	  array = doc.to<JsonArray>();//jsonBuffer
  }
  for (Game* item : vector){
    Serial.println(item->name);
  	array.add( item->name );
  }
  // Compute the length of the minified JSON document
//  int len1 = measureJson(doc);
//  char* buf = new char(len1);
//  // Produce a minified JSON document
//  serializeJson(doc, buf);
//  return buf;
  char* buf = NULL;
  if ( NULL != strm ){
	serializeJson(doc, (std::ostream&)strm);
  } else {
    int len1 = measureJson(doc);// Compute the length of the minified JSON document
    len1 *= 1.2;
    buf = new char(len1);
    serializeJson(doc, buf, len1-1);// Produce a minified JSON document
  }
  doc.clear();
  return buf;
}


char* vectorToJsonArray(std::vector<AuroraDrawable*> vector, const char* name, AsyncResponseStream* strm){
  const size_t CAPACITY = JSON_ARRAY_SIZE(vector.size())*2;
  DynamicJsonDocument doc(CAPACITY);
  JsonArray array;// = NULL;

  if ( NULL != name ) {
	 array = doc.createNestedArray(name);
  } else {
	  array = doc.to<JsonArray>();// create an empty array
  }
  for (AuroraDrawable* item : vector){
    Serial.println(item->name);
    array.add( item->name );
  }

  char* buf = NULL;
  if ( NULL != strm ){
	  serializeJson(doc, (std::ostream&)strm);
  } else {
	  int len1 = measureJson(doc);// Compute the length of the minified JSON document
	  len1 *= 1.2;
	  buf = new char(len1);
	  serializeJson(doc, buf, len1-1); // Produce a minified JSON document
  }
  doc.clear();
  return buf;
}

#endif //INC_201122_212358_ESP32DEV_JSONHELPER_H
