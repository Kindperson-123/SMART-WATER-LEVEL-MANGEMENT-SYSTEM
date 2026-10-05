// ================================
// SMART WATER MANAGEMENT SYSTEM
// ESP32 + Float Switches + Relay
// ================================

// Pin Definitions
#define LOW_FLOAT   26
#define HIGH_FLOAT  27

#define RELAY_PIN   25
#define LED_PIN     33
#define BUZZER_PIN  32

// Timer Variables for the 5-second LED/Buzzer delay
unsigned long tankFullStartTime = 0;
bool tankWasFull = false; 

// Timer Variables for Pump Run Time Tracking
unsigned long pumpStartTime = 0;
unsigned long totalPumpRuntime = 0; 
bool pumpIsRunning = false;

// Billing Constants
const float FLOW_RATE_LPS = 0.004; // 40 Liters per Second
const float COST_MULTIPLIER = 8.5;

void setup() {
  Serial.begin(115200);

  // Float switches 
  pinMode(LOW_FLOAT, INPUT_PULLUP);
  pinMode(HIGH_FLOAT, INPUT_PULLUP);

  // Outputs
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);


  // Initial state 
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);


  Serial.println("Smart Water Management System Started");
}

void loop() {
  // Read float switches
  bool lowFloat  = (digitalRead(LOW_FLOAT) == LOW);
  bool highFloat = (digitalRead(HIGH_FLOAT) == LOW);

  // 1. Tank Full (Both floats floating)
  if (!lowFloat && !highFloat) {
    digitalWrite(RELAY_PIN, LOW);     // Pump OFF immediately

    // If the pump WAS running, calculate times and the BILL
    if (pumpIsRunning) {
      unsigned long cycleDuration = millis() - pumpStartTime;
      totalPumpRuntime += cycleDuration;
      pumpIsRunning = false;

      // Convert milliseconds to seconds (using 1000.0 for decimal precision)
      float cycleSeconds = cycleDuration / 1000.0;
      float totalSeconds = totalPumpRuntime / 1000.0;

      // Calculate the bills using your formula: 40 * seconds * 8.5
      float cycleBill = FLOW_RATE_LPS * (cycleSeconds)* COST_MULTIPLIER;
      float totalBill = FLOW_RATE_LPS * (cycleSeconds)* COST_MULTIPLIER;

      // Print the Receipt to the Serial Monitor
      Serial.println("===================================");
      Serial.println("        PUMP CYCLE COMPLETE        ");
      Serial.println("-----------------------------------");
      
      Serial.print("Last Cycle Run Time: ");
      Serial.print(cycleSeconds); 
      Serial.println(" seconds");
      Serial.print("Last Cycle Bill:     Rs");
      Serial.println(cycleBill, 2); // The ', 2' formats it to 2 decimal places
      
      Serial.println("- - - - - - - - - - - - - - - - - -");
      
      Serial.print("Total Accumulated Time: ");
      Serial.print(totalSeconds); 
      Serial.println(" seconds");
      Serial.print("TOTAL ACCUMULATED BILL: Rs");
      Serial.println(totalBill, 2);
      
      Serial.println("===================================");
    }

    // Timer logic for the 5-second indicators
    if (!tankWasFull) {
      tankFullStartTime = millis(); 
      tankWasFull = true;
    }

    // Check if less than 5 seconds (5,000 milliseconds) have passed
    if (millis() - tankFullStartTime < 5000) {
      digitalWrite(LED_PIN, HIGH);      // LED ON
      digitalWrite(BUZZER_PIN, HIGH);   // Buzzer ON
    } else {
      digitalWrite(LED_PIN, LOW);       // Turn LED OFF after 5s
      digitalWrite(BUZZER_PIN, LOW);    // Turn Buzzer OFF after 5s
    }
  }
  else {
    // If the tank is not full, reset the tracker
    tankWasFull = false;

    // 2. Tank Empty (Both floats hanging down)
    if (lowFloat && highFloat) {
      digitalWrite(RELAY_PIN, HIGH);    // Pump ON
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);

      // If the pump wasn't already running, record the start time
      if (!pumpIsRunning) {
        pumpStartTime = millis();
        pumpIsRunning = true;
        Serial.println("Tank LOW -> Pump ON (Recording Time & Cost...)");
      }
    }

    // 3. Middle Zone (Water is between the two floats)
    else if (!lowFloat && highFloat) {
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
    }

    // 4. Safety fallback (Physically impossible state)
    else {
      digitalWrite(RELAY_PIN, LOW);     // Turn pump OFF for safety
      digitalWrite(LED_PIN, HIGH);      // Flash warning LED
      digitalWrite(BUZZER_PIN, HIGH);   // Sound warning buzzer

      if (pumpIsRunning) {
        totalPumpRuntime += (millis() - pumpStartTime);
        pumpIsRunning = false;
        Serial.println("PUMP EMERGENCY STOP -> Time Recorded.");
      }
    }
  }

  delay(500); 
}