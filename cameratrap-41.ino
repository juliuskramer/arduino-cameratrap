#include <SimpleTimer.h>

#include <Battery.h>

#include <Wire.h> 
#include <DS3231.h>
#include <LowPower.h>

#include <LiquidCrystal_I2C.h>
#include <MsTimer2.h>
#include <avr/sleep.h>

#include <Time.h>

//für das seeeduino stalker board, inklusive einer TIMING Funktion für Statusupdates von Batteriezustand.


LiquidCrystal_I2C lcd(0x27,16,2); //I2C ADRESSE



#define POTI A0
#define BUTTON 5
#define PIR 3

#define SHUTTER 11
#define FOCUS 12

int current; //Current state of the button (LOW is pressed b/c i'm using the pullup resistors
int count;   // How long the button was held (secs)
byte previous = LOW;
unsigned long firstTime;   // how long since the button was first pressed 




int loop_cnt,lcd_off,loop_cnt2;

long minimumpause; //gehe nach 10s schlafen

boolean cycling=false;

boolean displayon;

int potiWert,cursorPosition, lastValue;
long buttonpressedtime;

int werte[4];



//Intervall

volatile int int_tm,belichtungszeit;

int pause;

long interruptTime,nowTime;
long pausenTime;
long lastInterruptTime;

volatile int ausloesungen;
int cycles;

volatile boolean exposing,imintervall,pausing;
boolean feuer = false;

//Zeitschaltuhr

int startzeit;
int start_h,start_m;
int end_h,end_m;
int endzeit,endzeit2;

int nowminutes,now_m,now_h;



void (*funktion) (void); //welche funktion ist gerade Aktiv? -> Menü, Manuell, Intervall? welche funktion erfüllen die Inputs?
void (*anzeige) (void); //was soll gerade auf dem display angezeigt werden?
void (*buttonpressed) (void); //was soll gerade auf dem display angezeigt werden?
void (*buttonhold) (void); //was soll gerade auf dem display angezeigt werden?
void (*aftersleep) (void); //welche funktion wird direkt nach dem interrupt aufgerufen??

DS3231 RTC;

SimpleTimer timer;
int batterytimer;

Battery battery;
int percentage;
float voltage;
char* CS;
bool ch;




void setup()
{ 
  //analogReference(INTERNAL); 
  Wire.begin();
  RTC.begin();
  lcd.init();
  lcd.backlight();


  Serial.begin(9600);

  pinMode(SHUTTER,OUTPUT);
  pinMode(FOCUS,OUTPUT);

  pinMode(PIR,INPUT);
  pinMode(BUTTON,INPUT_PULLUP);
  digitalWrite(SHUTTER,HIGH);
  digitalWrite(FOCUS,HIGH);
  digitalWrite(PIR,LOW);
  //Startscreen: Zeige Batteriezustand!
  battery.update();
  percentage = battery.getPercentage();
  voltage = battery.getVoltage();
  ch = battery.isCharging();  
  
  if (!ch)
  {
    lcd.print("Batterie:   ");
    printPercentage(percentage);

    lcd.setCursor(11,1);
    lcd.print(voltage);
    lcd.print("V");
    
    delay(5000);
  }
  else
  {
    lcd.print("Batterie laedt..."); 
    lcd.setCursor(5,1);
    printPercentage(percentage);
    lcd.print("/");
    lcd.print(voltage);
    lcd.print("V");
    delay(8000);
  }
  lcd.clear();

  lcd.home();
  lcd.print("Starte Sensor");
  lcd.setCursor(0,1);
  for (int i =0;i<16;i++)
  {
    lcd.print(".");
    delay(300);
  }

  lcd.clear();

  batterytimer = timer.setInterval(5000, batterycontrol);
  initeinstellen();

}

void loop()
{

  timer.run();
  readInputs();
  funktion();
  if (!ch){
     anzeige();
  }
  else {
   anzeigecharging(); 
  }
  lcd_off++;

  //batterycontrol();
}

void sleepNow()       
{

  detachInterrupt(1); //erstmal die Interrupts aufräumen
  lcd.noBacklight();
  lcd.noDisplay();
  displayon=false;

  attachInterrupt(1,wakeUpNow, HIGH); 

  //Serial.println("sleeping");
  delay(10);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 

  detachInterrupt(1);      // disables interrupt 0 on pin 2 so the 


  attachInterrupt(1,aftersleep, HIGH);
  //lcd.display();


}

void wakeUpNow(){
  //Wake up Camera - "Auslöser antippen" wenn innerhalb der erlaubten Zeit
//  if(feuer)
//  {
//  cameraWakeUp();
//  }
}

void test()
{

}


void batterycontrol()
{
  battery.update();
  percentage = battery.getPercentage();
  voltage = battery.getVoltage(); 
  ch = battery.isCharging();  
  //if (ch) Serial.print("Charging");
}











