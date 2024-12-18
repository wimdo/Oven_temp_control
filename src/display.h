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

void buttonCheck(){
  /*
  if (button.buttonPressed){
    Serial.printf( "boop! button was down for %lu ms\n", button.millisPressed );
    int keuze =mainMenu();
    button.buttonPressed = false;
  }
  */
}

int rotaryScan(){
  rotaryEncoder.enable();
  long previousMillis = millis();
  button.buttonPressed=false;
  button.rotaryTurned=false;
  rotaryEncoder.enable();
  while (1)
  {
    if ((millis() - previousMillis) > 10000)
    {
      rotaryEncoder.disable();
      return btnTIMEOUT;
    }
    if (button.buttonPressed || button.rotaryTurned){
      rotaryEncoder.disable();
      return 1;
    }
  }
}



int menuDropbox(char *menuTable[], int rijen)
{
    
    
    box.setColorDepth(8);
    box.createSprite(150,100);
    //box.fillRect(0,0,150,100, GREEN);
    int positie = 0;
    //box.fillRect(0, 12, 128, 80, BLACK); //(x, y, breedte = 6x karakter +6+10, hoogte aantal lijnen
    //box.drawRect(0, 12, 128, 80, WHITE);
    while (1)
    {
      box.fillRect(0,0,150,100, WHITE);
      box.fillRect(0, 12, 128, 80, BLACK); //(x, y, breedte = 6x karakter +6+10, hoogte aantal lijnen
      box.drawRect(0, 12, 128, 80, WHITE);
      box.setTextColor(WHITE);//DARKGREY
      
      for (int i = 0; i <= rijen-1; i++)
        {
            box.setCursor(2, 16 + i * 8);
            if (positie == i)
            {
                box.printf("> %s", menuTable[i]);
            }
            else
            {
                box.printf("  %s", menuTable[i]);
            }
        }
        box.pushSprite(0,0);
        //Serial.printf("%d %d %d %d\n", rijen, beginregel, eindregel, positie);
        int keuze = 1;
        keuze = rotaryScan();
        if (keuze != btnTIMEOUT) {
          if (button.buttonPressed){
            button.buttonPressed=false;
            Serial.println(button.rotaryValue);
            box.deleteSprite();
            return button.rotaryValue;

          }
          if (button.rotaryTurned){
            positie = button.rotaryValue;
            button.rotaryTurned = false;
          }
        } else {
          box.deleteSprite();
          return buttonNone;
        }
    }
    return 0;
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