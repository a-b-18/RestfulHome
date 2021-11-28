#include <Arduino.h>
#include <assert.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>
#include <IRsend.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


// BEGIN HTTP SERVER

#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);

boolean rewrite1;
boolean rewrite2;
boolean rewrite3;
boolean rewrite4;

boolean send1;
boolean send2;
boolean send3;
boolean send4;

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  // Send 404 response
  server.send(404, "text/plain", message);
}

// END HTTP SERVER


const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.
uint16_t rawdata[99] = {};
int rawdataLength = 0;
boolean latch = false;

// ==================== start of TUNEABLE PARAMETERS ====================
// An IR detector/demodulator is connected to GPIO pin 14
// e.g. D5 on a NodeMCU board.
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
const uint16_t kRecvPin = 14;

// The Serial connection baud rate.
// i.e. Status message will be sent to the PC at this baud rate.
// Try to avoid slow speeds like 9600, as you will miss messages and
// cause other problems. 115200 (or faster) is recommended.
// NOTE: Make sure you set your Serial Monitor to the same speed.
const uint32_t kBaudRate = 115200;

// As this program is a special purpose capture/decoder, let us use a larger
// than normal buffer so we can handle Air Conditioner remote codes.
const uint16_t kCaptureBufferSize = 1024;


#if DECODE_AC
// Some A/C units have gaps in their protocols of ~40ms. e.g. Kelvinator
// A value this large may swallow repeats of some protocols
const uint8_t kTimeout = 50;
#else   // DECODE_AC
// Suits most messages, while not swallowing many repeats.
const uint8_t kTimeout = 15;
#endif  // DECODE_AC
const uint16_t kMinUnknownSize = 12;
const uint8_t kTolerancePercentage = kTolerance;  // kTolerance is normally 25%

// Legacy (No longer supported!)
//
// Change to `true` if you miss/need the old "Raw Timing[]" display.
#define LEGACY_TIMING_INFO false
// ==================== end of TUNEABLE PARAMETERS ====================

// Use turn on the save buffer feature for more complete capture coverage.
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;  // Somewhere to store the results

// This section of code runs only once at start-up.
void setup() {
  irsend.begin();
//#if defined(ESP8266)
  Serial.begin(kBaudRate, SERIAL_8N1, SERIAL_TX_ONLY);
//#else  // ESP8266
//  Serial.begin(kBaudRate, SERIAL_8N1);
//#endif  // ESP8266
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  // Perform a low level sanity checks that the compiler performs bit field
  // packing as we expect and Endianness is as we expect.
  assert(irutils::lowLevelSanityCheck() == 0);

  Serial.printf("\n" D_STR_IRRECVDUMP_STARTUP "\n", kRecvPin);
#if DECODE_HASH
  // Ignore messages with less than minimum on or off pulses.
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif  // DECODE_HASH
  irrecv.setTolerance(kTolerancePercentage);  // Override the default tolerance.
  irrecv.enableIRIn();  // Start the receiver

  // BEGIN HTTP SERVER

  rewrite1 = false;
  rewrite2 = false;
  rewrite3 = false;
  rewrite4 = false;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  // Infrared rewrite starts here
  server.on("/ir/rewrite", []() {
    // PUT -> Rewrite signal
    if (server.method() == 4){      
      if (server.hasArg("id")){
        if (server.arg("id") == "1"){
          if (server.hasArg("state")){
            if (server.arg("state") == "on") {
              // Allow resources
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, PUT");
              server.sendHeader("Access-Control-Allow-Headers", "*");
              server.send(200, "application/json", "{\"id\": \"1\", \"state\": \"on\"}");
              rewrite1 = true;
            }
          }
        }
        if (server.arg("id") == "2"){
          if (server.hasArg("state")){
            if (server.arg("state") == "on") {
              // Allow resources
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, PUT");
              server.sendHeader("Access-Control-Allow-Headers", "*");
              server.send(200, "application/json", "{\"id\": \"2\", \"state\": \"on\"}");
              rewrite2 = true;
            }
          }
        }
        if (server.arg("id") == "3"){
          if (server.hasArg("state")){
            if (server.arg("state") == "on") {
              // Allow resources
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, PUT");
              server.sendHeader("Access-Control-Allow-Headers", "*");
              server.send(200, "application/json", "{\"id\": \"3\", \"state\": \"on\"}");
              rewrite3 = true;
            }
          }
        }
        if (server.arg("id") == "4"){
          if (server.hasArg("state")){
            if (server.arg("state") == "on") {
              // Allow resources
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, PUT");
              server.sendHeader("Access-Control-Allow-Headers", "*");
              server.send(200, "application/json", "{\"id\": \"4\", \"state\": \"on\"}");
              rewrite4 = true;
            }
          }
        }
      }
    }
    
    if (server.method() == 7) {
      // Allow resources
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.sendHeader("Access-Control-Allow-Methods", "GET, PUT");
      server.sendHeader("Access-Control-Allow-Headers", "*");
      server.send(200, "text/json", "");
    }
  });




  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  
  // Infrared send starts here
  server.on("/ir/send", []() {
    // PUT -> Send Signal
    if (server.method() == 4){      
      if (server.hasArg("id")){
        if (server.arg("id") == "1"){
          if (server.hasArg("state")){
            if (server.arg("state") == "on") {
              // Allow resources
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, PUT");
              server.sendHeader("Access-Control-Allow-Headers", "*");
              server.send(200, "application/json", "{\"id\": \"1\", \"state\": \"on\"}");
              send1 = true;
            }
          }
        }
        if (server.arg("id") == "2"){
          if (server.hasArg("state")){
            if (server.arg("state") == "on") {
              // Allow resources
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, PUT");
              server.sendHeader("Access-Control-Allow-Headers", "*");
              server.send(200, "application/json", "{\"id\": \"2\", \"state\": \"on\"}");
              send2 = true;
            }
          }
        }
        if (server.arg("id") == "3"){
          if (server.hasArg("state")){
            if (server.arg("state") == "on") {
              // Allow resources
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, PUT");
              server.sendHeader("Access-Control-Allow-Headers", "*");
              server.send(200, "application/json", "{\"id\": \"3\", \"state\": \"on\"}");
              send3 = true;
            }
          }
        }
        if (server.arg("id") == "4"){
          if (server.hasArg("state")){
            if (server.arg("state") == "on") {
              // Allow resources
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, PUT");
              server.sendHeader("Access-Control-Allow-Headers", "*");
              server.send(200, "application/json", "{\"id\": \"4\", \"state\": \"on\"}");
              send4 = true;
            }
          }
        }
      }
    }
    if (server.method() == 7) {
      // Allow resources
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.sendHeader("Access-Control-Allow-Methods", "GET, PUT");
      server.sendHeader("Access-Control-Allow-Headers", "*");
      server.send(200, "text/json", "");
    }
  });



  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  // Handle not found
  server.onNotFound(handleNotFound);

  // Begin server
  server.begin();
  Serial.println("HTTP server started");

  // END HTTP SERVER
  
}

// The repeating section of the code
void loop() {

  // BEGIN HTTP SERVER

  server.handleClient();
  MDNS.update();

  // END HTTP SERVER
  
  // Check if the IR code has been received.
  if (irrecv.decode(&results) && (rewrite1)) {
    // Display a crude timestamp.
    uint32_t now = millis();
    Serial.printf(D_STR_TIMESTAMP " : %06u.%03u\n", now / 1000, now % 1000);
    // Check if we got an IR message that was to big for our capture buffer.
    if (results.overflow)
      Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
    // Display the library version the message was captured with.
    Serial.println(D_STR_LIBRARY "   : v" _IRREMOTEESP8266_VERSION_ "\n");
    // Display the tolerance percentage if it has been change from the default.
    if (kTolerancePercentage != kTolerance)
      Serial.printf(D_STR_TOLERANCE " : 1-> %d%%\n", kTolerancePercentage);
    // Display the basic output of what we found.
    Serial.print(" 2-> " + resultToHumanReadableBasic(&results));
    // Display any extra A/C info if we have it.
    String description = IRAcUtils::resultAcToString(&results);
    if (description.length()) Serial.println(D_STR_MESGDESC ": 3-> " + description);
    yield();  // Feed the WDT as the text output can take a while to print.
#if LEGACY_TIMING_INFO
    // Output legacy RAW timing info of the result.
    Serial.println(" 4-> ", resultToTimingInfo(&results));
    yield();  // Feed the WDT (again)
#endif  // LEGACY_TIMING_INFO
    // Output the results as source code
    Serial.println(" 5-> " + resultToSourceCode(&results));
    Serial.println();    // Blank line between entries
    yield();             // Feed the WDT (again)

    // REWRITE HERE

    if (rewrite1){
      rawdataLength = results.rawlen;
      Serial.println("Recorded signal of length ");
      for(int i=1; i < 100; i++){
         rawdata[i-1] = results.rawbuf[i] * RAWTICK * RAWTICK / 2;
         Serial.println("line " + String(i) + " " + String(rawdata[i-1]));    // RawData
      }
      Serial.println();    // Blank line between entries
      yield();
      rewrite1 = false;
    }
    // REWRITE END HERE
  }

  // SEND HERE
  if (send1){
    Serial.println("Replaying signal 1 ...");
    irsend.sendRaw(rawdata, rawdataLength, 38);  // Send a raw data capture at 38kHz.
    
    for(int i=1; i < 100; i++){
       Serial.println("line " + String(i) + " " + String(rawdata[i - 1]));    // RawData
    }

    send1 = false;
    delay(2000); 
  }
  // END SEND HERE
}
