/*Setra C264 +-2.5 WC, No.6 */

#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include "DHT.h"

#define DHTPIN 7
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);  

Adafruit_ADS1115 ads1115;  // construct an ads1115 at address 0x49


// DHT11 Sensor
float hum = 0;
float temp = 0;

int analogInput = A5;
float value = 0;

//Setra Properties Setra 3 
float low3 = -124.5444542; //Pa for Setra 264 -0,5"
float high3 = 124.5444542; //Pa for Setra 264 +0,5"
float range3 = 249.0889083; //Pascals
float vmin3 = 0; //V
float vmax3 = 6.144; //V
float r3 = 251; // 257 ohm

// Conversion from current to pressure, y = mx + b 
float m3 = 0;
float y3 = 0;
float x3 = 0;
float b3 = 0;


//Setra-1
float low1 = -62.27222708; //Pa for Setra 264 -0,5"
float high1 = -62.27222708; //Pa for Setra 264 +0,5"
float range1 = 124.5445; //Pascals
float vmin1 = 0; //V
float vmax1 = 6.144; //V
float r1 = 250; // 257 ohm

// Conversion from current to pressure, y = mx + b 
float m1 = 0;
float y1 = 0;
float x1 = 0;
float b1 = 0;

//Setra-5
float low5 = 0; //Pa
float high5 = 622.7222708; //Pa 
float range5 = 622.7222708; //Pascals
float vmin5 = 0; //V
float vmax5 = 6.144; //V
float r5 = 252; // 257 ohm

// Conversion from current to pressure, y = mx + b 
float m5 = 0;
float y5 = 0;
float x5 = 0;
float b5 = 0;


//Setra-6
float low6 = -622.7222708; //Pa for Setra 264 -0,5"
float high6 = 622.7222708; //Pa for Setra 264 +0,5"
float range6 = 1245.44; //Pascals
float vmin6 = 0; //V
float vmax6 = 6.144; //V
float r6 = 252; // 257 ohm

// Conversion from current to pressure, y = mx + b 
float m6 = 0;
float y6 = 0;
float x6 = 0;
float b6 = 0;


float i3 = 0; // sensor current [mA]
float i6 = 0; // sensor current [mA]
float i5 = 0; // sensor current [mA]
float i1 = 0; // sensor current [mA]
float imin = 4; // min current of sensor[mA]
float imax = 20; // max current of sensor[mA]

float voltage3 = 0; // sensor converted currtent to voltage [V]
float voltage6 = 0; // sensor converted currtent to voltage [V]
float voltage5 = 0; // sensor converted currtent to voltage [V]
float voltage1 = 0; // sensor converted currtent to voltage [V]


//density
float ro = 0; // kg/m3

// velocity
float velocity3 = 0;
float velocity5 = 0;
float velocity6 = 0;
float velocity1 = 0;

void setup() {
  // put your setup code here, to run once:

//Start Arduino
  pinMode(analogInput, INPUT);
  Serial.begin(9600);
  dht.begin();
  ads1115.begin();
}

void loop() {
    // put your main code here, to run repeatedly:
    int16_t adc1; // Setra-3
    int16_t adc3; // Setra-3
    int16_t adc6; // Setra-3
    int16_t adc5; // Setra-3

    adc1 = ads1115.readADC_SingleEnded(0); // Setra-1
    adc3 = ads1115.readADC_SingleEnded(1); // Setra-3
    adc6 = ads1115.readADC_SingleEnded(2); // Setra-6
    adc5 = ads1115.readADC_SingleEnded(3); // Setra-5

    voltage1 = (adc1 * 0.1875)/1000; //setra1
    voltage3 = (adc3 * 0.1875)/1000; // Setra-3
    voltage6 = (adc6 * 0.1875)/1000; //Setra6
    voltage5 = (adc5 * 0.1875)/1000; // Setra-5
    
    hum = dht.readHumidity();   // humidity reading  
    temp = dht.readTemperature();    // temperature reading
    ro =  352.6 / (dht.readTemperature() + 273.15) ; //air density

    // pressure sensor reading
   /* value = analogRead(analogInput); 
    v = (value * 5) / 1023; // voltage reading*/
    i1 = voltage1 / r1 * 1000; // current reading
    i3 = voltage3 / r3 * 1000; // current reading
    i6 = voltage6 / r6 * 1000; // current reading
    i5 = voltage5 / r5 * 1000; // current reading
        
    // current to pressure 3
    x3 = i3 - imin;
    m3 = range3 / ((imax - imin));
    b3 = low3;
    y3 = m3 * x3 + b3 ; //+ b

        // current to pressure 3
    x6 = i6 - imin;
    m6 = range6 / ((imax - imin));
    b6 = low6;
    y6 = m6 * x6 + b6 ; //+ b

        // current to pressure 2
    x5 = i5 - imin;
    m5 = range5 / ((imax - imin));
    b5 = low5;
    y5 = m5 * x5 ; //+ b

        // current to pressure 3
    x1 = i1 - imin;
    m1 = range1 / ((imax - imin));
    b1 = low1;
    y1 = m1 * x1 + b1 ; //+ b

    // velocity

   velocity3 = (sqrt((abs(2 * y3) / ro )))*0.85; // Setra-3
   velocity6 = (sqrt((abs(2 * y6) / ro )))*0.550; //Setra6
   velocity5 = (sqrt((abs(2 * y5) / ro )))*0.830; //Setra5
   velocity1 = (sqrt((abs(2 * y1) / ro )))*0.800; //Setra1
    
    Serial.print("V3 = "); Serial.print(voltage3,2);Serial.print("\t"); Serial.print("i3 = "); Serial.print(i3,2);Serial.print("\t");Serial.print("p3 = "); Serial.print(y3,2); Serial.print("\t"); Serial.print("u3 = ");Serial.print(velocity3,2); Serial.print("\t");Serial.print(" | "); 
    Serial.print("V6 = "); Serial.print(voltage6,2);Serial.print("\t"); Serial.print("i6 = "); Serial.print(i6,2);Serial.print("\t");Serial.print("p6 = "); Serial.print(y6,2); Serial.print("\t"); Serial.print("u6 = ");Serial.print(velocity6,2); Serial.print("\t");Serial.print(" | ");
    Serial.print("V5 = "); Serial.print(voltage5,2);Serial.print("\t"); Serial.print("i5 = "); Serial.print(i5,2);Serial.print("\t");Serial.print("p5 = "); Serial.print(y5,2); Serial.print("\t"); Serial.print("u5 = ");Serial.print(velocity5,2); Serial.print("\t");Serial.print(" | ");
    Serial.print("V1 = "); Serial.print(voltage1,2);Serial.print("\t"); Serial.print("i1 = "); Serial.print(i1,2);Serial.print("\t");Serial.print("p1 = "); Serial.print(y1,2); Serial.print("\t"); Serial.print("u1 = ");Serial.print(velocity1,2); Serial.print("\t");
    Serial.print("rho = "); Serial.print(ro,3);Serial.print("\t");Serial.println();
       
    delay(1000);

        if(Serial.available())
  {
    char test = Serial.read();
    if(test == 't' || test == 'T');      
  }
    if(Serial.available()){
        String input = Serial.readStringUntil('\n');
        Serial.println(input);
    }
}
