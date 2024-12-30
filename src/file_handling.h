void writeSensorSettingsFile(){

    JsonObject obj = doc.to<JsonObject>();
    //doc["sensor1_0_setPoint"] = sensor1_0.setPoint;
    //doc["sensor1_0_setPointValue"] = sensor1_0.setPointValue;
    doc["sensor1_0_tolerance"] = sensor1_local.tolerance;
    doc["sensor1_1_setPoint"] = sensor1_remote.setPoint;
    doc["sensor1_1_setPointValue"] = sensor1_remote.setPointValue;
    doc["sensor1_1_tolerance"] = sensor1_remote.tolerance;
    //doc["sensor2_setPoint"] = sensor2.setPoint;
    //doc["sensor2_setPointValue"] = sensor2.setPointValue;
    //doc["sensor2_tolerance"] = sensor2.tolerance;
    serializeJson(doc, myPayload);
     File file = SPIFFS.open("/SensorSettings.json", "w");
    file.print(myPayload);
    file.close();
}


void reWriteSensorSettingsFile()
{
    //sensor1_0.setPoint=0;
    //sensor1_0.setPointValue=15;
    sensor1_local.tolerance=1;
    sensor1_remote.setPoint=0;
    sensor1_remote.setPointValue=15;
    sensor1_remote.tolerance=1;
    //sensor2.setPoint=0;
    //sensor2.setPointValue=15;
    //sensor2.tolerance=1;
    writeSensorSettingsFile();
}

void readSensorSettingsFile()
{
    int lenght = 0;
    String fileName = "/SensorSettings.json";
    if (SPIFFS.exists(fileName))
    {
      File file = SPIFFS.open(fileName, "r");
      if (file) {
        //size_t size = file.size();
        while(file.available()){
            myPayload[lenght] = file.read();
            lenght++;
        }
        myPayload[lenght] ='\0';
        Serial.print("SensorSettings.json : ");
        Serial.println (myPayload); //use for debug
      }
    }
    DeserializationError error = deserializeJson(doc, myPayload,lenght);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
    } else {
        //sensor1_0.setPoint=doc["sensor1_0_setPoint"];
        //sensor1_0.setPointValue=doc["sensor1_0_setPointValue"];
        sensor1_local.tolerance=doc["sensor1_0_tolerance"];
        sensor1_remote.setPoint=doc["sensor1_1_setPoint"];
        sensor1_remote.setPointValue=doc["sensor1_1_setPointValue"];
        sensor1_remote.tolerance=doc["sensor1_1_tolerance"];
        //sensor2.setPoint=doc["sensor2_setPoint"];
        //sensor2.setPointValue=doc["sensor2_setPointValue"];
        //sensor2.tolerance=doc["sensor2_tolerance"];
    }

}

void writeMyServerFile(){
    Serial.println("saving config");
    JsonObject obj = doc.to<JsonObject>();
    doc["mqtt_server"] = myServer.serverMQTT;
    doc["mqtt_port"] = myServer.port;
    doc["mqtt_user"] = myServer.username;
    doc["mqtt_pass"] = myServer.passwordMQTT;
    doc["mqtt_topic"] = myServer.topic;
    doc["mqtt_interval"] = myServer.intervalMQTT;
    doc["thingspeak_channel"] = myServer.channelNumber;
    doc["thingspeak_APIkey"] = myServer.APIKey;
    doc["thingspeak_interval"] = myServer.intervalThingSpeak;    
    serializeJson(doc, myPayload);
    Serial.println(myPayload);
    String fileName = "/myServer.json";
    Serial.print("write ");
    Serial.println(fileName);
    File file = SPIFFS.open(fileName, "w");
    file.print(myPayload);
    file.close();
    doc.clear();
    doc["sensorConnect"] = mySystem.sensorConnect;
    doc["refreshRate"] = mySystem.refreshRate;
    doc["connectToWIFI"] = mySystem.connectToWIFI;
    doc["connectToMQTT"] = mySystem.connectToMQTT;
    doc["connectToThingSpeak"] = mySystem.connectToThingSpeak;
    serializeJson(doc, myPayload);
    file = SPIFFS.open("/mySystem.json", "w");
    file.print(myPayload);
    file.close();
}


void reWriteMyServerFile()
{  
    mySystem.connectToWIFI= true;
    mySystem.connectToMQTT= true;
    mySystem.refreshRate= 10;  
    strcpy(myServer.serverMQTT,"192.168.0.120");
    myServer.port=1883;
    strcpy(myServer.topic,"tempcontrole/temp_oven");
    myServer.intervalMQTT=30;
    writeMyServerFile();
    Serial.println("SYSTEM : rewrite server file");
}

void readMyServerFile()
{
    int lenght = 0;
    String fileName = "/myServer.json";
    if (SPIFFS.exists(fileName))
    {
       File file = SPIFFS.open(fileName, "r");
      if (file) {
        while(file.available()){
            myPayload[lenght] = file.read();
            lenght++;
        }
        myPayload[lenght] ='\0';
        Serial.print("myServer.json : ");
        Serial.println (myPayload); //use for debug
      }
    }
    DeserializationError error = deserializeJson(doc, myPayload,lenght);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
    } else {
        strcpy(myServer.serverMQTT,doc["mqtt_server"]| "default");
        myServer.port = doc["mqtt_port"];
        strcpy(myServer.username,doc["mqtt_user"]| "default");
        strcpy(myServer.passwordMQTT,doc["mqtt_pass"] | "default");
        strcpy(myServer.topic,doc["mqtt_topic"]| "default");
        myServer.intervalMQTT = doc["mqtt_interval"];
        myServer.channelNumber =doc["thingspeak_channel"]  ;
        strcpy(myServer.APIKey,doc["thingspeak_APIkey"] |"default");
        myServer.intervalThingSpeak =doc["thingspeak_interval"]  ;    
    }
    lenght = 0;
    fileName = "/mySystem.json";
    if (SPIFFS.exists(fileName))
    {
      File file = SPIFFS.open(fileName, "r");
      if (file) {
        while(file.available()){
            myPayload[lenght] = file.read();
            lenght++;
        }
        myPayload[lenght] ='\0';
        Serial.print("mySystem.json : ");
        Serial.println (myPayload); //use for debug
      }
    }
    error = deserializeJson(doc, myPayload,lenght);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
    } else {
        mySystem.sensorConnect = doc["sensorConnect"] ;
        mySystem.refreshRate= doc["refreshRate"]  ;
        mySystem.connectToWIFI = doc["connectToWIFI"]  ;
        mySystem.connectToMQTT = doc["connectToMQTT"] ;
        mySystem.connectToThingSpeak= doc["connectToThingSpeak"]  ;
    }
}

void listFilesInDir(File dir, int numTabs)
{
    while (true)
    {

        File entry = dir.openNextFile();
        if (!entry)
        {
            // no more files in the folder
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++)
        {
            Serial.print('\t');
        }
        Serial.print(entry.name());
        if (entry.isDirectory())
        {
            Serial.println("/");
            listFilesInDir(entry, numTabs + 1);
        }
        else
        {
            // display zise for file, nothing for directory
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
}



void checkFileSystem()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    Serial.println(F("Inizializing FS..."));
    if (SPIFFS.begin())
    {
        Serial.println(F("SPIFFS mounted correctly."));
    }
    else
    {
        Serial.println(F("!An error occurred during SPIFFS mounting"));
    }

    // Get all information of SPIFFS

    unsigned int totalBytes = SPIFFS.totalBytes();
    unsigned int usedBytes = SPIFFS.usedBytes();

    Serial.println("===== File system info =====");

    Serial.print("Total space:      ");
    Serial.print(totalBytes);
    Serial.println("byte");

    Serial.print("Total space used: ");
    Serial.print(usedBytes);
    Serial.println("byte");

    Serial.println();

    // Open dir folder
    File dir = SPIFFS.open("/");
    // List file at root
    listFilesInDir(dir, 1);
}


/*
void checkFileSystem2()
{
    String fileName;
    Serial.println(F("SYSTEM : Inizializing FS..."));
    if (SPIFFS.begin()){
        Serial.println(F("done."));
    }else{
        Serial.println(F("fail."));
    }
    FSInfo fs_info;
    SPIFFS.info(fs_info);
    Serial.printf("SYSTEM : Total space: %u byte\n",fs_info.totalBytes);
    Serial.printf("SYSTEM : Total space used: %u byte\n",fs_info.usedBytes);
    Serial.printf("SYSTEM : Block size: %u byte\n",fs_info.blockSize);
    Serial.printf("SYSTEM : Page size: %u byte\n",fs_info.totalBytes);
    Serial.printf("SYSTEM : Max open files: %u byte\n",fs_info.maxOpenFiles);
    Serial.printf("SYSTEM : Max path lenght: %u byte\n",fs_info.maxPathLength);
    // Open dir folder
    Dir dir = SPIFFS.openDir("/");
    // Cycle all the content
    while (dir.next()) {
        // get filename
        Serial.print(dir.fileName());
        Serial.print(" - ");
        // If element have a size display It else write 0
        if(dir.fileSize()) {
            File f = dir.openFile("r");
            Serial.println(f.size());
            f.close();
        }else{
            Serial.println("0");
        }
    }
}
*/
void loadDataSensor1(){ 
    /*
    if (digitalRead(switchSensor1Settings)){
      sensor1 =sensor1_remote;
      button.sensor1SettingsSelect = remote;
    } else {
      sensor1 = sensor1_local;
      button.sensor1SettingsSelect = local;     
    }*/
    }


void loadDataFromFile()
{
    Serial.println("SYSTEM : Read setting from File");
    checkFileSystem();
    reWriteMyServerFile();
    readMyServerFile();
    reWriteSensorSettingsFile();
    readSensorSettingsFile();
    loadDataSensor1();

}