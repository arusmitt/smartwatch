/*
 * Code Overview: 
 * Take averages of EMG sensor voltage readings, and based on a pre-defined number of points and threshold, determine whether a repetition of the desired 
 * exercise met the threshold for enough time. Similarly, take voltage reading from the flex sensor, and use a standard curve to determine wrist bending 
 * angle, again checking if the repetition's angle meets a desired pre-defined threshold. Print user feedback to the LCD screen based on whether bending 
 * angle and strength are met for the repetition. 
 */

// Initialize LCD variables 
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Initialize the  LCD library and what pins to communicate with on the display 


// Initialize EMG variables 
const int points = 50; // Number of points used to collect a moving average
int values [points]; // Array for moving average
int count = 0; // Count to check how many data samples have been taken
const int pinRead = A0; // Output data from EMG electrodes
const float thresh = 0.1; // Threshold signal to determine if muscle has contracted
int switchState = 0; // Marks whether the mean is above or below threshold
int prevSwitchState = 0; // Notes whether the mean was above or below threshold


// Initialize flex sensor variables 
int sensorValue = 0; 
int angleThresh = 30; 


void setup() {
  // EMG setup 
  Serial.begin(9600); // Set the data rate for the serial data transmission 
  pinMode(3, OUTPUT);
  // LCD setup 
  lcd.begin(16, 2); // Set screen size
  lcd.print("Wrist Therapy"); // Display the introductory message
  lcd.setCursor(0,1); // Set the cursor to the indices of the first row and column 
  
}


void loop() {
  // EMG sensor readings and calculations 
  float sum = 0.0; 
  float mean = 0.0; 
  // Rolling average of predetermined number of points. This smooths the signal. 
  if (count < points - 1){
    values[count] = analogRead(pinRead);
  }
  else{
    for (int j =0; j<= points -2; j++){
      values[j] = values[j+1];
    }
    values[points-1] = analogRead(pinRead);
  } 
  for(int i = 0; i<points; i++){
    sum = sum + values[i];
  }
  mean = sum/points;
  count = count+1;


 
  // FLEX sensor readings and calculations 
  sensorValue = analogRead(A1); // Read in the output of the flex sensor 
  float voltage = sensorValue * (5.0 / 1023.0); // Convert sensor reading to voltage 
  
  // Convert voltage to angle based on calibration curve 
  float angle = 169.27 * voltage - 173.33;
  

  // User feedback displayed on LCD 

  // If the Flex and the EMG thresholds are not met, prompt user to squeeze their muscle more and bend their wrist further 
  if (angle < angleThresh && mean < thresh){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Squeeze & Bend"); 
  }

  // If the Flex threshold is met and the EMG threshold is not, prompt user to squeeze their muscle more  
  if (angle >= angleThresh && mean < thresh){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Squeeze harder!"); 
  }

// If the both thresholds are met, give positive feedback to user 
  if (angle >= angleThresh && mean >= thresh){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("You got it!"); 
  } 
 
}
