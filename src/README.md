# dreamer-led-fw



void loop()
{
  if ( WiFi.status() ==  WL_CONNECTED )
  {
    // WiFi is UP,  do what ever
  } else
  {
    // wifi down, reconnect here
    WiFi.begin(  );
    int WLcount = 0;
    while (WiFi.status() != WL_CONNECTED && WLcount < 200 )
    {
      delay( 100 );
      Serial.printf(".");
      ++WLcount;
    }
  }
} // END loop()

/*
void connectToWiFi(){
  if ( WiFi.status() != WL_CONNECTED )  {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    WiFi.begin( "Guest" );
    vTaskDelay( 2000 );
    while ( WiFi.status() != WL_CONNECTED )	{
      vTaskDelay( 2000 );
      log_i(" waiting on wifi connection" );
    }
  }
}
void wifiKeepAlive( ){//void *pvParameters ){
  if ( WiFi.status() != WL_CONNECTED ) {
//  for (;;) {
    Serial.println("Connecting Wifi...");
    if (wifiMulti.run() == WL_CONNECTED) {
      Serial.println("");
      Serial.println("WiFi connected");
      printWLANStatus();
//	  Serial.println("IP address: ");
//	  Serial.println(WiFi.localIP());
    } else {
      log_i("MQTT keep alive found MQTTclient disconnected");
      connectToWiFi();
//	  connectToMQTT();
    }
    vTaskDelay(500);
//  }
//  vTaskDelete ( NULL );
  }
}
*/

void doOnWiFiConnect(){
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.print("Connected to ");
    printWLANStatus();

    /*use mdns for host name resolution*/
    if (!MDNS.begin(host)) { //http://esp32.local
      Serial.println("Error setting up MDNS responder!");
    }
    Serial.printf("\nmDNS responder started: http://%s.local\n", host);
    sprintf(__BOOT_STR__, "%d.%d.%d.%d;%s", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3],
	    ssid_sta);
  }
}

void loop111() {
//  wifiKeepAlive();
//
//  if(wifiMulti.run() != WL_CONNECTED) {
//	Serial.println("WiFi not connected!");
//	vTaskDelay(1000);
//  }

///*
//  if ((WiFi.status() != WL_CONNECTED) && (millis() > check_wifi)) {
//	Serial.println("Reconnecting to WiFi...");
////	WiFi.disconnect();
////	WiFi.begin(SSID, PASS);
//	connectToWLAN();
//	setupWifiConnection();
//	setupServer(&server);
//	check_wifi = millis() + 30000;
//  }
//  if (WiFi.status() == WL_CONNECTED) {
////  drawFrame();
////  gifDrawer.drawFrame();
////  connectToWLAN();
////  loop_Async_WiFiManager();
//  }
//  */

  clockDrawer.drawFrame();
//  textDrawer.drawFrame();
//  jpgDrawer.drawFrame();

}
