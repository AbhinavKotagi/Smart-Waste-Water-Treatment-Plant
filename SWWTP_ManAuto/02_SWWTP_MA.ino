void tank1Process() {
    // Clear the LCD screen
    lcd.clear();
    // Display the message on the LCD
    lcd.setCursor(0, 0); // Set the cursor to the first row, first column
    lcd.print("pour water into:");
    lcd.setCursor(0, 1); // Move to the second row
    lcd.print("TANK 1");
}

void tank2Process() {

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tank 2 Process:");
    lcd.setCursor(0,1);
    lcd.print("STARTED");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("PROCESSING");
    // Turn on relay 1 for 2 minutes
    digitalWrite(relayPins[0], LOW); // Assuming relay 1 is at index 0 in relayPins array
    delay(60000);
    digitalWrite(relayPins[0], HIGH);

      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tank 2 Process:");
    lcd.setCursor(0,1);
    lcd.print("FINISHED");
}

void tank3Process() {

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tank 3 Process:");
    lcd.setCursor(0,1);
    lcd.print("STARTED");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("PROCESSING");

   // Turn on relay 2 till float switch 1 is triggered
        digitalWrite(relayPins[1], LOW); // Assuming relay 2 is at index 1 in relayPins array
      delay(60000);
      digitalWrite(relayPins[1], HIGH);

  // Measure pH level (assuming you have the code for this part)
  // Read pH value from the sensor
  float pHLevel = analogRead(pHsensorPin);

    // Turn on relay 3 and relay 4 for 10 seconds 
    digitalWrite(relayPins[2], LOW); // Assuming relay 4 is at index 3 in relayPins array  
    delay(20000); // 10 seconds delay
    digitalWrite(relayPins[3], LOW); // Assuming relay 3 is at index 2 in relayPins array
    delay(5000);
    digitalWrite(relayPins[3], HIGH);
    delay(5000);
    digitalWrite(relayPins[2], HIGH);


 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("METAL CONTENT");
    lcd.setCursor(0,1);
    lcd.print(StoredMetalDetect);
    delay(2000);
  

  // SCALING OF pH RAW VALUE TO SCALED VALUE
  pHLevel = map(pHLevel, 0, 1023, 0, 14);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("INITIAL pH VALUE:");
    lcd.setCursor(0,1);
    lcd.print(pHLevel);
    delay(3000);
    lcd.clear();
    lcd.print("PROCESSING");
  // Print pH value to Serial Monitor

  Serial.print("pH Value: ");
  Serial.println(pHLevel);

  digitalWrite(relayPins[4], LOW);

  // Wait for 500 milliseconds
  delay(500);

    // Call the Neutralization function
  Neutralization();

   // Wait for 30 seconds after Neutralization
  unsigned long startTime = millis();
  while (millis() - startTime < 30000) {
    // Wait for 30 seconds
  }

  // Turn off relay 7
  digitalWrite(relayPins[4], HIGH);
  Serial.print("tank 3 process finished");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tank 3 Process:");
    lcd.setCursor(0,1);
    lcd.print("FINISHED");


}


 void Neutralization() {
  unsigned long currentMillis = millis();
  int cycleInterval = 10000; // Interval between cycles in milliseconds
  int numCycles = 2; // Number of cycles to repeat

  // Variable to store the pH value
  float pHLevel = 0;

  for (int cycle = 0; cycle < numCycles; cycle++) {
    pHLevel = analogRead(pHsensorPin); // Store the pH value

    // Check pH level and trigger corresponding process
    if (pHLevel > 8) {
      MAprocess();
    } else if (pHLevel < 6.5) {
      MBprocess();
    }

    delay(cycleInterval); // Wait for the specified interval before starting the next cycle
  }

  // After 2 cycles, read pH value and store it
  storedpHValue = map(pHLevel, 0, 1023, 0, 14); // Map the pH level to a suitable range

  // Print the pH value on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PRESENT pH VALUE");
  lcd.setCursor(0, 1);
  lcd.print(storedpHValue);

  // Check if pH value is within the neutral range
  if (storedpHValue >= 6.5 && storedpHValue <= 8) {
    lcd.clear();
    lcd.print("Water is neutralized");
  }
}

void MAprocess() {
  int relayIndex = 5; // Index of the relay to control (Relay 5)
  int relayOnDuration = 1000; // Duration to keep the relay ON in milliseconds
  
  for (int i = 0; i < 1; i++) { // Repeat turning on the relay 2 times
    digitalWrite(relayPins[relayIndex], LOW); // Turn on relay
    delay(relayOnDuration); // Wait for the specified duration
    digitalWrite(relayPins[relayIndex], HIGH); // Turn off relay
    delay(relayOnDuration); // Wait for the specified duration
  }
}

void MBprocess() {
  int relayIndex = 6; // Index of the relay to control (Relay 6)
  int relayOnDuration = 1000; // Duration to keep the relay ON in milliseconds
  
  for (int i = 0; i < 1; i++) { // Repeat turning on the relay 2 times
    digitalWrite(relayPins[relayIndex], LOW); // Turn on relay
    delay(relayOnDuration); // Wait for the specified duration
    digitalWrite(relayPins[relayIndex], HIGH); // Turn off relay
    delay(relayOnDuration); // Wait for the specified duration
  }
}
void tank4Process(){
     lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tank 4 Process:");
    lcd.setCursor(0,1);
    lcd.print("STARTED");
     // Turn on relay 8 till float switch 2 is triggered
    digitalWrite(relayPins[7], LOW); 
    delay(30000);
     digitalWrite(relayPins[7], HIGH);
    delay(10000);
    digitalWrite(relayPins[7], LOW); 
    delay(2000);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tank 4 Process:");
    lcd.setCursor(0,1);
    lcd.print("FINISHED");
}


void enterLabMode() {
    // Clear the LCD screen
    lcd.clear();
    // Display "entering lab mode" on the LCD for 5 seconds
    lcd.setCursor(0, 0);
    lcd.print("ENTERING:");   
    lcd.setCursor(0, 1);
    lcd.print("LAB MODE");
    delay(5000); // Wait for 5 seconds

    // Clear the LCD screen
    lcd.clear();
    // Display "classifying the water quality" on the LCD for 3 seconds
    lcd.setCursor(0, 0);
    lcd.print("CLASSIFYING");
    lcd.setCursor(0, 1);
    lcd.print("WATER QUALITY");
    delay(3000); // Wait for 3 seconds
}
void LabTesting() {
    // Start the DS18B20 temperature sensor
    ds18b20.begin();

    // Request temperature from DS18B20 sensor
    ds18b20.requestTemperatures();
    
    // Read temperature from DS18B20 sensor
    float temperature = ds18b20.getTempCByIndex(0); // Read the temperature in Celsius
    
    // Read turbidity data from the turbidity sensor
    int turbidityValue = analogRead(turbiditySensorPin);
    
    // Store the turbidity value globally
    storedTurbidityValue = turbidityValue;
    StoredTemperature = temperature;  
    // Convert turbidity value to a meaningful value (adjust calibration if needed)
    // For demonstration, we'll just use the raw analog value
    StoredDataPage();
}
void Classification(float pHLevel, float storedTurbidityValue, float temperature) {
    // Clear the LCD screen
    lcd.clear();
    
    // Compare pH value and turbidity value to classify the water quality
    if (storedpHValue  >= 7.0 && storedpHValue  <= 7.5 && storedTurbidityValue >= 1 && storedTurbidityValue <= 50) {
        // Display "Water class type: A" on the LCD
        lcd.setCursor(0, 0);
        lcd.print("Water class type:");
        lcd.setCursor(0, 1);
        lcd.print("A");
    } else if (storedpHValue  >= 6.5 && storedpHValue  <= 8.0 && storedTurbidityValue >= 51 && storedTurbidityValue <= 200) {
        // Display "Water class type: B" on the LCD
        lcd.setCursor(0, 0);
        lcd.print("Water class type:");
        lcd.setCursor(0, 1);
        lcd.print("B");
    } else if (storedpHValue  >= 5.5 && storedpHValue  <= 8.0 && storedTurbidityValue >= 201 && storedTurbidityValue <= 500) {
        // Display "Water class type: C" on the LCD
        lcd.setCursor(0, 0);
        lcd.print("Water class type:");
        lcd.setCursor(0, 1);
        lcd.print("C");
    }
     else {
        // Display "Water class type: D" on the LCD
        lcd.setCursor(0, 0);
        lcd.print("Water class type:");
        lcd.setCursor(0, 1);
        lcd.print("D");
    } 
}
void handleHashButton() {
    // Call enterLabMode function
    enterLabMode();
    // Introduce a delay of 1 second (1000 milliseconds)
    delay(1000);
    
    // Call LabTesting function
    LabTesting();
    // Introduce a delay of 1 second
    delay(5000);
    
    // Get the pH value (assuming you have a function readPHLevel() that returns the pH value)
 float pHLevel = analogRead(pHsensorPin); 
    
    // Read the turbidity and temperature values from LabTesting
    // Modify LabTesting to return turbidity and temperature values or store them in global variables
    // For demonstration, we assume the variables turbidityValue and temperature are available globally
    float turbidityValue = analogRead(turbiditySensorPin);
    float temperature = ds18b20.getTempCByIndex(0);
    
    // Call Classification function with the values read
    Classification(pHLevel, storedTurbidityValue, temperature);
}
void SendData(){

   String dataToSend = String(StoredMetalDetect) + "," + String(storedpHValue ) + "," + String(storedTurbidityValue) + "," + String(StoredTemperature);
   // Send the data to ESP8266
   espSerial.println(dataToSend);
   Serial.println(String(StoredMetalDetect) + "," + String(storedpHValue ) + "," + String(storedTurbidityValue) + "," + String(StoredTemperature));
    

    // Clear the LCD screen
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("data pushed");   
    lcd.setCursor(0, 1);
    lcd.print("to cloud");
}
void RealTimePage(){

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LOADING: REAL");   
    lcd.setCursor(0, 1);
    lcd.print("TIME DATA");
    delay(2000);

  // parameter 1
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MtD:");   
    lcd.print(metalDetect);

    //parameter 2

    lcd.setCursor(8, 0);
    lcd.print("pH:");   
    lcd.print(pHLevel);
    // parameter 3

    lcd.setCursor(0, 1);
    lcd.print("TRB:");   
    lcd.print(turbidityValue);

    //parameter 4

    lcd.setCursor(8, 1);
    lcd.print("TMP:");   
    lcd.print(temperature);
}

void StoredDataPage(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LOADING: STORED");   
    lcd.setCursor(0, 1);
    lcd.print("TYPE DATA");
    delay(2000);

  // parameter 1
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MtD:");   
    lcd.print(StoredMetalDetect);

    //parameter 2

    lcd.setCursor(8, 0);
    lcd.print("pH:");   
    lcd.print(storedpHValue);

    // parameter 3

    lcd.setCursor(0, 1);
    lcd.print("TRB:");   
    lcd.print(storedTurbidityValue);

    //parameter 4

    lcd.setCursor(8, 1);
    lcd.print("TMP:");   
    lcd.print(StoredTemperature);

}

