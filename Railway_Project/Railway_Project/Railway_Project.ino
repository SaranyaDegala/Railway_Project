#include <ESP32Servo.h>  // Include the Servo library for ESP32

// Pin declarations for ultrasonic sensors
const int trigPin1 = 23;  // Trigger pin for sensor 1
const int echoPin1 = 22;  // Echo pin for sensor 1
const int trigPin2 = 21;  // Trigger pin for sensor 2
const int echoPin2 = 19;  // Echo pin for sensor 2
const int trigPin3 = 18;  // Trigger pin for sensor 3
const int echoPin3 = 17;  // Echo pin for sensor 3
const int trigPin4 = 16;  // Trigger pin for sensor 4
const int echoPin4 = 15;  // Echo pin for sensor 4

// Pin declarations for LEDs, buzzer, and servo
const int greenLED = 25;  // Pin for green LED
const int redLED = 26;    // Pin for red LED
const int buzzer = 27;    // Pin for buzzer
const int servoPin = 13;  // Pin for controlling the servo motor

Servo gateServo;  // Create a Servo object to control the gate

// Function to measure distance using an ultrasonic sensor
// Parameters: trigPin and echoPin are the trigger and echo pins of the sensor
long measureDistance(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);  // Ensure the trigger pin is LOW
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);  // Send a 10 microsecond pulse to trigger pin
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);  // Measure the duration of the echo pulse
    return duration * 0.034 / 2;  // Convert the duration to distance in centimeters
}

void setup() {
    Serial.begin(115200);  // Initialize serial communication

    // Set pin modes for LEDs and buzzer
    pinMode(greenLED, OUTPUT);
    pinMode(redLED, OUTPUT);
    pinMode(buzzer, OUTPUT);

    // Set pin modes for ultrasonic sensors
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    pinMode(trigPin3, OUTPUT);
    pinMode(echoPin3, INPUT);
    pinMode(trigPin4, OUTPUT);
    pinMode(echoPin4, INPUT);  // Set echoPin4 as input to receive echo signal

    // Initialize the servo motor and set the gate to a closed position (0 degrees)
    gateServo.attach(servoPin);  // Attach the servo to the specified pin
    gateServo.write(0);  // Initially, close the gate (position at 0 degrees)
}

void loop() {
    // Measure the distance from all four ultrasonic sensors
    long distance1 = measureDistance(trigPin1, echoPin1);
    long distance2 = measureDistance(trigPin2, echoPin2);
    long distance3 = measureDistance(trigPin3, echoPin3);
    long distance4 = measureDistance(trigPin4, echoPin4);  // Measure distance for sensor 4

    // Logic to control LEDs, buzzer, and gate based on measured distances
    if (distance1 > 30 && distance2 > 30 && distance3 > 30 && distance4 > 30) {
        // All sensors show no obstacle (distances > 30cm)
        digitalWrite(greenLED, HIGH);   // Turn on green LED (safe to pass)
        digitalWrite(redLED, LOW);      // Turn off red LED
        digitalWrite(buzzer, LOW);      // Turn off the buzzer
        gateServo.write(90);            // Open the gate (position at 90 degrees)
    } 
    else if (distance1 < 30 && distance2 > 30 && distance3 > 30 && distance4 > 30) {
        // Obstacle detected by sensor 1 only
        digitalWrite(redLED, HIGH);     // Turn on red LED (obstacle detected)
        digitalWrite(greenLED, LOW);    // Turn off green LED
        digitalWrite(buzzer, HIGH);     // Turn on the buzzer (alert)
        gateServo.write(0);             // Close the gate (position at 0 degrees)
    } 
    else if (distance1 < 30 && distance2 < 30 && distance3 > 30 && distance4 > 30) {
        // Obstacle detected by sensors 1 and 2
        digitalWrite(redLED, HIGH);     // Turn on red LED
        digitalWrite(greenLED, LOW);    // Turn off green LED
        digitalWrite(buzzer, HIGH);     // Turn on the buzzer
        gateServo.write(0);             // Close the gate
    } 
    else if (distance1 > 30 && distance2 < 30 && distance3 < 30 && distance4 < 30) {
        // Obstacle detected by sensors 2, 3, and 4
        digitalWrite(redLED, HIGH);     // Turn on red LED
        digitalWrite(greenLED, LOW);    // Turn off green LED
        digitalWrite(buzzer, HIGH);     // Turn on the buzzer
        gateServo.write(0);             // Close the gate
    } 
    else if (distance1 > 30 && distance2 > 30 && distance3 > 30 && distance4 < 30) {
        // Obstacle detected by sensor 4 only
        digitalWrite(greenLED, HIGH);   // Turn on green LED
        digitalWrite(redLED, LOW);      // Turn off red LED
        digitalWrite(buzzer, LOW);      // Turn off the buzzer
        gateServo.write(90);            // Open the gate
    }

    delay(500);  // Add a delay of 500ms to avoid rapid sensor readings and output changes
}
