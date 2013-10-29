void anzeigeeinstellen()
{ backlightcontoller();

  lcd.setCursor(1,0);
  lcd.print("I:");
  printLeadingZeros(int_tm);
  lcd.print("s");

  lcd.setCursor(9,0);
  lcd.print("F:");
  printLeadingZeros(cycles);

  lcd.setCursor(1,1);
  lcd.print("B:");
  printLeadingZeros(belichtungszeit);
  lcd.print("m");

  lcd.setCursor(9,1);
  lcd.print("P:");
  printLeadingZeros(pause);
  lcd.print("s");

  if (cursorPosition==0)
  {
    lcd.setCursor(0,0);
    lcd.print(">"); 
  }
  else if (cursorPosition==1)
  {
    lcd.setCursor(8,0);
    lcd.print(">"); 
  }
  else if (cursorPosition==2)
  {
    lcd.setCursor(0,1);
    lcd.print(">"); 
  }
  else if (cursorPosition==3)
  {
    lcd.setCursor(8,1);
    lcd.print(">"); 
  }

}

void anzeigeZeitEinstellen()
{ backlightcontoller();

  lcd.setCursor(1,0);
  lcd.print("Start ");
  printDigits(start_h);
  lcd.print(":");
  printDigits(start_m);
  lcd.print("h");

  lcd.setCursor(1,1);
  lcd.print("Ende  ");
  printDigits(end_h);
  lcd.print(":");
  printDigits(end_m);
  lcd.print("h");



  if (cursorPosition==5)
  {
    lcd.setCursor(0,0);
    lcd.print(">"); 
  }

  else if (cursorPosition==6)
  {
    lcd.setCursor(0,1);
    lcd.print(">"); 
  }

}

void anzeigecharging ()
{
  lcd.noBacklight();
  lcd.setCursor(0,0);
  lcd.print("Charging...");
  lcd.setCursor(5,1);
  printPercentage(percentage);
  lcd.print("/");
  lcd.print(voltage);
  lcd.print("V");
}

void anzeigetrapping()
{
  
  lcd.setCursor(0,0);
  lcd.print("TRAP");

  lcd.setCursor(5,0);
  lcd.print("F:");
  printDigits(ausloesungen);

  lcd.setCursor(13,0);
  if (cycling)
  {
    lcd.print("on "); 
  }
  else {
    lcd.print("off"); 
  }

  lcd.setCursor(0,1);
  printDigits(start_h);
  lcd.print(":");
  printDigits(start_m);
  lcd.print("-");
  printDigits(end_h);
  lcd.print(":");
  printDigits(end_m);
  lcd.print("h ");
  if (!ch) {
    lcd.print(percentage);
    lcd.print("%");
  }
  else
  {
    lcd.print("Lad");  
  }
  //  if (pausing)
  //  {
  //    lcd.print("Pause"); 
  //  }
  //  else {
  //    if (cycling)
  //    {
  //      lcd.print("On "); 
  //   }
  //    else {
  //     lcd.print("Off"); 
  //    }
  //  }

}

void printDigits(int number){
  if (number <10) 
  {
    lcd.print("0");
    lcd.print(number);
  } 
  else 
  {
    lcd.print(number); 
  }

}

void printPercentage(int number)
{
  if (number<10)
  {
    lcd.print("  ");
    lcd.print(number);
    lcd.print("%"); 
  }
  else if (number<100)
  {
    lcd.print(" ");
    lcd.print(number);
    lcd.print("%"); 
  }
  else
  {
    lcd.print(number);
    lcd.print("%"); 
  }
}

void printLeadingZeros(int number)
{ 
  if (number<10)
  {
    lcd.print("   ");
    lcd.print(number);
  }
  else if (number<100)
  {
    lcd.print("  ");
    lcd.print(number);
  }
  else if (number<1000)
  {
    lcd.print(" ");
    lcd.print(number);
  }
  else
  {
    lcd.print("");
    lcd.print(number);
  }


}

//void displayonoff()
//{
//  if (displayon) {
//    lcd.noDisplay();
//    lcd.noBacklight(); 
//    displayon=false;
//  }
//  else {
//    lcd.display();
//    lcd.backlight(); 
//    displayon=true;
//  }
//}




void controlDisplayWithPoti (){

  int brightness = map(analogRead(POTI),0,1023,254,0);


  if (brightness > 10)
  {
    lcd.display();
    lcd.setBacklight(brightness); 
  }
  else {
    lcd.noDisplay();
    lcd.noBacklight();

  }

}

void backlightcontoller()
{
   if (lcd_off>200)
  {
   lcd.noBacklight();
   lcd.noDisplay(); 
   lcd.clear(); 
  }
  else {
   
   lcd.backlight(); 
   lcd.display();
  } 
}



