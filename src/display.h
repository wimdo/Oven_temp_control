void setupDisplay()
{
  display.init();
  display.setRotation(3);
  display.fillScreen(TFT_BLACK);
  display.setFreeFont(TT1);  
  display.setTextSize(1);
  
}

/*

void showStatus() {
  display.setTextSize(2);
  display.setCursor(2, 107);
  display.print(sensor1.setPointValue);
  display.setCursor(80, 107);
  display.print(sensor1.tempGemeten);
  display.setCursor(160, 107);
  display.print(sensor2.tempGemeten);
}
*/
/*
void updateGraph(int counter){
  sensor1.tempHistory[counter]= sensor1.tempGemeten;
  int pixelPosY = 99-round(sensor1.tempHistory[counter]/4);
  display.drawPixel(counter,pixelPosY, TFT_BLUE);
  sensor2.tempHistory[counter]= sensor2.tempGemeten;
  pixelPosY = 99-round(sensor2.tempHistory[counter]/4);
  display.drawPixel( counter,pixelPosY, TFT_GREEN);

}
*/

void outlineMainscreen()
{
  mainscreen.createSprite(240,120);
  mainscreen.setColorDepth(8);
  mainscreen.setFreeFont(FF17);
  mainscreen.fillRect(0,0,240,120, TFT_BLACK);  
  mainscreen.fillRect(0,0,120,30, TFT_LIGHTGREY);
  mainscreen.setTextColor(TFT_DARKGREY);
  mainscreen.drawString("meetwaarde",2, 2);
  mainscreen.fillRect(120,0,120,30, TFT_LIGHTGREY);
  mainscreen.setTextColor(TFT_DARKGREY);
  //mainscreen.drawString("meetwaarde",2, 2);
  mainscreen.drawFloat(sensor1.tempGemeten, 1, 122,2);
  mainscreen.drawFloat(sensor2.tempGemeten, 1, 182,2);
  mainscreen.fillRect(0,30,120,30, TFT_SKYBLUE);
  mainscreen.setTextColor(TFT_DARKGREY);
  mainscreen.drawFloat(periode, 1, 2,32);
  mainscreen.fillRect(0,60,120,30, TFT_YELLOW);
  mainscreen.setTextColor(TFT_DARKGREY);
  mainscreen.drawFloat(dutycycle, 0, 2,62);
  mainscreen.fillRect(0,90,120,30, TFT_RED);
  mainscreen.setTextColor(TFT_DARKGREY);
  mainscreen.drawString("temperatuur",2, 92);
  mainscreen.pushSprite(0,0);
}



void buttonCheck(){
  /*
  if (button.buttonPressed){
    Serial.printf( "boop! button was down for %lu ms\n", button.millisPressed );
    int keuze =mainMenu();
    button.buttonPressed = false;
  }
  */
}

boolean rotaryScan(){
  long previousMillis = millis();
  button.buttonPressed=false;
  button.rotaryTurned=false;
  rotaryEncoder.enable();
  while (1)
  {
    if ((millis() - previousMillis) > 10000)
    {
      rotaryEncoder.disable();
      return false;
    }
    if (button.buttonPressed || button.rotaryTurned){
      //Serial.println(button.rotaryValue);
      rotaryEncoder.disable();
      return true;
    }
  }
}



float valueDropbox(int x,int y, int width,int depth, float startWaarde, float min, float max, float step){
    TFT_eSprite box = TFT_eSprite(&display);
    box.createSprite(width,depth);
    box.setColorDepth(8);
    box.setFreeFont(FF17);
    float range = max- min;
    float steps = range/step;
    float begin = 0; 
    float end = steps;
    //Serial.println("valueDropbox");
    //Serial.println(range);
    //Serial.println(steps);
    float start = (startWaarde-min)/step;
    //Serial.println(start);
    rotaryEncoder.setBoundaries( begin, end, false );
    rotaryEncoder.setEncoderValue( start );
    while (1)
    { 
        box.fillRect(0,0,width,depth, TFT_BLACK);
        box.drawRect(0,0,width,depth, TFT_WHITE);
        box.drawFloat(startWaarde, 1, 2,5);
        box.pushSprite(x,y);
        if (rotaryScan()){
          if (button.buttonPressed){
            button.buttonPressed=false;
            box.deleteSprite();
            //Serial.println(startWaarde);
            return startWaarde;
          }
          if (button.rotaryTurned){
            startWaarde = min + button.rotaryValue*step;
            //Serial.println(startWaarde);
            button.rotaryTurned = false;
          }
        } else {
          box.deleteSprite();
          return buttonNone;
        }
    }
    return 0;
}




int menuDropbox(char *menuTable[], int rijen)
{   
    TFT_eSprite box = TFT_eSprite(&display);
    box.createSprite(200,100);
    box.setColorDepth(8);
    box.setFreeFont(FF17);  
    button.rotaryValue = 0;
    rotaryEncoder.setBoundaries( 0, rijen-1, true );
    rotaryEncoder.setEncoderValue( button.rotaryValue );
    while (1)
    {    
      for (int i = 0; i <= rijen-1; i++)
        {
          if (button.rotaryValue == i){
            box.fillRect(0,i*20,200,20, TFT_LIGHTGREY);
            box.setTextColor(TFT_DARKGREY);
            box.drawString(menuTable[i],2, 2 + i * 20);
          } else {
            box.fillRect(0,i*20,200,20, TFT_DARKGREY);
            box.setTextColor(TFT_WHITE);
            box.drawString(menuTable[i],2, 2 + i * 20);
          }
        }
        box.pushSprite(20,0);
        if (rotaryScan()){
          if (button.buttonPressed){
            button.buttonPressed=false;
            box.deleteSprite();
            return button.rotaryValue;
          }
        } else {
          box.deleteSprite();
          return buttonNone;
        }
    }
    return 0;
}

void drawInputChoise(int keuze){
  mainscreen.fillRect(120,0,120,30, TFT_LIGHTGREY);
  mainscreen.setTextColor(TFT_DARKGREY);
  mainscreen.drawFloat(sensor1.tempGemeten, 1, 122,2);
  mainscreen.drawFloat(sensor2.tempGemeten, 1, 182,2);
  mainscreen.fillRect(120,30,120,90, TFT_BLACK);
  switch (keuze)
  {
  case 1:
    mainscreen.fillRect(120,30,120,30, TFT_SKYBLUE);
    mainscreen.setTextColor(TFT_DARKGREY);
    mainscreen.drawString("frequentie",122, 32);
    break;
  case 2:
    mainscreen.fillRect(120,60,120,30, TFT_YELLOW);
    mainscreen.setTextColor(TFT_DARKGREY);
    mainscreen.drawString("duty cycle",122, 62);
    break;
  case 3:
    mainscreen.fillRect(120,90,120,30, TFT_RED);
    mainscreen.setTextColor(TFT_DARKGREY);
    mainscreen.drawString("meetwaarde",122, 92);
    break;
  }
  mainscreen.pushSprite(0,0);
}

void testMenu() {
  outlineMainscreen();
  menuPositie =1;
  rotaryEncoder.setBoundaries( 1, 3, true );
  rotaryEncoder.setEncoderValue(menuPositie);
  rotaryEncoder.enable();
  float keuze;
  //unsigned long time;
  drawInputChoise(menuPositie);
  while (1){
    if ( overrun ){
      checkTemperature();
      drawInputChoise(menuPositie);
      overrun =false;
    }
    if (button.buttonPressed){
      if (button.longPress){
        button.buttonPressed = false;
        Serial.println("leave");
        return ;
      } else {
        //Serial.printf( "menupositie %lu \n", menuPositie );
        switch (menuPositie)
          {
          case 1:
            keuze = valueDropbox(120,30,120,30, periode, 2, 20, 1); 
            Serial.println(keuze);
            if (keuze != buttonNone){
              periode=keuze;
              resetHeatingTimer();
            }
            break;
          case 2:
            keuze = valueDropbox(120,60,120,30, dutycycle, 0, 100, 2); 
            if (keuze != buttonNone){
              dutycycle=keuze;
              resetHeatingTimer();
            }
            break;
          case 3:
            keuze = valueDropbox(120,90,120,30, 200, 150, 300, 1);  
            break;
          }
        outlineMainscreen();
        button.rotaryValue =menuPositie;
        rotaryEncoder.setBoundaries( 1, 3, true );
        rotaryEncoder.setEncoderValue(button.rotaryValue);
        rotaryEncoder.enable();
        drawInputChoise(button.rotaryValue);
      }

      button.buttonPressed=false;
    }
    if (button.rotaryTurned){
      //Serial.printf( "rotary was turned to %lu \n", button.rotaryValue );
      menuPositie = button.rotaryValue;
      //Serial.printf( "menu postie set to %lu \n", menuPositie );
      drawInputChoise(button.rotaryValue);
      button.rotaryTurned = false;
    } 
  }
}

int mainMenu()
{
  display.setTextSize(1);
  int rijen = sizeof(mainMenu_table) / sizeof(*mainMenu_table);
  rotaryEncoder.setBoundaries( 0, rijen-1, true );
  rotaryEncoder.begin();
  int keuze = menuDropbox(mainMenu_table, rijen );
  switch (keuze)
  {
  case buttonNone:
    return buttonNone;
    break;
  case 0:
    Serial.println("eerste regel");
    testMenu();
    return keuze;
    break;
  case 1:
    Serial.println("2de regel");
    return keuze;
    break;
  case 2:
    Serial.println("3de regel");
    return 1;
    break;
  case 3:
    Serial.println("4de regel");
    return 1;
    break;
  case 4:
    Serial.println("5de regel");
    return 1;
    break;
  }
  return 1;
}


