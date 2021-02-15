//
// Created by orb1t on 08.12.20.
//

#ifndef _MEDIAHELPER_H_
#define _MEDIAHELPER_H_

#include "../system/SPIFFS_functions.h"

int numberOfFiles;

#define DEBUG 1

uint8_t calcMediaScale(uint16_t mediaWidth, uint16_t mediaHeigth, uint16_t matrixWidth, uint16_t matrixHeigth)  {
  int res = 1;
  for (res = 1; res <= 8; res <<= 1) {
	if (mediaWidth <= matrixWidth * res && mediaHeigth <= matrixHeigth * res) break;
  }
  return res;
};

bool isMediaFile(const char *filename, const char* mediaext) {
  String filenameString(filename);
  String ext(mediaext);
  Serial.printf("\nisMediaFile: filename: %s, mediaext: %s\n", filename, mediaext);

  filenameString.toUpperCase();
  ext.toUpperCase();
  if ( ! filenameString.endsWith( ext ) )
	return false;
  else
  	return true;

#if defined(ESP32)
  // ESP32 filename includes the full path, so need to remove the path before looking at the filename
  int pathindex = filenameString.lastIndexOf("/");
  if (pathindex >= 0)
	filenameString.remove(0, pathindex + 1);
#endif

  if ((filenameString[0] == '_') || (filenameString[0] == '~') || (filenameString[0] == '.')) {
	return false;
  }

  filenameString.toUpperCase();
  ext.toUpperCase();
  if (filenameString.endsWith(ext) != 1)
	return false;

  return true;
}
// Enumerate and possibly display the animated GIF filenames in GIFS directory

void dumpMediaTypeInfo(mediaTypeInfo* info){
	Serial.printf("\nMedia Ext: %s, path: %s, conunt %d:\n", info->ext, info->path, info->arr.size());
	for ( int i = 0; i < info->arr.size(); i++ ){
	  Serial.printf("\nItem[%d]: %s\n", i, info->arr[i]->c_str());
	}
}

int16_t getMediaIdxInTypeInfo(const mediaTypeInfo* nfo, const String* name){
  int16_t res = -1;

  if ( nfo->arr.size() <= 0 ) {
#if DEBUG == 1
	Serial.printf("no items in vector\n");
#endif
  } else {
#if DEBUG == 1
	String fname = name->substring(0, name->lastIndexOf('.'));
	Serial.printf("%d items in vector, looking for: %s\n", nfo->arr.size(), fname.c_str());
//	dumpMediaTypeInfo((mediaTypeInfo*)nfo);
#endif
	for (int i=0; i<nfo->arr.size(); i++) {
#if DEBUG == 1
	  Serial.printf("%d: ( %s == %s ) == %s\n", i, nfo->arr[i]->c_str(), fname.c_str(), ((String) nfo->arr[i]->c_str() == fname ? "true" : "false" ));
#endif
//	  nfo->arr[i]->c_str()
//	  cstr
//	  if ( ((String)*nfo->arr[i]).equals(fname) )
	  if ( (String) nfo->arr[i]->c_str() == fname )
		return i;
//	  printItem(i, nfo->arr[i]);
//      Serial.println(vect[i]);
	}
  }

  return res;
}


uint8_t MediaFileCounter = 0;
uint8_t __IDX = 0;
String __FOUND_FILE = "";
bool  __FOUND_FLAG = false;
mediaTypeInfo* __INFO = NULL;

void doOnMediaFileCounterCallbackWrapper(File file){
  String _n = String(file.name());
  String _name = _n.substring(_n.lastIndexOf('/')+1, _n.lastIndexOf('.'));
  __INFO->arr.push_back(new String(_name));
//  __INFO->count++;
  if ( ! __FOUND_FLAG ) {
	if (__IDX > MediaFileCounter)
	  MediaFileCounter++;
	else {
	  __FOUND_FILE = file.name();
	  __FOUND_FLAG = true;
	}
  }
//  Serial.printf("\ndoOnMediaFileCounterCallbackWrapper: file: %s, __IDX: %d, MediaFileCounter: %d\n", file.name(), __IDX, MediaFileCounter);
}

int enumerateMediaFiles(const char *directoryName, const char* mediaext, mediaTypeInfo* info, bool displayFilenames) {
  Serial.printf("\nenumerateMediaFiles: directoryName: %s, mediaext: %s, displayFilenames: %s\n", directoryName, mediaext, displayFilenames ? "true" : "false" );
//  numberOfFiles = 0;
  __FOUND_FLAG = false;
  __IDX = 128;
  __INFO = info;
  uint8_t len = strlen(directoryName);
  __INFO->path = new char(len);
  strcpy ( __INFO->path, directoryName );
//    (char*)directoryName;
  len = strlen(mediaext);
  __INFO->ext = new char(len);
  strcpy ( __INFO->ext, mediaext );
//  = (char*)mediaext;
//  info->path = (char*)directoryName;
//  info->ext = (char*)mediaext;
//  String res = "";
  MediaFileCounter = 0;
  findFilesInDir(SPIFFS, directoryName, mediaext, 1, doOnMediaFileCounterCallbackWrapper);
  return MediaFileCounter;
/*
  File directory = SPIFFS.open(directoryName);
  File file;

  if (!directory) {
	return -1;
  }

  while (file = directory.openNextFile()) {
	if (isMediaFile(file.name(), mediaext)) {
	  numberOfFiles++;
	  if (displayFilenames) {
		Serial.print(numberOfFiles);
		Serial.print(":");
		Serial.print(file.name());
		Serial.print("    size:");
		Serial.println(file.size());
	  }
	} else if (displayFilenames) {
	  Serial.println(file.name());
	}

	file.close();
  }

  //    file.close();
  directory.close();

  return numberOfFiles;
  */
}

// Get the full path/filename of the GIF file with specified index
String getMediaFilenameByIndex(const char *directoryName, const char* mediaext,  int index){//}, char *pnBuffer) {
  Serial.printf("\ngetMediaFilenameByIndex: directoryName: %s, mediaext: %s, index: %d\n", directoryName, mediaext, index);
  __FOUND_FLAG = false;
  __IDX = index;
  String res = "";
  MediaFileCounter = 0;
  findFilesInDir(SPIFFS, directoryName, mediaext, 1, doOnMediaFileCounterCallbackWrapper);
  return __FOUND_FILE;

/*
  // Make sure index is in range
  if ((index < 0) || (index >= numberOfFiles))
	return res;

  File directory = SPIFFS.open(directoryName);
  if (!directory)
	return res;
  File file;
//  while ((index >= 0)) {
  do{
	file = directory.openNextFile();
	if (file) {
	  Serial.printf("\ngetMediaFilenameByIndex: file: %s\n", file.name());

	  if (isMediaFile(file.name(), mediaext)) {
		index--;

		// Copy the directory name into the pathname buffer
//		strcpy(pnBuffer, directoryName);

		//ESP32 SD Library includes the full path name in the filename, so no need to add the directory name
#if defined(ESP32)
//	  pnBuffer[0] = 0;
#else
		int len = strlen(pnBuffer);
			  if (len == 0 || pnBuffer[len - 1] != '/') strcat(pnBuffer, "/");
#endif

		// Append the filename to the pathname
//		strcat(pnBuffer, file.name());
		res = String(String(directoryName) + "/" + file.name() );
	  }

	  file.close();
	}
  } while ((index >= 0));

//  file.close();
  directory.close();
  return res;
  */
}
File openMediaFilenameByIndex(const char *directoryName, const char* mediaext, int index) {
  Serial.printf("\nopenMediaFilenameByIndex: directoryName: %s, mediaext: %s, index: %d\n", directoryName, mediaext, index);
//  char pathname[255];
  File file;
  String pathname = getMediaFilenameByIndex(directoryName, mediaext, index);

  Serial.print("Pathname: ");
  Serial.println(pathname);

  // Attempt to open the file for reading
  file = SPIFFS.open(pathname);
  if (!file) {
	Serial.printf("Error opening %s file\n", mediaext);
//	return -1;
  }
  return file;
}
// Return a random animated gif path/filename from the specified directory
String chooseRandomMediaFilename(const char *directoryName, const char* mediaext) {

  int index = random(numberOfFiles);
  return getMediaFilenameByIndex(directoryName, mediaext, index);
}

#endif //INC_201122_212358_ESP32DEV_GAMESHELPER_H
