
void mainPage() {

  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>Temperaturecontrol server settings page</title>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"); // auto scale for mobile or website
  client.println("<meta http-equiv=\"refresh\" content=\""+String(mySystem.refreshRate)+"\">");
  client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: left;}");
  client.println("form  { display: table;      }");
  client.println(" p    { display: table-row;  }");
  client.println("label { display: table-cell; }");
  client.println("input { display: table-cell; }");
  client.println(".button { background-color: DarkGrey; border: 2px; margin: 2px;padding: 5px; width: 320px ;color: white; text-align: center;text-decoration: none; font-size: 20px; cursor: pointer;}");
  client.println("type=\"text/css\" #btn_submit{width:120px;} #formbox {width:320px; margin:auto 0; text-align: left;}#formbox2 {width:320px; margin:auto 0; text-align: center;}");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");

  client.println("<div id=\"formbox2\"><h1>Sensor 1</h1></div>");
  if (sensor1.present) {
    client.println("<div id=\"formbox2\"><h1 style=\"font-size:50px;\"><strong>" + String(sensor1.tempGemeten, 1) + " &#176C" + "</strong></h1></div> ");
    if (sensor1.setPoint) {
      client.println("<div id=\"formbox2\">");  
      switch (sensor1.power) {
        case heating:
          client.println("<h2>Status : Heating<br><br></h2>");
          break;
        case cooling:
          client.println("<h2>Status : Cooling<br><br></h2>");
          break;
        case idle:
          client.println("<h2>Status : Idle<br><br></h2>");
          break;
      } 
      client.println("</div> ");  
      client.println("<div id=\"formbox\">");
      client.println("<p><label for=\"foo\">Temperature control: </label> <span id=\"foo\">" + String(sensor1.setPointValue, 1) + " &#177 " + String(sensor1.tolerance, 1) + " &#176C" + "</span><br><br></p>");
      client.println("</div> ");
      if (button.sensor1SettingsSelect ){
        client.println("<div id=\"formbox2\"><h3>Remote settings</h1></div>");
      } else {
        client.println("<div id=\"formbox2\"><h3>Local settings</h1></div>");
      }
    } else {
        client.println("<div id=\"formbox2\">");
        client.println("<h2>No temperature control<br><br></h2>");
        client.println("</div> ");  
    }
  } else {
    client.println("<div id=\"formbox2\"><h1 STYLE=\"font-size: 200%;\">No sensor</h1><br><br></div>");
  }
  /*
  if (sensor1.setPoint){
    if (button.sensor1SettingsSelect ){
      client.println("<div id=\"formbox2\"><h3>Settings 2 used</h1></div>");
    } else {
      client.println("<div id=\"formbox2\"><h3>Settings 1 used</h1></div>");
    }
  }
*/
  client.println("<p><a href=\"settingsSensor1\"><button class=\"button\">Change Settings Sensor 1</button></a></p>");
  client.println("<div id=\"formbox\"><hr></div>");

  if (mySystem.sensorConnect==2){
  if (sensor2.present) {
    client.println("<div id=\"formbox2\"><h1 style=\"font-size:50px;\"><strong>" + String(sensor2.tempGemeten, 1) + " &#176C" + "</strong></h1></div> ");
    if (sensor2.setPoint) {
      client.println("<div id=\"formbox2\">");  
      switch (sensor2.power) {
        case heating:
          client.println("<h2>Status : Heating<br><br></h2>");
          break;
        case cooling:
          client.println("<h2>Status : Cooling<br><br></h2>");
          break;
        case idle:
          client.println("<h2>Status : Idle<br><br></h2>");
          break;
      } 
      client.println("</div> ");  
      client.println("<div id=\"formbox\">");
      client.println("<p><label for=\"foo\">Temperature control: </label> <span id=\"foo\">" + String(sensor2.setPointValue, 1) + " &#177 " + String(sensor2.tolerance, 1) + " &#176C" + "</span><br><br></p>");
      client.println("</div> ");
      /*
      client.println("<div id=\"formbox\">");
      if (sensor2.connectToThingSpeak) {
        client.println("<p><label for=\"foo\">Send to ThingSpeak: </label> <span id=\"foo\"> Yes</span><br><br></p>");
      } else {
        client.println("<p><label for=\"foo\">Send to ThingSpeak: </label> <span id=\"foo\"> No</span><br><br></p>");
      }
      if (sensor2.connectToMQTT) {
        client.println("<p><label for=\"foo\">Send to MQTT: </label> <span id=\"foo\"> Yes</span><br><br></p>");
      } else {
        client.println("<p><label for=\"foo\">Send to MQTT: </label> <span id=\"foo\"> No</span><br><br></p>");
      }
      client.println("</div> ");
      */
    } else {
        client.println("<div id=\"formbox2\">");
        client.println("<h2>No temperature control<br><br></h2>");
        client.println("</div> ");  
    }
  } else {
    client.println("<div id=\"formbox2\"><h1 STYLE=\"font-size: 200%;\">No sensor</h1><br><br></div>");
  }
  client.println("<p><a href=\"settingsSensor2\"><button class=\"button\">Change Settings Sensor 2</button></a></p>");
  client.println("<div id=\"formbox\"><hr></div>");

  }
  client.println("<div id=\"formbox\">");
  if (mySystem.connectToThingSpeak) {
    client.println("<p><label for=\"foo\">ThingSpeak : </label> <span id=\"foo\"> Yes</span><br><br></p>");
  } else {
    client.println("<p><label for=\"foo\">ThingSpeak : </label> <span id=\"foo\"> No</span><br><br></p>");
  }
  if (mySystem.connectToMQTT) {
    client.println("<p><label for=\"foo\">MQTT : </label> <span id=\"foo\">" +String(myServer.topic) + "</span><br><br></p>");
  } else {
    client.println("<p><label for=\"foo\">MQTT : </label> <span id=\"foo\"> No</span><br><br></p>");
  }
  client.println("</div> ");

  client.println("<div id=\"formbox\">");
  client.println("<p><label for=\"foo\">Version    : </label> <span id=\"foo\">" + String(VERSION) +  "</span><br><br></p>");
  client.println("</div> ");

  client.println("<div id=\"formbox\">");
  client.println("<p><label for=\"foo\">Hostname   : </label> <span id=\"foo\">" + String(fullhostname) +  "</span><br><br></p>");
  client.println("</div> ");


  client.println("<p><a href=\"settingsServer\"><button class=\"button\">CHANGE SERVER SETTINGS</button></a></p>");
  client.println("</body>");
  client.println("</html>");
  client.println();

}

void sensor1Page() {

  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>Temperature control settings page</title>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"); // auto scale for mobile or website
  client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: left;}");
  client.println("form  { display: table;      }");
  client.println(" p    { display: table-row;  }");
  client.println("label { display: table-cell; }");
  client.println("input { display: table-cell; }");
  client.println(".button { background-color: DarkGrey; border: 2px; margin: 2px;padding: 5px; width: 320px ;color: white; text-align: center;text-decoration: none; font-size: 20px; cursor: pointer;}");
  client.println("type=\"text/css\" #btn_submit{width:120px;} #formbox {width:320px; margin:auto 0; text-align: left;}#formbox2 {width:320px; margin:auto 0; text-align: center;}");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<form action=\"/settingsSensor1.php\" method=\"get\">");
  //client.println("<form action=\"/settingsSensor1.php\" method=\"post\" enctype=\"application/json\" >");
 
  client.println("<div id=\"formbox2\"><h1>Sensor 1</h1></div>");
  client.println("<div id=\"formbox2\"><h2>Settings local</h2>");
  client.println("<div id=\"formbox\">");
  /*
  client.println("<p><label for=\"S1_0\">Steering:</label>");
  client.println("<select name=\"S1_0\" >");
  if (sensor1_0.setPoint) {
    client.println("  <option value=\"1\" selected>On</option>");
    client.println("  <option value=\"0\" >Off</option>");
  } else {
    client.println("  <option value=\"1\" >On</option>");
    client.println("  <option value=\"0\" selected>Off</option>");
  }
  client.println("</select><br><br></p>");
  */
  //client.println("<p><label for=\"V1_0\">Target temperature: </label>");
  //client.println("<input type=\"number\" name=\"V1_0\" min=\"4\" max=\"28\" step=\"0.1\" value=" + String(sensor1_0.setPointValue, 1) + "><br><br></p>");

  client.println("<p><label for=\"H1_0\">Tolerance: </label>");
  client.println("<input type=\"number\" name=\"H1_0\" min=\"0\" max=\"2\" step=\"0.1\" value=" + String(sensor1_local.tolerance, 1) + "><br><br></p>");
  

  client.println("<div id=\"formbox2\"><h2>Settings remote</h2>");
  client.println("<div id=\"formbox\">");
  client.println("<p><label for=\"S1_1\">Steering:</label>");
  client.println("<select name=\"S1_1\" >");
  if (sensor1_remote.setPoint) {
    client.println("  <option value=\"1\" selected>On</option>");
    client.println("  <option value=\"0\" >Off</option>");
  } else {
    client.println("  <option value=\"1\" >On</option>");
    client.println("  <option value=\"0\" selected>Off</option>");
  }
  client.println("</select><br><br></p>");
  client.println("<p><label for=\"V1_1\">Target temperature: </label>");
  client.println("<input type=\"number\" name=\"V1_1\"min=\"4\" max=\"28\"  step=\"0.1\" value=" + String(sensor1_remote.setPointValue, 1) + "><br><br></p>");

  client.println("<p><label for=\"H1_1\">Tolerance: </label>");
  client.println("<input type=\"number\" name=\"H1_1\" min=\"0\" max=\"2\" step=\"0.1\" value=" + String(sensor1_remote.tolerance, 1) + "><br><br></p>");
  client.println("<div id=\"formbox2\"><input type=\"submit\" value=\"Change\"></div>");
  client.println("<hr>");
  client.println("</form>");
  client.println("<br>");
  client.println("<p><a href=\"main\"><button class=\"button\">BACK</button></a></p>");
  client.println("</body>");
  client.println("</html>");
  client.println();
}

void sensor2Page() {

  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>Temperature control settings page</title>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"); // auto scale for mobile or website
  client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: left;}");
  client.println("form  { display: table;      }");
  client.println(" p    { display: table-row;  }");
  client.println("label { display: table-cell; }");
  client.println("input { display: table-cell; }");
  client.println(".button { background-color: DarkGrey; border: 2px; margin: 2px;padding: 5px; width: 320px ;color: white; text-align: center;text-decoration: none; font-size: 20px; cursor: pointer;}");
  client.println("type=\"text/css\" #btn_submit{width:120px;} #formbox {width:320px; margin:auto 0; text-align: left;}#formbox2 {width:320px; margin:auto 0; text-align: center;}");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<form action=\"/settingsSensor2.php\" method=\"get\">"); 
  client.println("<div id=\"formbox2\"><h1>Sensor 2</h1></div>");
  client.println("<div id=\"formbox\">");
  client.println("<p><label for=\"S2\">Steering:</label>");
  client.println("<select  name=\"S2\" >");
  if (sensor2.setPoint) {
    client.println("  <option value=\"1\" selected>On</option>");
    client.println("  <option value=\"0\" >Off</option>");
  } else {
    client.println("  <option value=\"1\" >On</option>");
    client.println("  <option value=\"0\" selected>Off</option>");
  }
  client.println("</select><br><br></p>");
  client.println("<p><label for=\"V2\">Target temperature: </label>");
  client.println("<input type=\"number\"  name=\"V2\" step=\"0.1\" value=" + String(sensor2.setPointValue, 1) + "><br><br></p>");
  client.println("<p><label for=\"H2\">Tolerance: </label>");
  client.println("<input type=\"number\"  name=\"H2\" min=\"0\" max=\"1\" step=\"0.1\" value=" + String(sensor2.tolerance, 1) + "><br><br></p>");
  /*
  client.println("<p><label for=\"T2\">Send to ThinsgSpeak:</label>");
  client.println("<select name=\"T2\" >");
  if (sensor2.connectToThingSpeak) {
    client.println("  <option value=\"1\" selected>Yes</option>");
    client.println("  <option value=\"0\" >No</option>");
  } else {
    client.println("  <option value=\"1\" >Yes</option>");
    client.println("  <option value=\"0\" selected>No</option>");
  }
  client.println("</select><br><br></p> ");
  client.println("<p><label for=\"Q2\">Send to MQTT:</label>");
  client.println("<select name=\"Q2\" >");
  if (sensor2.connectToMQTT) {
    client.println("  <option value=\"1\" selected>Yes</option>");
    client.println("  <option value=\"0\" >No</option>");
  } else {
    client.println("  <option value=\"1\" >Yes</option>");
    client.println("  <option value=\"0\" selected>No</option>");
  }
  client.println("</select><br><br></p> ");
  client.println("</div>");
  */
  client.println("<div id=\"formbox2\"><input type=\"submit\" value=\"Change\"></div>");
  client.println("<hr>");
  client.println("</form>");
  client.println("<br>");
  client.println("<p><a href=\"main\"><button class=\"button\">BACK</button></a></p>");
  client.println("</body>");
  client.println("</html>");
  client.println();
}


void serverPage() {

  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>Temperaturecontrol server settings page</title>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"); // auto scale for mobile or website
  client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: left;}");
  client.println("form  { display: table;      }");
  client.println(" p    { display: table-row;  }");
  client.println("label { display: table-cell; }");
  client.println("input { display: table-cell; }");
  client.println(".button { background-color: DarkGrey; border: 2px; margin: 2px;padding: 5px; width: 320px ;color: white; text-align: center;text-decoration: none; font-size: 20px; cursor: pointer;}");
  client.println("type=\"text/css\" #btn_submit{width:120px;} #formbox {width:320px; margin:auto 0; text-align: left;}#formbox2 {width:320px; margin:auto 0; text-align: center;}");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<form action=\"/settingsServer.php\" method=\"get\">");
  client.println("<div id=\"formbox2\">");
  client.println("<h1>Server</h1>");
  client.println("</div>");
  client.println("<div id=\"formbox\">");
  client.println("<p><label for=\"R\">Refresh rate (s): </label>");
  client.println("<input type=\"number\"  name=\"R\" min=\"2\" max=\"30\"  value=" + String(mySystem.refreshRate) + "><br><br></p>");
  client.println("<p><label for=\"sensorConnect\">Sensor connected: </label>");
  client.println("<input type=\"number\"  name=\"sensorConnect\" min=\"1\" max=\"2\"  value=" + String(mySystem.sensorConnect) + "><br><br></p>");
  client.println("<p><label for=\"connectToWIFI\">Connect to WIFI:</label>");
  client.println("<select name=\"connectToWIFI\" >");
  if (mySystem.connectToWIFI) {
    client.println("  <option value=\"1\" selected>Yes</option>");
    client.println("  <option value=\"0\" >No</option>");
  } else {
    client.println("  <option value=\"1\" >Yes</option>");
    client.println("  <option value=\"0\" selected>No</option>");
  }
  client.println("</select><br><br></p>");
  client.println("</div>");
  client.println("<div id=\"formbox2\">");
  client.println("<input type=\"submit\" value=\"Change\">");
  client.println("</div>");
  client.println("<hr>");
  client.println("<div id=\"formbox2\">");
  client.println("<h1>ThingSpeak</h1>");
  client.println("</div>");
  client.println("<div id=\"formbox\">");
  
  client.println("<p><label for=\"connectToThingsSpeak\">Send to ThinsgSpeak:</label>");
  client.println("<select name=\"connectToThingsSpeak\" >");
  if (mySystem.connectToThingSpeak) {
    client.println("  <option value=\"1\" selected>Yes</option>");
    client.println("  <option value=\"0\" >No</option>");
  } else {
    client.println("  <option value=\"1\" >Yes</option>");
    client.println("  <option value=\"0\" selected>No</option>");
  }
  client.println("</select><br><br></p> ");

  client.println("<p><label for=\"CHANNEL\">Channel: </label>");
  client.println("<input type=\"number\" name=\"CHANNEL\" value=" + String(myServer.channelNumber) + "><br><br></p>");
  client.println("<p><label for=\"KEY\">KEY: </label>");
  client.println("<input type=\"text\" name=\"KEY\" value=" + String(myServer.APIKey) + "><br><br></p>");
  client.println("<p><label for=\"INTERVAL\">Interval (s): </label>");
  client.println("<input type=\"number\" name=\"INTERVAL\"min=\"15\" max=\"600\"  value=" + String(myServer.intervalThingSpeak) + " ><br><br></p>");
  client.println("</div>");
  client.println("<div id=\"formbox2\">");
  client.println("<input type=\"submit\" value=\"Change\"   >");
  client.println("</div>");
  client.println("<hr>");
  client.println("<div id=\"formbox2\">");
  client.println("<h1>MQTT</h1>");
  client.println("</div>");

  client.println("<div id=\"formbox\">");
  client.println("<p><label for=\"connectToMQTT\">Send to MQTT:</label>");
  client.println("<select name=\"connectToMQTT\" >");
  if (mySystem.connectToMQTT) {
    client.println("  <option value=\"1\" selected>Yes</option>");
    client.println("  <option value=\"0\" >No</option>");
  } else {
    client.println("  <option value=\"1\" >Yes</option>");
    client.println("  <option value=\"0\" selected>No</option>");
  }
  client.println("</select><br><br></p> ");

  client.println("<div id=\"formbox\">");
  client.println("<p><label for=\"serverMQTT\">Server: </label>");
  client.println("<input type=\"text\" name=\"serverMQTT\" value=" + String(myServer.serverMQTT) + "><br><br></p>");
  client.println("<p><label for=\"port\">Port: </label>");
  client.println("<input type=\"number\" name=\"port\" value=" + String(myServer.port) + "><br><br></p>");
  client.println("<p><label for=\"username\">Username: </label>");
  client.println("<input type=\"text\" name=\"username\" value=" + String(myServer.username) + "><br><br></p>");
  client.println("<p><label for=\"passwordMQTT\">Password: </label>");
  client.println("<input type=\"text\" name=\"passwordMQTT\" value=" + String(myServer.passwordMQTT) + "><br><br></p>");
  client.println("<p><label for=\"topic\">Topic: </label>");
  client.println("<input type=\"text\" name=\"topic\" value=" + String(myServer.topic) + "><br><br></p>");
  client.println("<p><label for=\"intervalMQTT\">Interval (s): </label>");
  client.println("<input type=\"number\" name=\"intervalMQTT\" value=" + String(myServer.intervalMQTT) + "><br><br></p>");
  client.println("</div>");
  client.println("<div id=\"formbox2\">");
  client.println("<input type=\"submit\" value=\"Change\"   >");
  client.println("</div>");
  client.println("<hr>");

  client.println("</form>");
  client.println("<br>");
  client.println("<p><a href=\"main\"><button class=\"button\">BACK</button></a></p>");
  client.println("</body>");
  client.println("</html>");
  client.println();

}

void handleRequest(){
  //Serial.println("New Client."); // print a message out in the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    { // loop while the client's connected
      currentTime = millis();
      if (client.available())
      {
        currentLine = client.readStringUntil('\r');
        char c = client.read();
        //Serial.println (currentLine);
        if (c == '\n')
        {
          if (currentLine.length() == 0)
          {
            Serial.print("HTTP: ");
            Serial.println(HTTPrequest);
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            if (HTTPrequest.indexOf("GET /settingsServer.php") >= 0)
            {
              checkHTTPrequest(HTTPrequest);
              writeMyServerFile();
              serverPage();
            }
            else if (HTTPrequest.indexOf("GET /settingsSensor1.php?") >= 0)
            {
              checkHTTPrequest(HTTPrequest);
              writeSensorSettingsFile();
              if (button.sensor1SettingsSelect){
                sensor1 = sensor1_remote;
              } else {
                sensor1 = sensor1_local;
              }
              checkTemperature(); // alles juist zetten in functie van een wijziging
              mainPage();
            }
            else if (HTTPrequest.indexOf("GET /settingsSensor2.php?") >= 0)
            {
              checkHTTPrequest(HTTPrequest);
              writeSensorSettingsFile();
              checkTemperature(); // alles juist zetten in functie van een wijziging
              mainPage();
            }
            else if (HTTPrequest.indexOf("GET /settingsServer") >= 0)
            {
              serverPage();
            }
            else if (HTTPrequest.indexOf("GET /settingsSensor1") >= 0)
            {
              sensor1Page();
            }
            else if (HTTPrequest.indexOf("GET /settingsSensor2") >= 0)
            {
              sensor2Page();
            }
            else if (HTTPrequest.indexOf("GET /main") >= 0)
            {
              mainPage();
            }
            else if (HTTPrequest.indexOf("GET /favicon.ico") >= 0)
            {
              mainPage();
            }
            else if (HTTPrequest.indexOf("GET /") >= 0)
            {
              mainPage();
            }
            HTTPrequest = "";
            client.stop();
            Serial.println("HTTP: Client disconnected.");
            Serial.println("");
          }
          else
          {
            if (HTTPrequest == "")
            {
              HTTPrequest = currentLine;
            }
            currentLine = "";
          }
        }
      }
    }


}
