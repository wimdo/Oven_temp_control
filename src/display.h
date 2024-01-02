void setupDisplay()
{
  display.init(135, 240); // Init ST7789 240x240
  display.setRotation(1);
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH); // Backlight on
  display.setTextSize(1);
}


void showStatus() {
  display.setTextSize(2);
  display.setCursor(2, 107);
  display.print(sensor1.setPointValue);
  display.setCursor(80, 107);
  display.print(sensor1.tempGemeten);
  display.setCursor(160, 107);
  display.print(sensor2.tempGemeten);
}

void updateGraph(int counter){
  sensor1.tempHistory[counter]= sensor1.tempGemeten;
  int pixelPosY = 99-round(sensor1.tempHistory[counter]/4);
  display.drawPixel(counter,pixelPosY, BLUE);
  sensor2.tempHistory[counter]= sensor2.tempGemeten;
  pixelPosY = 99-round(sensor2.tempHistory[counter]/4);
  display.drawPixel( counter,pixelPosY, GREEN);

}

void outlineMainscreen()
{
  display.fillScreen(ST77XX_BLACK);
  display.setTextColor(WHITE, ST77XX_BLACK);
  display.drawRect(0, 0, 240, 105, GREY);
  display.drawFastHLine(0, 24, 240, GREY);
  display.drawFastHLine(0, 24+25, 240, GREY);
  display.drawFastHLine(0, 24+25+25, 240, GREY);
  display.drawRect(0, 99, 240, 29, GREY);
  display.drawFastHLine(0, 99-round(sensor1.setPointValue/4), 240, ST77XX_RED);
}


int buttonpressCheck()
{ 
  unsigned long millisHold = 0;
  if (button.buttonPressed == btnLEFT)
  {
      while ((millis() - button.millisPressed) < 40)
      {
      }
      if (digitalRead(rotarySwitch) == 0){
         while (digitalRead(rotarySwitch) == 0)
         {
            millisHold = millis()-button.millisPressed;
            yield();
         }    
        if (millisHold < 1500){
            button.buttonPressed = btnNONE;
            return btnLEFT;
        } else 
          {
            button.buttonPressed = btnNONE;
            return btnLEFTLONG;
          } 
      } else {
        button.buttonPressed = btnNONE;
        return btnNONE;
      }
  } else if (button.buttonPressed == btnRIGHT)
  {
      while ((millis() - button.millisPressed) < 40)
      {
      }
      if (digitalRead(35) == 0){
         while (digitalRead(35) == 0)
         {
            millisHold = millis()-button.millisPressed;
            yield();
         }    
        if (millisHold < 1500){
            button.buttonPressed = btnNONE;
            return btnRIGHT;
        } else 
          {
            button.buttonPressed = btnNONE;
            return btnRIGHTLONG;
          } 
      } else {
        button.buttonPressed = btnNONE;
        return btnNONE;
      }
  }
  return 0;
}

void buttonCheck(){
  int value = buttonpressCheck();
  if (value != btnNONE){
    if (value == btnLEFT){
       if (sensor1.setPointValue > 0){
        sensor1.setPointValue = sensor1.setPointValue -5;
       }
    } else if (value == btnRIGHT){
       if (sensor1.setPointValue <400){
        sensor1.setPointValue = sensor1.setPointValue +5;
       }
    }
    outlineMainscreen();
    for (int i = 0; i <= sensor1.tempHistoryCounter; i++) {
      updateGraph(i);
    }
    showStatus();
  }

}

void rotaryCheck(){
  int value = buttonpressCheck();
  if (value != btnNONE){
    while(1){
      // Read the current state of inputCLK
      rotaryCurrentStateCLK = digitalRead(rotaryS1); 
      unsigned long lastButtonPress = 0;
       //If the previous and the current state of the inputCLK are different then a pulse has occured
      if (rotaryCurrentStateCLK != rotaryPreviousStateCLK){ 
         while (millis() - lastButtonPress <20) {}//
        // If the inputDT state is different than the inputCLK state then 
        // the encoder is rotating counterclockwise
        if (digitalRead(rotaryS2) != rotaryCurrentStateCLK) { 
          rotaryCounter --;
          encdir ="CCW";         
        } else {
          // Encoder is rotating clockwise
          rotaryCounter ++;
          encdir ="CW";         
        }
        Serial.print("Direction: ");
        Serial.print(encdir);
        Serial.print(" -- Value: ");
        Serial.println(rotaryCounter);
      } 
      // Update previousStateCLK with the current state
      rotaryPreviousStateCLK = rotaryCurrentStateCLK; 

    }



  }

}