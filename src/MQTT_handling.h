
void mqttSendIPAdress()
{
    IPAddress myIP = WiFi.localIP();
    sprintf (myPayload,"%u.%u.%u.%u", myIP[0],myIP[1],myIP[2],myIP[3]);
    JsonObject obj = doc.to<JsonObject>();
    doc["IP"]= myPayload;
    doc["Module"]= moduletype;
    doc["Version"]= VERSION;
    doc["Hostname"]= fullhostname;
    doc["Time"]= millis();
    serializeJson(doc, myPayload);
    //sprintf (myPayload,"{\"IP\":\"%u.%u.%u.%u\",\"Hostname\":\"%s\"}", myIP[0],myIP[1],myIP[2],myIP[3],fullhostname);
    //sprintf (myPayload,"{\"IP\":\"%u.%u.%u.%u\",\"Module\":\"%s\",\"Hostname\":\"%s\",\"time\":\"%u\"}", myIP[0],myIP[1],myIP[2],myIP[3],moduletype,fullhostname,millis());
    sprintf (myTopic,"%s/%s",infoPrefix, &myServer.topic[0]);
    clientEsp.publish(myTopic, myPayload,1);
    Serial.printf("MQTT : %s = %s\n", myTopic, myPayload ); 
}
/*
void mqttSendIPAdress()
{
  JsonObject obj = doc.to<JsonObject>();
  doc["IP"]= myIPString;
  doc["Module"]= moduletype;
  doc["Version"]= VERSION;
  doc["Hostname"]= fullhostname;
  doc["Time"]= millis();
  serializeJson(doc, myPayload);
  clientEsp.publish(myTopicInfo, myPayload,1);
  Serial.printf("MQTT : %s = %s\n", myTopicInfo, myPayload ); 
}
*/





void callback(char* topic, byte* payload, unsigned int length) {
    char command[10] = "";
    for (unsigned int i = 0; i < length; i++) {
      myPayload[i]=payload[i]; 
    }
    myPayload[length]= 0;
    Serial.printf("MQTT : received [%s] %s\n", topic, myPayload);
    deserializeJson(doc, myPayload, length);
    strcpy(command, doc["command"] | "default");
    if ( strcmp(command,"IP") == 0 ){
      mqttSendIPAdress(); 
    }

}

bool connectMQTT()
{
  byte errorCountConnection = 0;
  while (!clientEsp.connected())
  {
    if (clientEsp.connect(fullhostname))
    { 
      mqttSendIPAdress();
      sprintf (myTopic,"%s/%s",commandPrefix, &myServer.topic[0]);
      if (clientEsp.subscribe(myTopic)) { 
        Serial.printf("MQTT : Subscribed to %s\n",myTopic);
      } else {
        Serial.printf("MQTT : Fail to subscribed to %s\n",myTopic);
      }
      return true;
    }
    else
    {
      errorCountConnection++;
      delay(500);
    }
    if (errorCountConnection == 5)
    {
      Serial.println(F("MQTT : Can not connect with MQTT server"));
      mySystem.connectToMQTT= false;
      return false;
      break;
    }
  }
  return true;
}

void publishWithMQTT()
{ 
  currentTime = millis();
  //if ((currentTime - previousTimeMQTT) > (myServer.intervalMQTT * 1000))
  if ((currentTime - previousTimeMQTT) > (15 * 1000))
  {
    unsigned long seconds = currentTime / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    unsigned long days = hours / 24;
    //currentTime %= 1000;
    seconds %= 60;
    minutes %= 60;
    hours %= 24;
    char timeOn[64];
    sprintf (timeOn,"%d:%d:%d",hours,minutes,seconds);
    previousTimeMQTT = currentTime;
    String statusString = "";
    myPayloadString ="";
    JsonObject obj = doc.to<JsonObject>();
    if (WiFi.status() == WL_CONNECTED)
    {
      if (mySystem.connectToMQTT){
        if (connectMQTT()) {  
            doc["Time"]=timeOn;
            if (sensor1.present){
              //doc["Sensor1"]["Id"]= sensor1.deviceAddress;
              doc["Sensor1"]["Temperature"]= round(sensor1.tempGemeten*10)/10;
              if (sensor1.setPoint)
              {
                switch (sensor1.power)
                {
                case heating:
                  statusString = "heat";
                  break;
                case cooling:
                  statusString = "cool";
                  break;
                case idle:
                  statusString = "idle";
                  break;
                }
                doc["Sensor1"]["Status"]= statusString;
                doc["Sensor1"]["Tolerance"]= sensor1.tolerance;
                doc["Sensor1"]["Target"]= sensor1.setPointValue;
              }
           }  
          if (sensor2.present){
            //doc["Sensor2"]["Id"]= sensor2.deviceAddress;
            doc["Sensor2"]["Temperature"]= round(sensor2.tempGemeten*10)/10;
            if (sensor2.setPoint)
            {
              switch (sensor2.power)
              {
              case heating:
                statusString = "heat";
                break;
              case cooling:
                statusString = "cool";
                break;
              case idle:
                statusString = "idle";
                break;
              }
              doc["Sensor2"]["Status"]= statusString;
              doc["Sensor2"]["Tolerance"]= sensor2.tolerance;
              doc["Sensor2"]["Target"]= sensor2.setPointValue;
            }
          } 
          if (!doc.isNull() ){
            serializeJson(doc, myPayload);
            sprintf (myTopic,"%s/%s",dataPrefix, &myServer.topic[0]);
            clientEsp.publish(myTopic, myPayload,1);
            Serial.printf("MQTT : %s = %s\n", myTopic, myPayload ); 
          }
        }
      }    
    }
  }
}




/*
00:00:00.001 HDW: ESP8266EX
00:00:00.050 CFG: Loaded from flash at FB, Count 1241
00:00:00.055 QPC: Count 1
00:00:00.063 Project tasmota Tasmota Version 9.5.0(tasmota)-2_7_4_9(2021-06-17T08:26:35)
00:00:00.518 WIF: Connecting to AP1 57_home Channel 7 BSSId A0:40:A0:89:8F:50 in mode 11n as badkamer-0584...
00:00:03.752 WIF: Connected
00:00:04.004 HTP: Web server active on badkamer-0584 with IP address 192.168.0.138
18:09:20.042 MQT: Attempting connection...
18:09:20.053 MQT: Connected
18:09:20.056 MQT: tele/badkamer/LWT = Online (retained)
18:09:20.059 MQT: cmnd/badkamer/POWER = 
18:09:20.069 MQT: tele/badkamer/INFO1 = {"Info1":{"Module":"Generic","Version":"9.5.0(tasmota)","FallbackTopic":"cmnd/DVES_4AA248_fb/","GroupTopic":"cmnd/tasmotas/"}}
18:09:20.077 MQT: tele/badkamer/INFO2 = {"Info2":{"WebServerMode":"Admin","Hostname":"badkamer-0584","IPAddress":"192.168.0.138"}}
18:09:20.089 MQT: tele/badkamer/INFO3 = {"Info3":{"RestartReason":"Software/System restart"}}
18:09:20.096 MQT: stat/badkamer/RESULT = {"POWER":"OFF"}
18:09:20.099 MQT: stat/badkamer/POWER = OFF
18:09:21.437 QPC: Reset
18:09:24.416 MQT: tele/badkamer/STATE = {"Time":"2021-11-28T18:09:24","Uptime":"0T00:00:11","UptimeSec":11,"Heap":29,"SleepMode":"Dynamic","Sleep":50,"LoadAvg":19,"MqttCount":1,"POWER":"OFF","Wifi":{"AP":1,"SSId":"57_home","BSSId":"A0:40:A0:89:8F:50","Channel":7,"Mode":"11n","RSSI":54,"Signal":-73,"LinkCount":1,"Downtime":"0T00:00:05"}}
18:09:24.444 MQT: tele/badkamer/SENSOR = {"Time":"2021-11-28T18:09:24","DS18S20":{"Id":"000802E7DE83","Temperature":4.8},"AM2301":{"Temperature":25.9,"Humidity":80.7,"DewPoint":22.3},"TempUnit":"C"}
18:09:24.569 MQT: stat/badkamer/RESULT = {"POWER":"OFF"}
18:09:24.572 MQT: stat/badkamer/POWER = OFF
*/

