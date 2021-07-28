void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
  ticker.attach(0.2, tick);
}

void wifiStuff () {
  WiFi.mode(WIFI_STA);
  ticker.attach(0.6, tick);
  WiFiManager wm;
  wm.setAPCallback(configModeCallback);
  if (!wm.autoConnect("HydroTech-ESP")) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }
  Serial.println("connected...yeey :)");
  ticker.detach();
  digitalWrite(LED, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  ID = WiFi.macAddress();
  Serial.println(ID);
}
