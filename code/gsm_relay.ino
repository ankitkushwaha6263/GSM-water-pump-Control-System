#include <SoftwareSerial.h>

// --- Configuration ---
SoftwareSerial gsm(7, 8); // RX (Arduino Pin 7), TX (Arduino Pin 8)
int relay = 9;             // Arduino pin connected to the RELAY module
bool motorState = false;   // Tracks the current state of the motor/relay
String incoming = "";      // Buffer to store incoming data from the GSM module

// *** NOTE: CONFIRMATION_NUMBER is now redundant, but kept as a placeholder if needed later. ***
// *** SMS is now sent to ALL numbers in the AUTHORIZED_NUMBERS list. ***
const String CONFIRMATION_PLACEHOLDER = "+916263170768"; 

// *** AUTHORIZED NUMBERS (5 numbers for control and confirmation) ***
const String AUTHORIZED_NUMBERS[] = {
  "+916263170768", // 1. Original number
  "+919876543210", // 2. Example Authorized Number
  "+911122334455", // 3. Example Authorized Number
  "+917766554433", // 4. Example Authorized Number
  "+912233445566"  // 5. Example Authorized Number
};
const int NUM_AUTHORIZED = 5;

// --- Function Declarations ---
void clearBuffer();
// sendSMS ab recipient number aur message dono leta hai
void sendSMS(String recipientNumber, String text); 

// ------------------------------------
//          SETUP
// ------------------------------------
void setup() {
  // Initialize Relay Pin
  pinMode(relay, OUTPUT);

  // *** RELAY LOGIC REVERSED ***
  digitalWrite(relay, HIGH);

  // Initialize Serial Communications
  Serial.begin(9600);
  gsm.begin(9600);
  delay(1000);

  Serial.println("Initializing GSM...");

  // Basic AT Commands for GSM Setup
  gsm.println("AT");
  delay(1000);
  gsm.println("ATE0");    // Echo Off
  delay(1000);
  gsm.println("AT+CLIP=1");  // Enable Caller ID
  delay(1000);

  Serial.println("Setup Done. Waiting for call...");
}

// ------------------------------------
//           LOOP
// ------------------------------------
void loop() {
  if (gsm.available()) {
    char c = gsm.read();
    incoming += c;

    // Process the buffer when a newline character is received
    if (c == '\n') {

      // 1. Detect RING
      if (incoming.indexOf("RING") != -1) {
        Serial.println("RING detected!");
      }

      // 2. Detect Caller Number (+CLIP)
      if (incoming.indexOf("+CLIP:") != -1) {
        Serial.println("Incoming Call Details:");
        Serial.println(incoming);

        // --- Number Extraction ---
        String incomingNumber = "";
        int start = incoming.indexOf('"');
        int end = incoming.indexOf('"', start + 1);

        if (start != -1 && end != -1) {
          incomingNumber = incoming.substring(start + 1, end);
        }

        Serial.print("Extracted Number: ");
        Serial.println(incomingNumber);

        // *** AUTHORIZATION CHECK LOGIC ***
        bool isAuthorized = false;
        for (int i = 0; i < NUM_AUTHORIZED; i++) {
          if (incomingNumber.indexOf(AUTHORIZED_NUMBERS[i]) != -1 || AUTHORIZED_NUMBERS[i].indexOf(incomingNumber) != -1) {
            isAuthorized = true;
            break; 
          }
        }
        // *************************************************************

        if (isAuthorized) {

          Serial.println(">>> AUTHORIZED CALL: Proceeding with Action.");

          // --- Motor Logic ---
          motorState = !motorState;
          digitalWrite(relay, motorState ? LOW : HIGH);  // Active-LOW Logic

          String statusMsg = motorState ? "Motor turned ON via Call by " : "Motor turned OFF via Call by ";
          statusMsg += incomingNumber;
          Serial.println(motorState ? "Motor ON" : "Motor OFF");

          // 1. Hang up the call IMMEDIATELY (missed call logic)
          delay(500);
          gsm.println("ATH");
          delay(6000);

          clearBuffer();

          // 2. Send confirmation SMS to ALL authorized numbers
          for (int i = 0; i < NUM_AUTHORIZED; i++) {
              sendSMS(AUTHORIZED_NUMBERS[i], statusMsg);
              Serial.print("Confirmed Status to: ");
              Serial.println(AUTHORIZED_NUMBERS[i]);
          }


        } else {
          // --- UNAUTHORIZED ACTION ---
          Serial.print(">>> UNAUTHORIZED CALL: HANGING UP. Incoming: ");
          Serial.println(incomingNumber);

          // Just hang up silently to save cost
          delay(500);
          gsm.println("ATH");
          delay(6000);

          clearBuffer();  // Clean up "NO CARRIER" and buffer
        }
      }

      // Clear the line buffer for the next line of response
      incoming = "";
    }
  }
}

// ------------------------------------
//      HELPER FUNCTIONS
// ------------------------------------

/**
 * @brief Clears the serial buffer of the GSM module.
 */
void clearBuffer() {
  while (gsm.available()) {
    gsm.read();  // Read and discard available characters
  }
  incoming = "";
  Serial.println("Buffer cleared. Waiting for next call...");
}

/**
 * @brief Sends an SMS message to a specific recipient.
 */
void sendSMS(String recipientNumber, String text) {
  // NOTE: Sending 5 SMS in quick succession can drain power and cause temporary instability.
  // Ensure your external power supply and capacitor are robust!
  gsm.println("AT+CMGF=1");  // Set SMS to Text Mode
  delay(1000);
  
  gsm.print("AT+CMGS=\"");
  gsm.print(recipientNumber);
  gsm.println("\"");
  delay(1000);
  gsm.print(text);
  delay(100);
  gsm.write(26);  // ASCII code for Ctrl+Z to send the message
  delay(5000);    // Delay for SMS delivery and module response

  // *** THE CRITICAL FIX: Clear buffer after each SMS ***
  clearBuffer();
}
