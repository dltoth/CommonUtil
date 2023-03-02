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

#ifndef COMMON_PROGMEM_H
#define COMMON_PROGMEM_H
#include <Arduino.h>

#include "CommonDef.h"

/** Leelanau Software Company namespace 
*  
*/
namespace lsc {

/** Helper functions to fill buffer with HTML from templates.
 *  For example:
 *    char buffer[1500];
 *    int size = sizeof(buffer);
 *    int pos = formatHeader(buffer,size,"Page Title");
 *    pos = formatBuffer(buffer,size,pos,template1,a1,a2,...aN);
 *    pos = formatBuffer(buffer,size,pos,template2,b1,b2...bN);
 *    formatTail(buffer,size,pos);
 *  
 */
extern int  formatBuffer_P(char buffer[], int size, int pos, PGM_P format, ...);
extern int  formatBuffer(char buffer[], int size, int pos, const char* format, ...);
extern int  formatHeader(char buffer[], int size, const char* title);
extern int  formatTail(char buffer[], int size, int pos);

/**
 *  URL Encode a base64 character string, replacing '=' with "%3D" and '+' with "%20"
 *  The resulting buffer is '\0' terminated, truncating encoding if buffer size is exceeded.
 */
extern int  base64ToURL(char buffer[], int size, int pos, const char* b64Str);  

/**   Stack Memory checking
 *    
 */
extern const char*  _stackStart;
extern void initStackStart(const char* c);
extern int  stackUsed(const char* c);

/**
 *    Report stack memory used up to a specific point in code. chost char* c is a character declared just prior to the call and
 *    msg is a label for the report.
 */
extern void reportStackUse(const char* msg,const char* c);

/** Templates for filling char buffers
 * 
 */
const char error_html[]    PROGMEM = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><error>%s Not Found</error>";                                       
const char TEXT_HTML[]     PROGMEM = "text/html";
const char TEXT_CSS[]      PROGMEM = "text/css";
const char html_header[]   PROGMEM = "<!DOCTYPE html><html><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                                        "<head><link rel=\"stylesheet\" type=\"text/css\" href=\"/styles.css\"></head>"
                                        "<body style=\"font-family: Arial\">";
const char html_title[]    PROGMEM = "<H1 align=\"center\"> %s </H1><br>";
const char html_L2_title[] PROGMEM = "<H2 align=\"center\"> %s </H2>";
const char html_L3_title[] PROGMEM = "<H3 align=\"center\"> %s </H3>";
const char html_tail[]     PROGMEM = "</body></html>";
const char iframe_html[]   PROGMEM = "<div align=\"center\"><iframe src=\"%s\" height=\"%d\" width=\"%d\" style=\"border:none;\"></iframe></div>";
const char html_NotFound[] PROGMEM = "<!DOCTYPE html><html><body style=\"font-family: Calibri\"><h1 align=\"center\"> OOPS! %s Not Found!</h1></body></html>"; //URI
const char nearby_html[]   PROGMEM = "<a href=\"/%s/nearbyDevices\" class=\"scaled apButton\">Nearby Devices</a>";
const char app_button[]    PROGMEM = "<a href=\"%s\" class=\"scaled apButton\">%s</a>";
const char small_button[]  PROGMEM = "<a href=\"%s\" class=\"small apButton\">%s</a>";

/**
 *   Note that a config apButton is a scaled apButton with different colors
 */
const char config_button[] PROGMEM = "<a href=\"%s\" class=\"config apButton\">%s</a>";

const char styles_css[]    PROGMEM = ".apButton {"
                "background:linear-gradient(to bottom, #ededed 5%, #bab1ba 100%);"
                "background-color:#ededed;"
                "border-radius:12px;"
                "border:1px solid #d6bcd6;"
                "display:block;"
                "cursor:pointer;"
                "color:#3a8a9e;"
                "font-family:Arial;"
                "font-size: 1.2em;"  
                "padding: .5em;"
                "width: 100%;"
                "text-decoration:none;"
                "margin: 0px auto 3px auto;"                                                 
                "text-shadow:0px 1px 0px #e1e2ed;"
                "text-align: center;"
              "}"
              ".apButton:hover {"
                "background:linear-gradient(to bottom, #bab1ba 5%, #ededed 100%);"
                "background-color:#bab1ba;"
              "}"
              ".apButton:active {"
                "position:relative;"
                "top:1px;"
              "}" 
              "[class*=\"small\"]  {"
                "width: 20% ;"
                "display: inline-block ;"
              "}"                    
              "[class*=\"config\"]  {" 
                 "background:linear-gradient(to bottom, #ededed 5%, #9bb5cf 100%);"  
                 "background-color:#ededed;"
                 "width: 80% ;"                    
              "}" 
              ".config:hover {"
                  "background:linear-gradient(to bottom, #9bb5cf 5%, #ededed 100%);"
                  "background-color:#9bb5cf;" 
              "}"                    
              "@media only screen and (min-width: 768px) {"
                ".small  {width: 10%;}"                       
             "}"
              "[class*=\"medium\"]  {"
                "width: 50% ;"
                "display: inline-block ;"
              "}"                    
              "@media only screen and (min-width: 768px) {"
                ".medium {width: 25%;}"  
              "}"
              "[class*=\"scaled\"] {"
                "width: 80% ;"                    
             "}"
              "@media only screen and (min-width: 768px) {"
                ".scaled {width: 40%;}"
                ".config {width: 40%;}"                       
              "}"
              "label {" 
                "font-family: Arial;"                
                "font-size: 1.2em;"
                "display: inline;"
              "}"
              ".fmButton {"
                 "background:linear-gradient(to bottom, #ededed 5%, #bab1ba 100%);"
                 "background-color:#ededed;" 
                 "border-radius:5px;"
                 "border:1px solid #d6bcd6;"
                 "display: inline;"  
                 "cursor:pointer;"
                 "color:#3a8a9e;"
                 "font-family:Arial;"
                 "font-size: 1em;"  
                 "padding: .3em;"
                 "width: 6em;"
                 "text-decoration:none;"
                 "margin: auto;"
                 "text-shadow:0px 1px 0px #e1e2ed;"
                 "text-align: center;"
               "}"
               ".fmButton:hover {"
                 "background:linear-gradient(to bottom, #bab1ba 5%, #ededed 100%);"
                 "background-color:#bab1ba;"
               "}"
               ".fmButton:active {"
                 "position:relative;"
                 "top:1px;"
               "}"
               ".toggle {"
               "  cursor: pointer;"
               "  display: inline-block;"
               "}"
               ".toggle-switch {"
               "  display: inline-block;"
               "  background: #e5eefc;"
               "  border-radius: 16px;"
               "  border: 1px solid #7faaf1;"
               "  width: 36px;"
               "  height: 20px;"
               "  position: relative;"
               "  vertical-align: middle;"
               "  transition: background 0.25s;"
               "}"
               ".toggle-switch:before, .toggle-switch:after {"  
               "  content: \"\";"
               "}"
               ".toggle-switch:before {"
               "  display: block;"
               "  background: linear-gradient(to bottom, #fff 0%, #eee 100%);"
               "  border-radius: 50%;"
               "  box-shadow: 0 0 0 1px rgba(0, 0, 0, 0.25);"
               "  width: 15px;"
               "  height: 15px;"
               "  position: absolute;"
               "  top: 3px;"
               "  left: 3px;"
               "  transition: left 0.25s;"
               "}"
               ".toggle:hover .toggle-switch:before {"
               "  background: linear-gradient(to bottom, #fff 0%, #fff 100%);"
               "  box-shadow: 0 0 0 1px rgba(0, 0, 0, 0.5);"
               "}"
               ".toggle-checkbox:checked + .toggle-switch {"
               "  background: #7faaf1;"
               "}"
               ".toggle-checkbox:checked + .toggle-switch:before {"
               "  left: 18px;"
               "}"
               ".toggle-checkbox {"
               "  position: absolute;"
               "  visibility: hidden;"
               "}"
               ".toggle-label {"
               "  margin-left: 5px;"
               "  position: relative;"
               "  top: 2px;"
               "}";
               
const char loc_template[]  PROGMEM = "http://%d.%d.%d.%d:%d";

} // End of namespace lsc

#endif
