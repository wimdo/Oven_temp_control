void checkTemperature() {

  currentTime = millis();
  if ((currentTime - previousTimeTempRead) > (2000)){
    previousTimeTempRead= currentTime;
    
    sensor1.tempGemeten=(int)round(thermocouple1.readCelsius());
    sensor2.tempGemeten=(int)round(thermocouple2.readCelsius());
    Serial.print(sensor1.tempGemeten);
    Serial.print("  ");
    Serial.print(sensor2.tempGemeten);
    Serial.print("   ");
    Serial.println(sensor1.setPointValue);
    showStatus();
    sensor1.present = true;
    sensor2.present = true;
    /*
    if (sensor1.tempGemeten != -127) {
      sensor1.present = true;
      if (sensor1.setPoint == true) {
        if ( sensor1.tempGemeten < (sensor1.setPointValue - sensor1.tolerance)) {
          sensor1.power = heating;
        }
        if (( sensor1.tempGemeten > sensor1.setPointValue - sensor1.tolerance) && (sensor1.tempGemeten <= sensor1.setPointValue))  {
          if (sensor1.power == cooling) {
            sensor1.power = idle;
          }
        }
        if (( sensor1.tempGemeten > sensor1.setPointValue) && (sensor1.tempGemeten <= sensor1.setPointValue + sensor1.tolerance)) {
          if (sensor1.power == heating) {
            sensor1.power = idle;
          }
        }
        if ( sensor1.tempGemeten > (sensor1.setPointValue + sensor1.tolerance))   {
            sensor1.power = cooling;
        }
      }
    } else {
      sensor1.present = false;
      sensor1.power = idle;
    }
    */
/*
    switch (sensor1.power) {
      case heating:
        digitalWrite(sensor1HeatPin, relayOn);
        //digitalWrite(sensor1CoolPin, relayOff);
        break;
      case cooling:
        digitalWrite(sensor1HeatPin, relayOff);
        //digitalWrite(sensor1CoolPin, relayOn);
        break;
      case idle:
        digitalWrite(sensor1HeatPin, relayOff);
        //digitalWrite(sensor1CoolPin, relayOff);
        break;
    }
 */

  }

}

void setupTemperature(){

}
