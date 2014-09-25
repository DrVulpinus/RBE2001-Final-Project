#include <LiquidCrystal.h>




LiquidCrystal lcd(12, 11, 6, 5, 4, 3); //Initialize the LCD library on the correct pins

String firstLine = "";
String secondLine = "";

void setupLCD(){
  lcd.begin(16,2);
}

void lcdPrintStrLn(String output){
  firstLine = secondLine;
  secondLine = output;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(firstLine);
  lcd.setCursor(0,1);
  lcd.print(secondLine);
}
