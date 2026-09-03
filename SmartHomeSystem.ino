#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo doorServo;

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

#define SERVO_PIN   13
#define RED_PIN     10
#define GREEN_PIN   11
#define BLUE_PIN    A3
#define BUZZER_PIN  A1
#define FAN_PIN     A0
#define SOUND_PIN   12
#define LM35_PIN    A2
#define FLAME_PIN   9

String input = "";
char lastKey = NO_KEY;

void setup() {
  lcd.init();
  lcd.backlight();
  doorServo.attach(SERVO_PIN);
  doorServo.write(0);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);
  pinMode(SOUND_PIN, INPUT_PULLUP);
  pinMode(FLAME_PIN, INPUT);

  lcd.setCursor(3, 0);
  lcd.print("Enter Code");
}

void loop() {
  // --- Flame / fire alarm ---
  int flameState = digitalRead(FLAME_PIN);
  if (flameState == HIGH) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("FIRE!!!");
    lcd.setCursor(2, 1);
    lcd.print("EVACUATE NOW");
    doorServo.write(90);

    while (digitalRead(FLAME_PIN) == HIGH) {
      digitalWrite(BUZZER_PIN, HIGH);
      setColor(255, 0, 0);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
      setColor(0, 0, 0);
      delay(100);
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FIRE EXTINGUISHED");
    lcd.setCursor(0, 1);
    lcd.print("Closing in 10s");
    delay(10000);

    doorServo.write(0);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Enter Code");
  }

  // --- Sound / clap detection ---
  int soundState = digitalRead(SOUND_PIN);
  if (soundState == LOW) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("CLAP DETECTED!");
    for (int i = 0; i < 5; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      setColor(0, 255, 255);
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

  // --- Keypad input ---
  char key = keypad.getKey();
  if (key != NO_KEY && key != lastKey) {
    lastKey = key;
    if (key == '#') {
      checkPassword();
      input = "";
    } else {
      input += key;
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
  }
  if (key == NO_KEY) lastKey = NO_KEY;

  // --- Temperature-controlled fan ---
  int analogValue = analogRead(LM35_PIN);
  float millivolts = (analogValue / 1023.0) * 5000.0;
  float temp = millivolts / 10.0;
  if (temp > 40) {
    digitalWrite(FAN_PIN, HIGH);
  } else {
    digitalWrite(FAN_PIN, LOW);
  }
}

void checkPassword() {
  if (input == "1234") {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Welcome");
    lcd.setCursor(0, 1);
    lcd.print("Dr.Momtaz");
    setColor(0, 0, 255);
    openDoor();
  } else if (input == "5678") {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Welcome");
    lcd.setCursor(0, 1);
    lcd.print("G9");
    setColor(0, 255, 0);
    openDoor();
  } else {
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
    delay(2000);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Enter Code");
    setColor(0, 0, 0);
  }
}

void openDoor() {
  doorServo.write(90);
  delay(6000);
  doorServo.write(0);
}

void setColor(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}
