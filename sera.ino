#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <SoftwareSerial.h>



byte temrmometru[8] = {
	0b00100,
	0b01010,
	0b01010,
	0b01010,
	0b01110,
	0b11111,
	0b11111,
	0b01110
};


byte picatura[8] = {
	0b00000,
	0b00100,
	0b00100,
	0b01110,
	0b01110,
	0b11111,
	0b11111,
	0b01110
};

byte soare[8] = {
	0b00000,
	0b00000,
	0b10101,
	0b01110,
	0b11111,
	0b01110,
	0b10101,
	0b00000
};



int setTemp = 26;
int setHum = 1050;
int setLum = 5;

int a = 0;
int b = 0;
int c = 0;
int d = 0;

 int b1 = 8;
 int b2 = 9;
 int b3 = 10;
 int b4 = 11;
 int b5 = 12;
 int b6 = 13;
 int ventil = 4;


 int photoresistorPin = A2;
 int umiditatePin = A0;
 int pompa = 6;
 int bec = 5;

LiquidCrystal_I2C lcd(0x27,16,2);

DHT dht(7, DHT11);

float temp;
float btemp;


SoftwareSerial SIM900(1, 0); 


void setup() {
  
  SIM900.begin(19200);
  delay(2000);  
  sendSMS();

  lcd.init();
  lcd.backlight();

  pinMode(photoresistorPin,INPUT);
  
  pinMode(umiditatePin,INPUT);
  
  pinMode(pompa,OUTPUT);
  digitalWrite(pompa,LOW);

  pinMode(ventil,OUTPUT);
  digitalWrite(ventil,LOW);

  pinMode(bec,OUTPUT);
  digitalWrite(bec,LOW);

  dht.begin();  

  pinMode(b1,INPUT_PULLUP);
  pinMode(b2,INPUT_PULLUP);
  pinMode(b3,INPUT_PULLUP);
  pinMode(b4,INPUT_PULLUP);
  pinMode(b5,INPUT_PULLUP);
  pinMode(b6,INPUT_PULLUP);


          
}





void loop() {


lcd.clear(); 

// TEMPERATURA #################################################################
  
  temp = dht.readTemperature();
 
  if(!isnan(temp))
  {
  btemp = temp;
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("  ");

      lcd.setCursor(9,0);
      lcd.print(temp);

  if(temp>setTemp)
      {
       digitalWrite(ventil,HIGH);
       Serial.print(" Mere ventilatoru  ");

          if(d==0)
       {
             tempSMS();
             
       d=1;
       }

      }
   else
      {
        digitalWrite(ventil,LOW);
        Serial.print(" NUNU Mere ventilatoru  ");
        d=0;
      }


}
else{
  Serial.print("Temperature: ");
  Serial.print(btemp);
  Serial.print("  ");

      lcd.setCursor(9,0);
      lcd.print(btemp);

  if(btemp>setTemp)
      {
       digitalWrite(ventil,HIGH);
       Serial.print(" Mere ventilatoru  ");

           if(a == 0)
           {
             tempSMS();
              a=1;
           }

      }
   else
      {
        digitalWrite(ventil,LOW);
        Serial.print(" NUNU Mere ventilatoru  ");
        a=0;
      }
}



// LUMINA #################################################################

  int lightValue = analogRead(photoresistorPin);

  Serial.print("Light: ");
  Serial.print(lightValue);
  Serial.print("  ");

// LED #################################################################

   if(lightValue<setLum)
      {
       digitalWrite(bec,HIGH);
       Serial.print(" Mere ledu  ");

        if(b == 0 )
      {
             lumSMS();
       b=1;
      }

      }
   else
      {
        digitalWrite(bec,LOW);
        Serial.print(" NUNU Mere ledu  ");
        b=0;
      }

// UMIDITATE #################################################################

  int umedValue = analogRead(umiditatePin);

  Serial.print("Umiditate: ");
  Serial.print(umedValue);
  Serial.print("  ");



// POMPA ##################################################################
    if(umedValue>setHum)
      {
       digitalWrite(pompa,HIGH);
       Serial.print("  Mere pompa  ");

        if(c==0)
       {
         umedSMS();
       c=1;
       }
      }
    else
      {
        digitalWrite(pompa,LOW);
        Serial.print("  NUNU Mere pompa  ");
        c=0;
      }


// BUTONE ###########################################################################
 Serial.print("  ");
int but1 = digitalRead(b1);
Serial.print(but1);
 Serial.print("  ");
  if(but1 == 0)
    {
      setTemp++;
    }


  Serial.print("  ");
int but2 = digitalRead(b2);
Serial.print(but2);
 Serial.print("  ");
      if(but2 == 0)
        setTemp--;


  Serial.print("  ");
int but3 = digitalRead(b3);
if(but3 == 0)
    setHum = setHum+10;
Serial.print(but3);
 Serial.print("  ");


  Serial.print("  ");
int but4 = digitalRead(b4);
Serial.print(but4);
 Serial.print("  ");
       if(but4 == 0)
    setHum = setHum-10;


  Serial.print("  ");
int but5 = digitalRead(b5);
Serial.print(but5);
 Serial.print("  ");
    if(but5 == 0)
    setLum = setLum-1;

 Serial.print("  ");
int but6 = digitalRead(b6);
Serial.print(but6);
        if(but6 == 0)
    setLum = setLum + 1;
Serial.print("  ");

Serial.print("SetLum: ");
Serial.print(setLum);
Serial.print("  ");
Serial.print("SetHum: ");
Serial.print(setHum);
Serial.print("  ");
Serial.print("SetTemp: ");
Serial.print(setTemp);
Serial.println("  ");


 // LCD #########################################################################
      

      lcd.createChar(0, temrmometru);
      lcd.createChar(1, picatura);
      lcd.createChar(2, soare);
      lcd.setCursor(0,0);
      lcd.write(0);

      lcd.setCursor(0,1);
      lcd.write(1);
   
      lcd.setCursor(0,2);
      lcd.write(2);

      lcd.setCursor(2,0);
      lcd.print(setTemp);


      lcd.setCursor(2,1);
      lcd.print(setHum);

      lcd.setCursor(9,1);
      lcd.print(umedValue);

      lcd.setCursor(2,2);
      lcd.print(setLum);

      lcd.setCursor(9,2);
      lcd.print(lightValue);


delay(500);

}


void sendSMS() {
  
  
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT+CMGS=\"+40737605350\""); 
  delay(100);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  SIM900.println("A pornit arduino"); 
  delay(100);
  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000);


}

void tempSMS() {
  
  
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT+CMGS=\"+40737605350\""); 
  delay(100);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  SIM900.println("A pornit ventilatorul"); 
  delay(100);
  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000);


}

void umedSMS() {
  
  
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT+CMGS=\"+40737605350\""); 
  delay(100);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  SIM900.println("A pornit pompa"); 
  delay(100);
  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000);


}

void lumSMS() {
  
  
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT+CMGS=\"+40737605350\""); 
  delay(100);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  SIM900.println("A pornit becul"); 
  delay(100);
  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000);


}

