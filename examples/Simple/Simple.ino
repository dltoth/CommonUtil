/**
 * 
 *  CommonUtil Library
 *  Copyright (C) 2023  Daniel L Toth
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published 
 *  by the Free Software Foundation, either version 3 of the License, or any 
 *  later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *  
 *  The author can be contacted at dan@leelanausoftware.com  
 *
 */

/**
 *   Simple example demonstrating the use of WebContext and CommonProgmem
 *   Input your ssid and psk on lines 10 and 11 of this application, then start the application with a Serial Monitor active and grab the
 *   IP address and port of the active Web Server
 *   Point a browser at:
 *   1. address:port/ to receive a welcome message
 *   2. address:port/device&arg=...&arg=... to list input arguments of the request
 *   3. address:port/request to receive ip address of local and remote ends of the client request
 */

#define AP_SSID "An SSID"
#define AP_PSK  "A_PSK"

#define SERVER_PORT 80

#include "WebContext.h"
#include "Simple.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
ESP8266WebServer  server(SERVER_PORT);
ESP8266WebServer* svr = &server;
#define           BOARD "ESP8266"
#elif defined(ESP32)
#include <WiFi.h>
WebServer        server(SERVER_PORT);
WebServer*       svr = &server;
#define          BOARD "ESP32"
#endif

WebContext       ctx;
WebContext*      c   = &ctx;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println();
  Serial.printf("\nStarting WebContext Test for board %s\n",BOARD);
  
  WiFi.begin(AP_SSID,AP_PSK);
  Serial.printf("Connecting to Access Point %s\n",AP_SSID);
  while(WiFi.status() != WL_CONNECTED) {Serial.print(".");delay(500);}

  Serial.printf("\nWiFi Connected to %s with IP address: %s\n",WiFi.SSID().c_str(),WiFi.localIP().toString().c_str());

  server.begin();
  ctx.setup(svr,WiFi.localIP(),SERVER_PORT);
  Serial.printf("Web Server started on %s:%d/\n",ctx.getLocalIPAddress().toString().c_str(),ctx.getLocalPort());
 
  ctx.on("/",[](WebContext* context){Simple::handleRoot(context);});
  ctx.on("/device",[](WebContext* context){Simple::handleDevice(context);});
  ctx.on("/request",[](WebContext* context){Simple::handleRequest(context);});

}

void loop() {
     server.handleClient();
}
