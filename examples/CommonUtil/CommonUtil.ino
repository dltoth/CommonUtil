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

#include "CommonUtil.h"

#define AP_SSID "MySSID"
#define AP_PSK  "MyPSK"
#define SOFT_AP_SSID "SleepingBear"
#define SOFT_AP_PSK  "BigLakeMI"
#define PORT         80

using namespace lsc;

const char html_template[]   PROGMEM = "<!DOCTYPE html><html><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                                       "<head><link rel=\"stylesheet\" type=\"text/css\" href=\"/styles.css\"></head>"
                                       "<body style=\"font-family: Arial\">"
                                         "<H3 align=\"center\"> Hello World </H3><br><br>"
                                         "<div align=\"center\">"
                                            "<table>"
                                                 "<tr><td><b>Client local IP Address:</b></td><td>&ensp;%s:%d</td></tr>"
                                                 "<tr><td><b>Client remote IP Address:</b></td><td>&ensp;%s:%d</td></tr>"
                                              "</table><br>"
                                         "</div>"
                                      "</body></html>";

#ifdef ESP8266
#define           BOARD "ESP8266"
#elif defined(ESP32)
#define          BOARD "ESP32"
#endif

WebContext  server;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println();
  Serial.printf("Starting WebContext Test for Board %s\n",BOARD);

  WiFi.mode(WIFI_AP_STA);                       // Set WiFi to STA and AP
  WiFi.softAP(SOFT_AP_SSID,SOFT_AP_PSK);        // Set up SoftAP
  WiFi.begin(AP_SSID,AP_PSK);                   // Start WiFi
  Serial.printf("SoftAP started on %s\n",WiFi.softAPIP().toString().c_str());

  Serial.printf("Connecting to Access Point %s\n",AP_SSID);
  while(WiFi.status() != WL_CONNECTED) {Serial.print(".");delay(500);}
  Serial.printf("\n\nWiFi Connected to %s with IP address: %s\n",WiFi.SSID().c_str(),WiFi.localIP().toString().c_str());

  server.begin(80);
  WebContext* svr = &server;
  server.on("/",[](WebContext*){handleRequest(&server);});
  server.on("/hello",[svr](WebContext*){handleRequest(svr);});
}

void loop() {
  server.handleClient();
}

void handleRequest(WebContext* svr) {
  IPAddress  localIP          = svr->client().localIP();
  int        localPort        = svr->getLocalPort();
  IPAddress  remoteIP         = svr->client().remoteIP();
  int        remotePort       = svr->client().remotePort();
  Serial.printf("HTTP Request: Local IP  = %s:%d \n",localIP.toString().c_str(),localPort);
  Serial.printf("              Remote IP = %s:%d\n",remoteIP.toString().c_str(),remotePort);

  char htmlBuffer[1000];
  size_t buffLen = sizeof(htmlBuffer);

  snprintf_P(htmlBuffer,buffLen,html_template,localIP.toString().c_str(),localPort,remoteIP.toString().c_str(),remotePort);
  svr->send(200,"text/html",htmlBuffer); 
}

