/* Setra C264  +-0,25" */

#include <max6675.h>

/*voltmeter*/
int analogInput = A1;
float vout = 0.0;
float vin = 0.0;
float R1 = 30000.0; //  
float R2 = 7500.0; // 
int value = 0;

//pressure probe
float low = -62.27222708; /* for Setra 264 -0,25"*/
float high = 62.27222708; /* for Setra 264 +0,25"*/
float range = high - low;
float voltage = 5; /*Setra 264*/
float pressure = 0;

//pressure
float pa = 0;

//thermocouple
int thermoDO = 9;
int thermoCS = 8;
int thermoCLK = 13;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

//density
float ro = 0;

//velocity
float velocity = 0;
float k = 1.08; //0.66; callibration factor 0.58

//Measurement - current conversion to Voltage
float r = 260; /*ohm*/
float i = 0; // sensor current [mA]
float imin = 4; // min current of sensor[mA]
float imax = 20; // max current of sensor[mA]
float v = 0; // sensor converted currtent to voltage [V]
float vmin = 1; /*V*/
float vmax = 5; /*V*/

void setup() {
  // put your setup code here, to run once:
//Start Arduino
   pinMode(analogInput, INPUT);
   Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 
   float mapping =  map(analogRead (A0),205, 1023, -124.544454166665, 124.544454166665);

   ro =  352.6/ (thermocouple.readCelsius() + 273.15);

  value = analogRead(analogInput); 

  v = (value * 5) / 1024;

  i = v/r;
  
  pa = range * (v - vmin) / (vmax - vmin) + low; //   !!!works

  velocity = (sqrt((abs(2 * pa) / ro )));


  // Results printing
  Serial.print("raw value = ");
  Serial.print(value,2);
  Serial.print("  mapping = ");
  Serial.print(mapping,2);
  Serial.print("            ");
  Serial.print("INPUT V= ");
  Serial.print(v,2);
  Serial.print(" [V]            ");
  Serial.print("INPUT C= ");
  Serial.print(i,4);
  Serial.print(" [mA]            ");
  Serial.print(pa,2);
  Serial.print(" [Pa]            ");
  Serial.print("C = "); 
  //Serial.print(thermocouple.readCelsius());
  Serial.print(thermocouple.readCelsius());
  Serial.print("[ °C]            ");
  Serial.print("RO = "); 
  Serial.print(ro,3);
  Serial.print(" [kg/m3]            ");  
  Serial.print(velocity,2);
  Serial.print(" [m/s]            ");  
  Serial.println();
  delay(1000);
}
