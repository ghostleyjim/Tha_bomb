#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

//display configuration via I2C
LiquidCrystal_I2C lcd(0x3F, 16, 2);

//pin configuration
byte keypad = A1;
byte keySw = 3;
byte switch1 = 4;
byte switch2 = 5;
byte switch3 = 6;
byte switch4 = 7;
#define PIN  8

//Neopixel stuff
#define NUMPIXELS   4
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

long time;

//timer display config data
long timekeep;
byte minutes = 50;
byte seconds = 0;

//flags to set events
bool armedFlag = false;
bool flag = false;
bool firstFlag = false;
bool secondFlag = false;
bool thirdFlag = false;
bool fourthFlag = false;
bool wireFlag = false;
bool displayFlag = false;
//keycodes
byte armCode[4] = {0, 0, 0, 0};
byte disarmCode[4] = {0, 0, 0, 0}; 

//sequences for disarming
byte wireColors[4] = {1, 2, 3, 4};
byte disarmStages[4] = {1, 2, 3, 4};


void setup(){
//debugging serial line
Serial.begin(9600);

//set the keyswitch pinmode
pinMode(keySw, INPUT_PULLUP);

//lcd display startup
lcd.init();

lcd.backlight();

//Use analog pin to use for pseudo randomness
randomSeed(analogRead(A3));

//create armcode and disarm code arrays
for(byte i = 0; i <= 3; i++)
{
    armCode[i] = random(0, 9);
    disarmCode[i] = random(0,9);
}

//randomize the wirecut module and determin what module to start with
for(int j = 0; j <= 3; j++)
{
    int k = random(j, 3);

    int l = wireColors[j];
    wireColors[j] = wireColors[k];
    wireColors[k] = l;
    int m = disarmStages[j];
    disarmStages[j] = disarmStages[k];
    disarmStages[k] = m;
}

//start with show what to do.
lcd.setCursor(0,0);

lcd.print("Sleutel om bom");

lcd.setCursor(0,1);
lcd.print("te activeren");

}

void loop()
{
//arm the bomb
while (armedFlag == false)
{
arming();
}
//disarm fase
while(armedFlag == true)
{
    if(flag == true)
    {
        lcd.clear();
        flag = false;
    }
    
    //first stage to disarm bomb
   while (firstFlag == false)
 {
 runtimer();
 firststage();
 }
}

}


void arming(){   
    if( digitalRead(keySw) == LOW)
    {
        if (flag == false)
        {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Vul code in: ");
        lcd.setCursor(0, 1);
        for(byte i = 0; i <= 3; i++)
        {
            lcd.print(armCode[i]);
        }
        flag = true;    
    } 
    armedFlag = true;
}
}

void runtimer()
{
    //run timer on display

    if (millis() - timekeep >= 1000)
    {
        seconds = (seconds - 1);
        if (seconds == 255)
        {
            seconds = 59;
            minutes--;
        }
    timekeep = millis();
    }
    
    lcd.setCursor(6, 0);
    lcd.print(minutes);
    lcd.print(":");
    if (seconds < 10)
    {
        lcd.print(0);
    }
    lcd.print(seconds);

if (minutes == 0 && seconds == 0)
    {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("YOU LOSE!");
        while(0 == 0)
        {
           lcd.noBacklight();
           delay(1000);
           lcd.backlight();
           delay(1000);
        }
}
}


void firststage()
//first of 4 stages to disarm bomb chosen from the random array disarmStages
{
int i;
i = disarmStages[0];
switch (i)
{
case (1):
    wire();
    break;
case (2):
    keys();
    break;
case (3):
    colorcode();
    break;
default:
    break;
}
}

void wire()
{
if (displayFlag == false)
{
lcd.setCursor(0,1);
lcd.print("cut wire");
displayFlag = true;
time = millis();
}

if (millis() - time >= 5000)
{
lcd.setCursor(0,1);
lcd.print("test       ");

}
}


void keys()
{
lcd.setCursor(0,1);
lcd.print("now in keys");
}

void colorcode()
{
lcd.setCursor(0,1);
lcd.print("now in colorcode");
}
