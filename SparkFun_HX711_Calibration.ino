/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This is the calibration sketch. Use it to determine the calibration_factor that the main example uses. It also
 outputs the zero_factor useful for projects that have a permanent mass on the scale in between power cycles.
 
 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Use this calibration_factor on the example sketch
 
 This example assumes pounds (lbs). If you prefer kilograms, change the Serial.print(" lbs"); line to kg. The
 calibration factor will be significantly different but it will be linearly related to lbs (1 lbs = 0.453592 kg).
 
 Your calibration factor may be very positive or very negative. It all depends on the setup of your scale system
 and the direction the sensors deflect from zero state

 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE

 Arduino pin 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND
 
 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.
 
 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
 
*/
// CLK pin op 2 en DOUT op 3~
#include "HX711.h"
int current_index = 1;
const int size = 5;
// Loadcell 1
#define CLK1  12
#define DOUT1  11

//Loadcell 2
#define CLK2 4
#define DOUT2 5

//Loadcell 3
#define CLK3 7
#define DOUT3 6

//Loadcell 4
#define CLK4 8
#define DOUT4 9

//Loadcell 5
#define CLK5 12
#define DOUT5 11


HX711 scale1(DOUT1, CLK1);/*
HX711 scale2(DOUT2, CLK2);
HX711 scale3(DOUT3, CLK3);
HX711 scale4(DOUT4, CLK4);
HX711 scale5(DOUT5, CLK5);*/



float calibration_factor = -404; //-7050 worked for my 440lb max scale setup


void setup() {
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  //Loadcell 1 setup
  scale1.set_scale();
  scale1.tare();	//Reset the scale to 0



  long zero_factor = scale1.read_average() ; //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
  delay(2000);
}

void loop() {
  delay(500);
  scale1.set_scale(calibration_factor); //Adjust to this calibration factor
  //scale2.set_scale(calibration_factor[1]); //Adjust to this calibration factor
  //scale3.set_scale(calibration_factor[2]); //Adjust to this calibration factor
  //scale4.set_scale(calibration_factor[3]); //Adjust to this calibration factor
  //scale5.set_scale(calibration_factor[4]); //Adjust to this calibration factor

//Loadcell 1
  if(current_index == 1)
  {
    Serial.print("Reading Loadcell 1: ");
    Serial.print(scale1.get_units(10), 2);
    Serial.print(" gram"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
    Serial.print(" calibration_factor_1: ");
    Serial.print(calibration_factor);
    Serial.println();
  }

  if(Serial.available())
  {
    char input = Serial.read();
    if(input == '+' || input == 'a')
      calibration_factor += 10;
    else if(input == '-' || input == 'q')
      calibration_factor -= 10;
    else if( input == 'z')
      calibration_factor += 1;
    else if( input == 's')
      calibration_factor -= 1;
    else if( input == 'e')
      calibration_factor += 100;
    else if( input == 'd')
      calibration_factor -= 100;


    else if(input == 'm'){
      Serial.print("current_index");
      Serial.println(current_index);
    }
  }

}
