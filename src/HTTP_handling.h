
void configModeCallback (WiFiManager *myWiFiManager) {
  //DisplayShowConnectAP();
}

void setupHostName(){
  uint16_t maxlen = strlen(nameprefix) + 7;
  uint8_t mac[6];
  WiFi.macAddress(mac);
  snprintf(fullhostname, maxlen, "%s-%02X%02X%02X", nameprefix, mac[3], mac[4], mac[5]);

}


void setupWifiManager(){
  char port[8];
  char interval[8];
  if (mySystem.connectToWIFI) {
    //DisplayShowConnectWifi();  
    Serial.println("setup wifimanager");
    sprintf (port, "%i", myServer.port);
    sprintf (interval, "%i", myServer.intervalMQTT);
    WiFiManagerParameter custom_mqtt_server("server", "mqtt server", myServer.serverMQTT, 32);
    WiFiManagerParameter custom_mqtt_port("port", "mqtt port", port, 6);
    WiFiManagerParameter custom_mqtt_user("user", "mqtt user", myServer.username, 32);
    WiFiManagerParameter custom_mqtt_pass("pass", "mqtt pass", myServer.passwordMQTT, 32);
    WiFiManagerParameter custom_mqtt_topic("topic", "mqtt topic", myServer.topic, 32);
    WiFiManagerParameter custom_mqtt_interval("port", "mqtt interval", interval, 6);
    WiFiManager wifiManager;
    wifiManager.addParameter(&custom_mqtt_server);
    wifiManager.addParameter(&custom_mqtt_port);
    wifiManager.addParameter(&custom_mqtt_user);
    wifiManager.addParameter(&custom_mqtt_pass);
    wifiManager.addParameter(&custom_mqtt_topic);
    wifiManager.addParameter(&custom_mqtt_interval);
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setConfigPortalTimeout(60);  
    boolean result = wifiManager.autoConnect(fullhostname);
    if (result)
      {
        Serial.println("WIFI : Connected");
        boolean checkToSave = false;
        if( strcmp(myServer.serverMQTT,custom_mqtt_server.getValue()) != 0 ){
          strcpy(myServer.serverMQTT, custom_mqtt_server.getValue());
          checkToSave = true;
        }
        if( strcmp(myServer.username,custom_mqtt_user.getValue()) != 0 ){
          strcpy(myServer.username, custom_mqtt_user.getValue());
          checkToSave = true;
        }
        if( strcmp(myServer.passwordMQTT,custom_mqtt_pass.getValue()) != 0 ){
          strcpy(myServer.passwordMQTT, custom_mqtt_pass.getValue());
          checkToSave = true;
        }
        if( strcmp(myServer.topic,custom_mqtt_topic.getValue()) != 0 ){
          strcpy(myServer.topic, custom_mqtt_topic.getValue());
          checkToSave = true;
        } 
        if( strcmp(port,custom_mqtt_port.getValue()) != 0 ){
          myServer.port = atoi(custom_mqtt_port.getValue());
          checkToSave = true;
        } 
        if( strcmp(interval,custom_mqtt_interval.getValue()) != 0 ){
          myServer.intervalMQTT = atoi(custom_mqtt_interval.getValue());
          checkToSave = true;
        } 
        //checkToSave = true;
        if (checkToSave){
          //writeMyServerFile();
        }
        Serial.println("MQTT : start");
        clientEsp.setServer(&myServer.serverMQTT[0], myServer.port);
        clientEsp.setCallback(callback);
        if (mySystem.connectToMQTT){
          
          if (connectMQTT()){
            mqttSendIPAdress();
          } 
          
        }  
      }
      else{
        Serial.println("WIFI : not connected");
        mySystem.connectToWIFI = false;
      }   
  } else {
      WiFi.mode(WIFI_AP);
      WiFi.softAP(fullhostname);
  }
  setupOTA();
  Serial.println("WIFI : Start webserver");
  server.begin();
}

void checkHTTPrequest(String inputString)
{
  unsigned int beginPositie = inputString.indexOf("?", 0) + 1;
  unsigned int eindPositie = inputString.indexOf(" ", beginPositie);
  inputString = inputString.substring(beginPositie, eindPositie) + "&";
  beginPositie = 0;
  eindPositie = 0;
  int middenPositie = 0;
  Serial.print("Check request ");
  Serial.println(inputString);
  while (beginPositie < inputString.length())
  {
    eindPositie = inputString.indexOf("&", beginPositie);
    middenPositie = inputString.indexOf("=", beginPositie);
    String fieldString = inputString.substring(beginPositie, middenPositie);
    String dataString = inputString.substring(middenPositie + 1, eindPositie);
    beginPositie = eindPositie + 1;
    
    if (fieldString == "S1_0")
    {
      sensor1_local.setPoint = (boolean)dataString.toInt();
    }
    else if (fieldString == "V1_0")
    {
      sensor1_local.setPointValue = dataString.toFloat();
    }
    else if (fieldString == "H1_0")
    {
      sensor1_local.tolerance = dataString.toFloat();
    }

    //else if (fieldString == "T1_0")
    //{
    //  sensor1_0.connectToThingSpeak = (boolean)dataString.toInt();
    //}
    //else if (fieldString == "Q1_0")
    //{
    //  sensor1_0.connectToMQTT = (boolean)dataString.toInt();
    //}

    else if (fieldString == "S1_1")
    {
      sensor1_remote.setPoint = (boolean)dataString.toInt();
    }
    else if (fieldString == "V1_1")
    {
      sensor1_remote.setPointValue = dataString.toFloat();
    }
    else if (fieldString == "H1_1")
    {
      sensor1_remote.tolerance = dataString.toFloat();
    }
    //else if (fieldString == "T1_1")
    //{
    // sensor1_1.connectToThingSpeak = (boolean)dataString.toInt();
    //}
    //else if (fieldString == "Q1_1")
    //{
    //  sensor1_1.connectToMQTT = (boolean)dataString.toInt();
    //}





    else if (fieldString == "S2")
    {
      sensor2.setPoint = (boolean)dataString.toInt();
    }
    else if (fieldString == "V2")
    {
      sensor2.setPointValue = dataString.toFloat();
    }
    else if (fieldString == "H2")
    {
      sensor2.tolerance = dataString.toFloat();
    }
    //else if (fieldString == "T2")
    //{
    //  sensor2.connectToThingSpeak = (boolean)dataString.toInt();
    //}
    //else if (fieldString == "Q2")
    //{
    //  sensor2.connectToMQTT = (boolean)dataString.toInt();
    //}
    else if (fieldString == "R")
    {
      mySystem.refreshRate = dataString.toInt();
    }
    else if (fieldString == "sensorConnect")
    {
      mySystem.sensorConnect = dataString.toInt();
    }
    //else if (fieldString == "AP-SSID")
    //{
    //  dataString.toCharArray(myServer.ssidAP, dataString.length() + 1);
    //}
    else if (fieldString == "connectToWIFI")
    {
      mySystem.connectToWIFI = (boolean)dataString.toInt();
    }
    else if (fieldString == "connectToThingsSpeak")
    {
      mySystem.connectToThingSpeak = (boolean)dataString.toInt();
    }
    else if (fieldString == "CHANNEL")
    {
      myServer.channelNumber = (long)dataString.toInt();
    }
    else if (fieldString == "KEY")
    {
      dataString.toCharArray(myServer.APIKey, dataString.length() + 1);
    }
    else if (fieldString == "INTERVAL")
    {
      myServer.intervalThingSpeak = dataString.toInt();
    }
    else if (fieldString == "connectToMQTT")
    {
      mySystem.connectToMQTT = (boolean)dataString.toInt();
    }
    else if (fieldString == "serverMQTT")
    {
      dataString.toCharArray(myServer.serverMQTT, dataString.length() + 1);
    }
    else if (fieldString == "port")
    {
      myServer.port = dataString.toInt();
    }
    else if (fieldString == "username")
    {
      dataString.toCharArray(myServer.username, dataString.length() + 1);
    }
    else if (fieldString == "passwordMQTT")
    {
      dataString.toCharArray(myServer.passwordMQTT, dataString.length() + 1);
    }
    else if (fieldString == "topic")
    {
      while (dataString.indexOf("%2F") >= 0)
      {
        dataString.replace("%2F", "/");
      }
      Serial.println(dataString);
      dataString.toCharArray(myServer.topic, dataString.length() + 1);
    }
    else if (fieldString == "intervalMQTT")
    {
      myServer.intervalMQTT = dataString.toInt();
    }
  }
}
