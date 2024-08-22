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

#include "Simple.h"

const char html_header1[]    PROGMEM = "<!DOCTYPE html><html><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
              "<head><link rel=\"stylesheet\" type=\"text/css\" href=\"/styles.css\"></head>"
              "<body style=\"font-family: Arial\">";
const char html_title1[]     PROGMEM = "<H1 align=\"center\"> %s </H1><br><br>";
const char html_body1[]      PROGMEM = "<H3 align=\"center\"> Arg %d name is %s and value is %s </H3>";
const char html_body2[]      PROGMEM = "<H3 align=\"center\"> Request local IP is %s:%d </H3>"
                               "<H3 align=\"center\"> Request remote IP is %s:%d </H3>";
const char html_tail1[]      PROGMEM = "</body></html>";

using namespace lsc;

 void Simple::handleRoot(WebContext* c) {
  Serial.printf("handleRoot called...\n");
  char buffer[1000];
  int pos = 0;
  size_t len = sizeof(buffer);
  pos = formatBuffer_P(buffer,len,pos,html_header1);
  pos = formatBuffer_P(buffer,len,pos,html_title1,"Hello from the application \"Simple\"");
  pos = formatBuffer_P(buffer,len,pos,html_tail1);
  Serial.printf("   Sending %d bytes\n",strlen(buffer));
  c->send(200,"text/html",buffer);
  Serial.printf("...handleRoot done\n\n");
}

void Simple::handleDevice(WebContext* c) {
  Serial.printf("handleDevice called...\n");
  char buffer[1000];
  int pos = 0;
  size_t len = sizeof(buffer);
  pos = formatBuffer_P(buffer,len,pos,html_header1);
  pos = formatBuffer_P(buffer,len,pos,html_title1,"Arg Test");
  int argCount = c->argCount();
  for( int i=0; i<argCount; i++) {
    const String& argName = c->argName(i);
    const String& argVal  = c->arg(i);
    pos = formatBuffer_P(buffer,len,pos,html_body1,i,argName.c_str(),argVal.c_str());
  }
  pos = formatBuffer_P(buffer,len,pos,html_tail1);
  Serial.printf("   sending %d bytes:\n",strlen(buffer));
  c->send(200,"text/html",buffer);  
  Serial.printf("...handleDevice done\n");
}

void Simple::handleRequest(WebContext* c) {
  Serial.printf("handleRequest called...\n");
  char buffer[1000];
  int pos = 0;
  size_t len = sizeof(buffer);
  pos = formatBuffer_P(buffer,len,pos,html_header1);
  pos = formatBuffer_P(buffer,len,pos,html_title1,"Request Endpoint Info");
  pos = formatBuffer_P(buffer,len,pos,html_body2,c->client().localIP().toString().c_str(),c->getLocalPort(),c->client().remoteIP().toString().c_str(),c->client().remotePort());
  pos = formatBuffer_P(buffer,len,pos,html_tail1);
  len = strlen(buffer);
  Serial.printf("   sending %d bytes:\n",strlen(buffer));
  c->send(200,"text/html",buffer);
  Serial.printf("...handleRequest done\n\n");
}

void Simple::styles(WebContext* c) {
  c->send_P(200,TEXT_CSS,styles_css);
}
