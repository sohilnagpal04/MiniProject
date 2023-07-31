#include <SD.h>

// Constants
const int pirPin = 2;         // PIR motion sensor input pin
const int ledPin = 13;        // LED output pin
const int buzzerPin = 9;      // Buzzer output pin
const int motionThreshold = 1; // Minimum time (seconds) between motion events
const char* logFileName = "motion_log.csv"; // Log file name

// Variables
int motionState = 0;          // Variable to store motion sensor state
unsigned long lastMotionTime = 0;  // Variable to store the time of the last motion event

File dataFile;               // File object to handle data logging

void setup() {
  pinMode(pirPin, INPUT);    // Set PIR motion sensor pin as INPUT
  pinMode(ledPin, OUTPUT);   // Set LED pin as OUTPUT
  pinMode(buzzerPin, OUTPUT); // Set buzzer pin as OUTPUT
  Serial.begin(9600);        // Initialize serial communication for debugging (optional)

  // Initialize the SD card
  if (!SD.begin(10)) {
    Serial.println("SD card initialization failed!");
    while (1); // Halt the program if the SD card initialization fails
  }
  Serial.println("SD card initialized.");

  // Open the log file (create if it doesn't exist)
  dataFile = SD.open(logFileName, FILE_WRITE);
  if (!dataFile) {
    Serial.println("Error opening log file!");
  }
  else {
    // If the file is successfully opened, write the headers
    dataFile.println("Time (Seconds), Motion Detected");
  }
}

void loop() {
  // Read motion sensor state
  motionState = digitalRead(pirPin);

  if (motionState == HIGH && (millis() - lastMotionTime) > (motionThreshold * 1000)) {
    // Motion detected, turn on LED and buzzer
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    Serial.println("Motion detected!");

    // Log the motion event with timestamp (in seconds)
    dataFile.print(second());
    dataFile.print(", ");
    dataFile.println("Yes");

    lastMotionTime = millis();
  } else {
    // No motion or not enough time passed, turn off LED and buzzer
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);

    // Log the absence of motion event with timestamp (in seconds)
    dataFile.print(second());
    dataFile.print(", ");
    dataFile.println("No");
  }
}
