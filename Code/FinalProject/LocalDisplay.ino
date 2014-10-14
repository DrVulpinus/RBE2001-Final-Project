#include <LiquidCrystal.h>




LiquidCrystal lcd(37, 35, 29, 27, 25, 21); //Initialize the LCD library on the correct pins

String firstLine = "";
String secondLine = "";

void setupLCD(){
  lcd.begin(16,2);
}

void lcdPrintLn(String output){
  firstLine = secondLine;
  secondLine = output;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(firstLine);
  lcd.setCursor(0,1);
  lcd.print(secondLine);
}
void lcdPrintLn(int output){
  firstLine = secondLine;
  secondLine = String(output);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(firstLine);
  lcd.setCursor(0,1);
  lcd.print(secondLine);
}
