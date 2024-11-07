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

