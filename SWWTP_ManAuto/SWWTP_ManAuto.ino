#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

// Define the software serial pins on the ESP8266
SoftwareSerial espSerial(4, 5); // RX, TX (change these pins as per your setup)

// Pin definitions
int turbiditySensorPin = A4; // Pin for the turbidity sensor
int oneWireBusPin = A6; // Pin for the DS18B20 sensor (OneWire bus)

// Create a OneWire instance for the DS18B20 sensor
OneWire oneWireBus(oneWireBusPin);

// Create a DallasTemperature instance for the DS18B20 sensor
DallasTemperature ds18b20(&oneWireBus);

// Constants for Keypad
const byte ROWS_KEYPAD = 4; // Number of rows in the keypad
const byte COLS_KEYPAD = 4; // Number of columns in the keypad
char keys[ROWS_KEYPAD][COLS_KEYPAD] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins_keypad[ROWS_KEYPAD] = {36, 34, 32, 30}; // Keypad rows
byte colPins_keypad[COLS_KEYPAD] = {28, 26, 24, 22}; // Keypad columns
Keypad keypad = Keypad(makeKeymap(keys), rowPins_keypad, colPins_keypad, ROWS_KEYPAD, COLS_KEYPAD);

LiquidCrystal_I2C lcd(0x25, 16, 2); // Address 0x25, 16 columns and 2 rows

// Constants for relays
const int relayPins[] = {37, 35, 33, 31, 29, 27, 25, 23};
const int numRelays = sizeof(relayPins) / sizeof(relayPins[0]);

// Constants for other sensors and pins
 int floatSwitch1 = 53;
 int floatSwitch2 = 51;
 int pHsensorPin = A2;
const int MD1 = 49; // Pin connected to the sensor

int toggle1 = 3; // Auto pin
int toggle2 = 2; // Manual pin

int previousState1 = HIGH; // Initial state
int previousState2 = HIGH; // Initial state

unsigned long debounceDelay = 50; // milliseconds
unsigned long lastKeyPressTime = 0; // Last key press time

// Global variable to store the pH value
float storedpHValue = 0;
float turbidityValue = analogRead(turbiditySensorPin);
float pHLevel = analogRead(pHsensorPin);
float storedTurbidityValue = 0.0; // Initialize it with a default value, change data type as needed
float temperature = ds18b20.getTempCByIndex(0);
float StoredTemperature = 0.0;
float metalDetect = digitalRead(MD1);
float StoredMetalDetect = 0;

void setup() {
  Serial.begin(115200); // Initialize serial communication
   espSerial.begin(115200);
 lcd.begin(16, 2);
 lcd.init();
 lcd.backlight();
  pinMode(12, OUTPUT); // Warmup LED

  for (int i = 0; i < 9; i++) {
    digitalWrite(12, HIGH);  // Turn the LED on
    delay(200);                  // Wait for 200 milliseconds
    digitalWrite(12, LOW);   // Turn the LED off
    delay(200);                  // Wait for 200 milliseconds before next blink
  }
  
  for (int i = 0; i < numRelays; i++) {
    pinMode(relayPins[i], OUTPUT); // Set relay pins as outputs
    digitalWrite(relayPins[i], HIGH); // Turn off all relays initially (active low)
  }

  
  pinMode(floatSwitch1, INPUT_PULLUP); // Initialize input pins
  pinMode(floatSwitch2, INPUT_PULLUP);

  pinMode(toggle1, INPUT_PULLUP);
  pinMode(toggle2, INPUT_PULLUP);

  pinMode(MD1, INPUT); // Set sensor pin as input

  homesetup();
}

void loop() {

  unsigned long currentMillis = millis();

  char key = keypad.getKey(); // Read the pressed key

  int currentState1 = digitalRead(toggle1);
  int currentState2 = digitalRead(toggle2);

  if (currentState1 == LOW) { // Auto mode
    if (key && (currentMillis - lastKeyPressTime >= debounceDelay)) {
      lastKeyPressTime = currentMillis; // Update last key press time
      Serial.println(key); // Print the key to the serial monitor
      handleAutoKey(key); // Call the function to handle the key press in auto mode
    }
  } else if (currentState2 == LOW) { // Manual mode
    if (key && (currentMillis - lastKeyPressTime >= debounceDelay)) {
      lastKeyPressTime = currentMillis; // Update last key press time
      Serial.println(key); // Print the key to the serial monitor
      handleManualKey(key); // Call the function to handle the key press in manual mode
    }
  } else {
    // Update previous state
    previousState1 = currentState1;
    previousState2 = currentState2;
  }

  delay(50); // Adjust delay as needed for keypad responsiveness
  // Add any other continuous operations here
   // Sample sensor data

if(digitalRead(MD1) == HIGH){
    delay(1000);
    metalDetect = true; 
    delay(1000);
   }else{
    metalDetect = false;
   }
   
   if(metalDetect == true){
     StoredMetalDetect = true;
     delay(1000);
   }
}


void handleAutoKey(char key) {
  switch (key) {
    case '3':
      tank2Process();
      delay(500);
      tank3Process();
      delay(500); 
      tank4Process();
      break;

    case '6':
     SendData();
      break;
    case '#':
     handleHashButton();
      break;
        case 'B':
     RealTimePage();
      break;
  case 'C':
     StoredDataPage();
      break;
    default:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Unknown key");
      lcd.setCursor(0,1);
      lcd.print("in AUTO mode");
  }
}

void handleManualKey(char key) {
  switch (key) {
     case '1':
      tank1Process();
      break; 

    case '4':
      tank2Process();
      break;

    case '7':
      tank3Process();
      break;

       case '*':
      tank4Process();
      break;
    case '6':
     SendData();
      break;
  case '#':
     handleHashButton();
      break;
    case 'B':
     RealTimePage();
      break;
  case 'C':
     StoredDataPage();
      break;

    default:
     lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Unknown key");
    lcd.setCursor(0,1);
    lcd.print("in MAN mode");
  }

 
}

void homesetup(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO");
  lcd.setCursor(0, 1);
  lcd.print("SWWTP");
}

