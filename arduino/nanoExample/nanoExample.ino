// You can download Arduino code and Ethernet library - link below the video.

// UIP is a proper library for Arduino Nano Ethernet shield
// NOTE: UIPEthernet library is not needed if you are using Arduino UNO/Duemilanove/Mega/etc.
// UIPEthernet library is used for Arduino Nano Ethernet Shield

#include <UIPEthernet.h> // Used for Ethernet

// **** ETHERNET SETTING ****
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED  };    
boolean lightActive = true;      
int pinLED = 2;                        
IPAddress ip(192, 168, 1, 111);                        
EthernetServer server(80);

void setup() {
  Serial.begin(9600);
  pinMode(pinLED, OUTPUT);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();

  Serial.print("IP Address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();

  if (client) 
  {  
    Serial.println("-> New Connection");

    // an http request ends with a blank line
    boolean currentLineIsBlank = true;

    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) 
        {
          // insert response to GET request here
          if (!lightActive) {
            digitalWrite(pinLED, HIGH);
            lightActive = true;
            client.println("<html><title>Hello on!</title><body><h3>Hello on!</h3></body>");
          } else {
            digitalWrite(pinLED, LOW);
            lightActive = false;
            client.println("<html><title>Hello off!</title><body><h3>Hello off!</h3></body>");
          }          
          break;
        }

        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') 
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      } 
    }

    // give the web browser time to receive the data
    delay(10);
    
    // close the connection:
    client.stop();
    Serial.println("   Disconnected\n");

  }
}
