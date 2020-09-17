// THE VALUES OF RESISTORS IN SERIES WITH THERMISTOR (~10k)
#define SERIESRESISTOR 9910    
#define SERIESRESISTOR2 9828

// ARDUINO MICROCONTROLLER PINS TO CONNECT THERMISTORS TO 
#define THERMISTORPIN A1 
#define THERMISTORPIN2 A2


// THERMISTOR RESISTANCE AT NOMINAL TEMPERATURE (10K THERMISTOR)
#define THERMISTORNOMINAL 10000      

// TEMPERATURE AT NOMINAL RESISTANCE (ALMOST ALWAYS 25 C)
#define TEMPERATURENOMINAL 25  

//CONSTANT FOR CONVERSION
#define BCOEFFICIENT 3950

#include "VernierLib.h" //include Vernier functions 
VernierLib Vernier; //create an instance of the VernierLib library
 

 
void setup() {

// I. PINMODE SETUP

    Serial.begin(500000);                                             // Setup communication to display
    Vernier.autoID();                                               // Identify the sensor being used
    pinMode(A3, OUTPUT);                                            // Sets the pin A3 as output for Solenoid valve
    pinMode(A4, OUTPUT);}                                           // Sets the pin A4 as output for AC heater


 
void loop() {

// II. CO2 SENSOR CODE 

    float sensorReading;
    sensorReading = Vernier.readSensor();                           // Read one data value
    Serial.print(sensorReading);                                    // Print data value 
    Serial.println(" *ppm");                                        // Print units and skip to next line
    delay(500);                                                     // Wait half second
  
// III. PERFUSION CHAMBER THERMISTOR CODE
  
    float reading;
    reading = analogRead(THERMISTORPIN);

  // III.a CONVERT READING VALUE TO RESISTANCE
  
      reading = (1023 / reading)  - 1;                              // (1023/ADC - 1) 
      reading = SERIESRESISTOR / reading;                           // 10K / (1023/ADC - 1)


  // III.b CONVERT RESISTANCE TO DEGREES C
  
      float steinhart;                                              // Create new variable "steinhart"
      steinhart = reading / THERMISTORNOMINAL;                      // (R/Ro)
      steinhart = log(steinhart);                                   // ln(R/Ro)
      steinhart /= BCOEFFICIENT;                                    // 1/B * ln(R/Ro)
      steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15);             // + (1/To)
      steinhart = 1.0 / steinhart;                                  // Invert
      steinhart -= 273.15;                                          // convert to C
 
      Serial.print("Temperature: Perfusion Chamber  "); 
      Serial.print(steinhart);
      Serial.println(" *C");
 
      delay(1000);


  // III.c PERFUSION CHAMBER TEMPERATURE REGULATION CODE
  
      if (steinhart<37)
          digitalWrite(A4, HIGH);                                    // Turns AC heater on if it dips below required temperature 
      else 
           digitalWrite(A4, LOW);                                    // Turns AC heater off once the desired temperature is reached 
  


// IV. MEDIA RESERVOIR THERMISTOR CODE
  
    float reading2;
    reading2 = analogRead(THERMISTORPIN2);

  // IV.a CONVERT READING VALUE TO RESISTANCE
      reading2 = (1023 / reading2)  - 1;                             // (1023/ADC - 1) 
      reading2 = SERIESRESISTOR2 / reading2;                         // 10K / (1023/ADC - 1)


  // IV.b CONVERT RESISTANCE TO DEGREES C
      float steinhart2;                                              // Create new variable "steinhart2"
      steinhart2 = reading2 / THERMISTORNOMINAL;                     // (R/Ro)
      steinhart2 = log(steinhart2);                                  // ln(R/Ro)
      steinhart2 /= BCOEFFICIENT;                                    // 1/B * ln(R/Ro)
      steinhart2 += 1.0 / (TEMPERATURENOMINAL + 273.15);             // + (1/To)
      steinhart2 = 1.0 / steinhart2;                                 // Invert
      steinhart2 -= 273.15;                                          // convert to C
 
      Serial.print("Temperature: Reservoir  "); 
      Serial.print(steinhart2);
      Serial.println(" *C");
 
      delay(1000);

// V. MEDIA CO2 REGULATION CODE

  if (sensorReading < 300 )                                            // Logic gate reliant on CO2 reading in ppm
    digitalWrite(A3, HIGH);                                          // Opens solenoid valve letting CO2 into the system
  else                   
  digitalWrite(A3, LOW);                                             // Closes solenoid valve stopping flow of CO2
                  

}
