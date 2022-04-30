// String serverPath = "http://192.168.100.17:1337";
String serverPath = "https://hydrotech-backend.herokuapp.com";

void getService()
{
  //  WiFiClient client;
  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient http2;
  if (http2.begin(*client, serverPath + "/servicesfordevice?servedBy=" + USER_ID + "&status=running&status=posted"))
  {
    Serial.print("[HTTP] getService GET...\n");
    int httpCode = http2.GET();
    if (httpCode > 0)
    {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String payload = http2.getString();
        JSONVar myObject = JSON.parse(payload);
        // JSON.typeof(jsonVar) can be used to get the type of the var
        if (JSON.typeof(myObject[0]) == "undefined")
        {
          Serial.println("Parsing input failed!");
          return;
        }
        if (JSON.typeof(myObject[0]) == "null")
        {
          clearService();
        }
        else
        {
          setService(myObject[0]);
        }
      }
    }
    else
    {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http2.errorToString(httpCode).c_str());
    }
    http2.end();
  }
  else
  {
    Serial.printf("[HTTP} Unable to connect\n");
  }
}

void getDevice()
{
  //  WiFiClient client;
  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient http;
  if (http.begin(*client, serverPath + "/users/" + USER_ID))
  {
    Serial.print("[HTTP] GET Device...\n");
    int httpCode = http.GET();
    if (httpCode > 0)
    {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String payload = http.getString();
        JSONVar myObject = JSON.parse(payload);
        // JSON.typeof(jsonVar) can be used to get the type of the var
        if (JSON.typeof(myObject) == "undefined")
        {
          return;
        }
        if (JSON.typeof(myObject) == "null")
        {
          clearDevice();
          Serial.println("User Not Found!!!");
        }
        else
        {
          setDevice(myObject);
        }
      }
    }
    else
    {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  else
  {
    Serial.printf("[HTTP} Unable to connect\n");
  }
}

void updateConnection(bool sendId, long seconds)
{
  //  WiFiClient client1;
  std::unique_ptr<BearSSL::WiFiClientSecure>
  client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient http1;

  Serial.println("[HTTP] begin...\n");
  // configure traged server and url
  http1.begin(*client, serverPath + "/users/" + USER_ID); //HTTP
  http1.addHeader("Content-Type", "application/json");

  Serial.print("[HTTP] PUT Connection...\n");
  // start connection and send HTTP header and body
  String connection = "false";
  if (isDeviceConnected) {
    isDeviceConnected = false;
    connection = "false";
  }
  else {
    isDeviceConnected = true;
    connection = "true";
  }
  int httpCode;
  if (seconds <= 0)
  {
    if (sendId)
      httpCode = http1.PUT("{\"isDeviceConnected\":" + connection + ",\"deviceId\":\"" + ID + "\"}");
    else
      httpCode = http1.PUT("{\"isDeviceConnected\":" + connection + "}");
  }
  else
  {
    String millis_available = (const char *)device["availablemillis"];
    long availablemillis = millis_available.toInt() - seconds;
    if (sendId)httpCode = http1.PUT("{\"isDeviceConnected\":" + connection + ",\"deviceId\":\"" + ID + "\",\"availablemillis\":" + (String)availablemillis + "}");
    else
      httpCode = http1.PUT("{\"isDeviceConnected\":" + connection + ",\"availablemillis\":" + (String)availablemillis + "}");
  }

  // httpCode will be negative on error
  if (httpCode > 0)
  {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] PUT... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http1.getString();
      JSONVar myObject = JSON.parse(payload);
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined")
      {
        return;
      }
      if (JSON.typeof(myObject) == "null")
      {
        Serial.println("User Not Found!!!");
      }
      else
      {
        device = myObject;
      }
    }
  }
  else
  {
    Serial.printf("[HTTP] PUT... failed, error: %s\n", http1.errorToString(httpCode).c_str());
  }

  http1.end();
}

void updateService(String body)
{
  //  WiFiClient client1;
  std::unique_ptr<BearSSL::WiFiClientSecure>
  client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient http3;
  http3.begin(*client, serverPath + "/servicesfordevice/" + (int)service["id"]); //HTTP
  http3.addHeader("Content-Type", "application/json");

  // start connection and send HTTP header and body
  int httpCode;
  httpCode = http3.PUT(body);

  // httpCode will be negative on error
  if (httpCode > 0)
  {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] .Update Service.. code: %d\n", httpCode);
    // file found at server
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http3.getString();
      JSONVar myObject = JSON.parse(payload);
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined")
      {
        return;
      }
      if (JSON.typeof(myObject) == "null")
      {
        Serial.println("Service Not Found!!!");
        clearService();
      }
      else
      {
        Serial.println(myObject);
        setService(myObject);
      }
    }
  }
  else
  {
    Serial.printf("[HTTP] PUT... failed, error: %s\n", http3.errorToString(httpCode).c_str());
  }

  http3.end();
}
