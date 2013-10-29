
void readInputs()
{ 
  
  for (int i = 0;i<20;i++)
  {
  potiWert+=map(analogRead(POTI),0,1023,1023,0);
  }
  potiWert/=20;
  
  if (potiWert-lastValue>5 || potiWert-lastValue<-5)
  {
   lcd_off=0;
   
//   Serial.println(lastValue);
//   Serial.println(potiWert);
//  Serial.println("---");
  }
  
  lastValue=potiWert;
  tastenverarbeiten();

}

void tastenverarbeiten()
{
  //  if(buttonpressedtime+300 < millis())
  //  {
  //    if(digitalRead(BUTTON)==LOW)
  //    {
  //      buttonpressed();
  //      lcd_off=0;
  //      lcd.display();
  //      lcd.backlight();
  //      lcd.clear();
  //      buttonpressedtime = millis();
  //    }
  //
  //
  //  }

  current = digitalRead(BUTTON);
  if (current == HIGH && previous == LOW && millis()- firstTime > 200){
    firstTime = millis();    // if the buttons becomes press remember the time 
  }

  if (current == HIGH && ((millis() - firstTime) % 1000) < 30 && millis() - firstTime > 800){
    //Serial.print("Hold"); Hold FUNKTION
    buttonhold();
     lcd_off=0;
    count++;


  }
  if (current == LOW && previous == HIGH && count >=2 && count < 4){
    //Serial.print("Released"); // When the button is released if the counter is between the
  }                          // two numbers (3-6 blinks or secs) run the program

  if (current == LOW && previous == HIGH && count >=0 && count < 1){
    buttonpressed(); //This is where you would put you differnt functions
    lcd_off=0;
  }                                // with diffent time parameters

  if (current == LOW){ // reset the counter if the button is not pressed
    count = 0;
  }

  previous = current;

}


void toggleitem()
{
  clearArrows();
  if (cursorPosition<7)
  {
    cursorPosition++;  
  }
  else {
    cursorPosition=0; 
  }


}

void clearArrows(){
  lcd.setCursor(0,0);
  lcd.print(" ");
  lcd.setCursor(8,0);
  lcd.print(" ");
  lcd.setCursor(0,1);
  lcd.print(" ");
  lcd.setCursor(8,1);
  lcd.print(" ");

}

void stoptrapping()
{
  MsTimer2::stop();
  lcd.clear();
  stop_camera_trigger();
  initeinstellen();
  cycling=false;
  exposing=false;
}





