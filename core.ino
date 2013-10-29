void initeinstellen()
{ 
  cursorPosition=0; 
  lcd.clear();
  funktion = &einstellen;
  anzeige = &anzeigeeinstellen;

  lcd.clear();
  buttonpressed= &toggleitem;
  buttonhold= &inittrapping;
}




void einstellen()
{
  MsTimer2::stop();
  if (cursorPosition<4)
  {
    werte[cursorPosition] = potiWert; 
    int_tm=werte[0]/10; //intervall zwischen den einzelnen auslösungen NACH PIR trigger in SEKUNDEN
    cycles=werte[1]/80; //wie viele durchläufe
    belichtungszeit=werte[2]*5/100*100; //wie lange soll die kamera dann getriggert werden?
    minimumpause=(( long)cycles*(( long)int_tm*1000+( long)belichtungszeit))/1000; //was ist die minimale zeit, die zwischen zwei interrupts auftritt?
    pause=(map(werte[3],0,1023,minimumpause,600)); //welche pause muss zwischen zwei interrupts vom sensor liegen
  }
  else
  {
    initZeitEinstellen();
  }
}

void initZeitEinstellen()
{ 
  cursorPosition=5; 
  lcd.clear();
  funktion = &zeitEinstellen;
  anzeige = &anzeigeZeitEinstellen;

  lcd.clear();
  buttonpressed= &toggleitem;
  buttonhold= &inittrapping;
}

void zeitEinstellen()
{
  if (cursorPosition==5)
  {
    startzeit=map(potiWert,0,1023,0,1440);
    startzeit=constrain(startzeit,0,1440);
    start_h=startzeit/60;
    start_m=startzeit-start_h*60;

  }
  else if (cursorPosition==6)
  {
    endzeit=map(potiWert,0,1023,0,1440);
    endzeit=constrain(endzeit,0,1440);
    end_h=endzeit/60;
    end_m=endzeit-end_h*60;
  }
  else
  {
    initeinstellen(); 
  }




}


void inittrapping(){

  buttonhold= &stoptrapping;  
  buttonpressed=&test;
  lcd.clear();
  anzeige=&anzeigetrapping;  

  ausloesungen=cycles;
  pausenTime=(long)(pause);
  funktion= &trapping;
  aftersleep= &sensorinterrupt;

  attachInterrupt(1,aftersleep,HIGH);

}

void trapping()
{ 

  DateTime now = RTC.now();

  nowTime=now.get();
  now_m=now.minute();
  now_h=now.hour();
  nowminutes=now_h*60+now_m;


  controlDisplayWithPoti();
  loop_cnt2++;  
  if (loop_cnt2 > 200 && !cycling)
    // if (!cycling)
  { 
    lcd.noBacklight();
    delay(100);
    loop_cnt2 = 0;
    sleepNow();     // sleep function called here    
  }
}

void sensorinterrupt()
{ 

  interruptTime=nowTime;
  //    Serial.println(interruptTime);
  //    Serial.println(lastInterruptTime+pausenTime);



  if (startzeit<endzeit)
  {
    //    Serial.println(startzeit);
    //    Serial.println(nowminutes);
    //    Serial.println(endzeit); 
    if (startzeit<nowminutes && nowminutes<endzeit)
    {
      feuer=true;
      // Serial.println("Feuer normal!");
    }
    else{
      feuer=false; 
      // Serial.println("not in time!");
    }
  }
  else if (startzeit>endzeit)
  { 
    //    Serial.println(startzeit);
    //    Serial.println(nowminutes);
    //    Serial.println(endzeit); 
    if (startzeit < nowminutes || nowminutes<endzeit)
    {
      feuer=true;
      //Serial.println("Feuer special!");
    }
    else
    {
      feuer=false;
      //Serial.println("not in time!");
    }

  }

  else if (startzeit==endzeit)
  {
    feuer=true; 
  }





  if (interruptTime > lastInterruptTime+pausenTime &&feuer)
  { 
    //Serial.println("Neues Intervall");
    lastInterruptTime = interruptTime;
    start_ausloesung();   
    pausing= false;  
  }
  else
  {
    //Serial.println("Es ist Pause!");
    pausing=true; 
    loop_cnt2=500;
    //Gehe sofort wieder schlafen!

  }
}

void start_ausloesung()
{   
  cycling=true;
  detachInterrupt(1);
  //  Serial.println("Foto!");
  if (!exposing){
    MsTimer2::set(belichtungszeit,stop_ausloesung);
    MsTimer2::start();   
    exposing=true;

    ausloesungen--;
    digitalWrite(SHUTTER,LOW);
    digitalWrite(FOCUS,LOW);
  }


}




void stop_ausloesung()
{ 
  stop_camera_trigger();
  MsTimer2::stop();

  exposing=false;

  //STARTE INTERVALL
  MsTimer2::set(int_tm*1000,stop_intervall);
  MsTimer2::start(); 

  imintervall=true; 
}



void stop_intervall(){

  MsTimer2::stop();
  imintervall=false; 
  if (ausloesungen>0)
  {
    MsTimer2::stop();
    start_ausloesung();
  }
  else
  { 
    //    Serial.println("Cyling beendet");
    MsTimer2::stop();
    ausloesungen=cycles; 
    cycling=false;
    digitalWrite(PIR,LOW);
    attachInterrupt(1,aftersleep,HIGH);
  }
}





void cameraWakeUp()
{
  digitalWrite(FOCUS,LOW);
  delay(200);
  digitalWrite(FOCUS,HIGH);
}

void stop_camera_trigger()
{
  digitalWrite(SHUTTER,HIGH);
  digitalWrite(FOCUS,HIGH);
}































