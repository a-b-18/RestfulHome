/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "SSID"
#define STAPSK  "password"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);

const int led1 = 14;
boolean led1On;
const int led2 = 12;
boolean led2On;
const int led3 = 13;
boolean led3On;
const int led4 = 15;
boolean led4On;

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

void setup(void) {
  pinMode(led1, OUTPUT);
  digitalWrite(led1, 0);
  led1On = false;

  pinMode(led2, OUTPUT);
  digitalWrite(led2, 0);
  led2On = false;

  pinMode(led3, OUTPUT);
  digitalWrite(led3, 0);
  led3On = false;

  pinMode(led4, OUTPUT);
  digitalWrite(led4, 0);
  led4On = false;
  
  Serial.begin(115200);
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

  // LED interface starts here
  server.on("/led", []() {
    // GET -> Read LED state
    if (server.method() == HTTP_GET){
      if (server.argName(0) == "id"){
        if (server.arg(0) == "1"){
          if (led1On){
            server.send(200, "text/json", "{\n\t'id': '1',\n\t'state': 'on'\n}");
          }
          if (!led1On){
            server.send(200, "text/json", "{\n\t'id': '1',\n\t'state': 'off'\n}");
          }
        }
        if (server.arg(0) == "2"){
          if (led2On){
            server.send(200, "text/json", "{\n\t'id': '2',\n\t'state': 'on'\n}");
          }
          if (!led2On){
            server.send(200, "text/json", "{\n\t'id': '2',\n\t'state': 'off'\n}");
          }
        }
        if (server.arg(0) == "3"){
          if (led3On){
            server.send(200, "text/json", "{\n\t'id': '3',\n\t'state': 'on'\n}");
          }
          if (!led3On){
            server.send(200, "text/json", "{\n\t'id': '3',\n\t'state': 'off'\n}");
          }
        }
        if (server.arg(0) == "4"){
          if (led4On){
            server.send(200, "text/json", "{\n\t'id': '4',\n\t'state': 'on'\n}");
          }
          if (!led4On){
            server.send(200, "text/json", "{\n\t'id': '4',\n\t'state': 'off'\n}");
          }
        }
      }
    }
    // PUT -> Write LED state
    if (server.method() == 4){
      if (server.argName(0) == "id"){
        if (server.arg(0) == "1"){
          if (server.argName(1) == "state"){
            if (server.arg(1) == "on") {
              server.send(200, "text/json", "{\n\t'id': '1',\n\t'state': 'on'\n}");
              digitalWrite(led1, 1);
              led1On = true;
            } else if (server.arg(1) == "off") {
              server.send(200, "text/json", "{\n\t'id': '1',\n\t'state': 'off'\n}");
              digitalWrite(led1, 0);
              led1On = false;
            } 
          }
        }
        if (server.arg(0) == "2"){
          if (server.argName(1) == "state"){
            if (server.arg(1) == "on") {
              server.send(200, "text/json", "{\n\t'id': '2',\n\t'state': 'on'\n}");
              digitalWrite(led2, 1);
              led2On = true;
            } else if (server.arg(1) == "off") {
              server.send(200, "text/json", "{\n\t'id': '2',\n\t'state': 'off'\n}");
              digitalWrite(led2, 0);
              led2On = false;
            }
          }
        }
        if (server.arg(0) == "3"){
          if (server.argName(1) == "state"){
            if (server.arg(1) == "on") {
              server.send(200, "text/json", "{\n\t'id': '3',\n\t'state': 'on'\n}");
              digitalWrite(led3, 1);
              led3On = true;
            } else if (server.arg(1) == "off") {
              server.send(200, "text/json", "{\n\t'id': '3',\n\t'state': 'off'\n}");
              digitalWrite(led3, 0);
              led3On = false;
            }
          }
        }
        if (server.arg(0) == "4"){
          if (server.argName(1) == "state"){
            if (server.arg(1) == "on") {
              server.send(200, "text/json", "{\n\t'id': '4',\n\t'state': 'on'\n}");
              digitalWrite(led4, 1);
              led4On = true;
            } else if (server.arg(1) == "off") {
              server.send(200, "text/json", "{\n\t'id': '4',\n\t'state': 'off'\n}");
              digitalWrite(led4, 0);
              led4On = false;
            }
          }
        }
      }
    }
  });
  
  // Handle not found
  server.onNotFound(handleNotFound);

  // Begin server
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
