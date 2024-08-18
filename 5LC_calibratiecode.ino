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


float S = 0.02;
float rho = 1.29;
float V = 10.0;
float P = 50;
float c = 0.1;

const int size = 4;
int current_index = 0;
int output_index = 0;
int cali_index = current_index + size - 1;
float AOA = 0.0;
float AOA_deg = 0.0;
float distLC1 = 0.00275;  //distance to Loadcell 1 
float distLC2 = 0.04425;  //distance to Loadcell 2 


// Loadcell 1 = Drag back
#define CLK1 8
#define DOUT1 9

//Loadcell 2 = Drag front 
#define CLK2 4
#define DOUT2 5

//Loadcell 3 = Lift back
#define CLK3 7
#define DOUT3 6

//Loadcell 4 = Lift front
#define CLK4 2
#define DOUT4 3


HX711 scaleDB(DOUT1, CLK1);
HX711 scaleDF(DOUT2, CLK2);
HX711 scaleLB(DOUT3, CLK3);
HX711 scaleLF(DOUT4, CLK4);

float degreesToRadians(float degrees) {
  return degrees * M_PI / 180.0;
}


float calibration_factor[] = { 22, 32, 1867, 155};  


void setup() {
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");

  // Prompt the user to input the Angle of Attack
  Serial.println("Please input the Angle of Attack (in degrees):");

  while (!Serial.available()) {
    // Wait until data is available
  }

  // Read the input from the serial monitor
  AOA_deg = Serial.parseFloat();

  // Convert AoA from degrees to radians
  AOA = degreesToRadians(AOA_deg);


  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  //Loadcell 1 setup
  scaleDB.set_scale();
  scaleDB.tare();  //Reset the scale to 0

  //Loadcell 2 setup
  scaleDF.set_scale();
  scaleDF.tare();  //Reset the scale to 0

  //Loadcell 3 setup
  scaleLB.set_scale();
  scaleLB.tare();  //Reset the scale to 0

  //Loadcell 4 setup
  scaleLF.set_scale();
  scaleLF.tare();  //Reset the scale to 0


  long zero_factor[] = { scaleDB.read_average(), scaleDF.read_average(), scaleLB.read_average(), scaleLF.read_average()};  //Get a baseline reading
  // Print all elements of zero_factor array
  for (int i = 0; i < 4; i++) {
    Serial.println(zero_factor[i]);
  }
  delay(2000);
}
void loop() {
  delay(500);

  //Display during calibration
  //Loadcell 1
  if (current_index == 0 && output_index == 0) {
    float D_back = scaleDB.get_units(10);
    Serial.print("Reading Drag back: ");
    Serial.print(D_back, 2);
    Serial.print(" miligram");  
    Serial.print(" calibration_factor_1: ");
    Serial.print(calibration_factor[0]);
    Serial.println();
  }
  //Loadcell 2
  if (current_index == 1 && output_index == 0) {
    float D_front = scaleDF.get_units(10);
    Serial.print("Reading drag front: ");
    Serial.print(D_front, 2);
    Serial.print(" miligram");  
    Serial.print(" calibration_factor_2: ");
    Serial.print(calibration_factor[1]);
    Serial.println();
  }

  //Loadcell 3
  if (current_index == 2 && output_index == 0) {
    float LB = -scaleLB.get_units(10);
    Serial.print("Reading Lift back: ");
    Serial.print(LB, 2);
    Serial.print(" gram");  
    Serial.print(" calibration_factor_3: ");
    Serial.print(calibration_factor[2]);
    Serial.println();
  }
  //Loadcell 4
  if (current_index == 3 && output_index == 0) {
    float LF = scaleLF.get_units(10);  //L2 = LF
    Serial.print("Reading lift front: ");
    Serial.print(LF, 2);
    Serial.print(" gram");  
    Serial.print(" calibration_factor_4: ");
    Serial.print(calibration_factor[3]);
    Serial.println();
  }
  
  if (output_index == 1) {
    V = 23.07 * (P/100) + 0.573;  //Equation Velocity in function of the Power of the windtunnel
    
    //Getting the values from each loadcell
    float D_back = scaleDB.get_units(1);
    float D_front = scaleDF.get_units(1);
    float LB = -scaleLB.get_units(1);
    float LF = scaleLF.get_units(1);

    float tot_normal = (LB * 9.81 / 1000) + (LF * 9.81 / 1000); // Formula total Normal Force
    float tot_axial = (D_front * 9.81 / 1000000) + (D_back * 9.81 / 1000000); //Formula total Axial Force

    float tot_lift = tot_normal * cos(AOA) - tot_axial * sin(AOA); //Formula total Lift
    float tot_drag = -(tot_normal * sin(AOA) + tot_axial * cos(AOA)); //Formula total Drag
    float pitch_moment = (-LF * 9.81 / 1000) * (distLC1) - (-LB * 9.81 / 1000) * (distLC2); //Formula total Moment

    float Cl = -tot_lift / (0.5 * rho * V * V * S); //Formula total Lift coefficient
    float Cd = tot_drag / (0.5 * rho * V * V * S); //Formula total Drag coefficient
    float Cm = pitch_moment / (0.5 * rho * V * V * S * c); //Formula total Moment coefficient

// Display for the speed and all coefficients
    Serial.print(V);
    Serial.print(", ");
    Serial.print(tot_lift, 6);
    Serial.print(", ");
    Serial.print(tot_drag, 6);
    Serial.print(", ");
    Serial.print(pitch_moment, 6);
    Serial.println();
  }

  //Commands during calibration
  if (Serial.available()) {
    char input = Serial.read();
    if (input == 'a')
      calibration_factor[current_index] += 1;
    else if (input == 'q')
      calibration_factor[current_index] -= 1;
    else if (input == 'z')
      calibration_factor[current_index] += 10;
    else if (input == 's')
      calibration_factor[current_index] -= 10;
    else if (input == 'e')
      calibration_factor[current_index] += 100;
    else if (input == 'd')
      calibration_factor[current_index] -= 100;
    else if (input == 'n')
      current_index = (current_index + 1) % size;
    else if (input == 'p')
      current_index = (current_index + size - 1) % size;
    else if (input == 'o')
      output_index = (output_index + 1) % 2;
    else if (input == 't') {
      scaleDB.tare();
      scaleDF.tare();
      scaleLB.tare();
      scaleLF.tare();
    } else if(input == 'w'){
      Serial.println(V);
      Serial.println(P);
      Serial.println(S);
      Serial.println(rho);
      Serial.println(AOA_deg);
    }else if(input == '-'){
      P = P-1;
    }else if(input =='+'){
      P = P+1;
    }
    

  }
  //int cali_index = (current_index + size + 1)%size;
  scaleDB.set_scale(calibration_factor[0]);   //Adjust to this calibration factor
  scaleDF.set_scale(calibration_factor[1]);  //Adjust to this calibration factor
  scaleLB.set_scale(calibration_factor[2]);   //Adjust to this calibration factor
  scaleLF.set_scale(calibration_factor[3]);   //Adjust to this calibration factor

}
