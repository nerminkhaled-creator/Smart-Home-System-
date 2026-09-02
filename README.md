# Smart-Home-System-
An integrated Smart Home Security &amp; Safety system based on Arduino, featuring keypad access control, fire detection with auto-evacuation, and temperature-regulated cooling. 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

// Initialize LCD and Servo
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo doorServo;

// Keypad configuration
const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Pin Definitions
#define SERVO_PIN 13
#define RED_PIN 10
#define GREEN_PIN 11
#define BLUE_PIN A3
#define BUZZER_PIN A1
#define FAN_PIN A0
#define SOUND_PIN 12
#define LM35_PIN A2
#define FLAME_PIN 9 // Flame sensor connected to digital pin 9

String input = "";
char lastKey = NO_KEY;

void setup() {
  lcd.init();
  lcd.backlight();
  
  doorServo.attach(SERVO_PIN);
  doorServo.write(0); // Initial position (Closed)

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);

  pinMode(SOUND_PIN, INPUT_PULLUP);
  pinMode(FLAME_PIN, INPUT); 

  // Initial UI message
  lcd.setCursor(3, 0);
  lcd.print("Enter Code");
}

void loop() {
  // 1. Fire Detection System (Highest Priority)
  int flameState = digitalRead(FLAME_PIN);
  
  if (flameState == HIGH) { // Sensor sends HIGH when flame is detected
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("FIRE!!!");
    lcd.setCursor(2, 1);
    lcd.print("EVACUATE NOW");
    
    doorServo.write(90); // Open door immediately for evacuation
    
    // Maintain alarm as long as flame is present
    while(digitalRead(FLAME_PIN) == HIGH) { 
      digitalWrite(BUZZER_PIN, HIGH);
      setColor(255, 0, 0); // Red alert
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
      setColor(0, 0, 0);
      delay(100);
    }

    // After fire is extinguished
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FIRE EXTINGUISHED");
    lcd.setCursor(0, 1);
    lcd.print("Closing in 10s");

    delay(10000); // 10-second delay before locking the door
    
    doorServo.write(0); // Lock door automatically
    
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Enter Code");
  }

  // 2. Sound Detection System (Clap)
  int soundState = digitalRead(SOUND_PIN);
  if (soundState == LOW) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("CLAP DETECTED!");
    for (int i = 0; i < 5; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      setColor(0, 255, 255); // Cyan alert
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
      setColor(0, 0, 0);
      delay(100);
    }
    delay(1000);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Enter Code");
  }

  // 3. Keypad and Password System
  char key = keypad.getKey();
  if (key != NO_KEY && key != lastKey) {
    lastKey = key;
    if (key == '#') {
      checkPassword();
      input = ""; // Clear input after check
    } else {
      input += key;
      lcd.setCursor(0, 1);
      lcd.print("                "); // Clear line
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
  }
  if (key == NO_KEY) lastKey = NO_KEY;

  // 4. Cooling System (LM35 Temperature Sensor)
  int analogValue = analogRead(LM35_PIN);
  // Convert analog reading to voltage, then to Celsius
  float millivolts = (analogValue / 1023.0) * 5000.0;
  float temp = millivolts / 10.0; 

  if (temp > 40) { 
    digitalWrite(FAN_PIN, HIGH); // Turn on fan if temp exceeds 40°C
  } else {
    digitalWrite(FAN_PIN, LOW);
  }
}

void checkPassword() {
  if (input == "1234") {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Welcome");
    lcd.setCursor(3, 1);
    lcd.print("Dr.Momtaz");
    setColor(0, 255, 0); // Green for access granted
openDoor();
  } 
  else if (input == "5678") {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Welcome");
    lcd.setCursor(5, 1);
    lcd.print("G9");
    setColor(255, 0, 255); // Purple for access granted
    openDoor();
  } 
  else {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Access Denied");
    for (int i = 0; i < 3; i++) {
      setColor(0, 255, 255);
      digitalWrite(BUZZER_PIN, HIGH);
      delay(200);
      digitalWrite(BUZZER_PIN, LOW);
      setColor(0, 0, 0);
      delay(200);
    }
  }
  delay(2000);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Enter Code");
  setColor(0, 0, 0);
}

void openDoor() {
  doorServo.write(90); // Open position
  delay(6000);        // Wait for 6 seconds
  doorServo.write(0);  // Close position
}

void setColor(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}
