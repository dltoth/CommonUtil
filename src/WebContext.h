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

/** Class to Wrap minimal Web Server functionality necessary to implement 
 *  UPnPDevices. WebContext will wrap ESP8266WebServer and (ESP32) WebServer. 
 *  Any other web server implementation should implement a subclass.
 *  
 *  This is not all-inclusive but adaquate for UPnPDevices and should be implementable
 *  for a variety of additional Web Servers 
 */

#ifndef WEB_CONTEXT_H
#define WEB_CONTEXT_H

#include <Arduino.h>
#include <functional>
#include <IPAddress.h>

#ifdef ESP8266
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WebServer.h>
#include <WiFi.h>
#endif

/** Leelanau Software Company namespace 
*  
*/
namespace lsc {

class WebContext;
typedef std::function<void(void)> ClientHandler;                                                          // WebServer::handleClient() function
typedef std::function<const String&(void)> URIFunction;                                                   // WebServer::uri() function to return current URI on Http Request
typedef std::function<void(WebContext*)> HandlerFunction;                                                 // Web Request Handler, set on WebServer::on()
typedef std::function<void(int statusCode, const char* contentType, const char*  content)> SendFunction;  // WebServer::send to send Http Response
typedef std::function<void(int statusCode, PGM_P contentType, PGM_P  content)> Send_PFunction;            // WebServer::send_P variant for Http Response
typedef std::function<void(const char* path, HandlerFunction)> OnFunction;                                // WebServer::on() to Register a HandlerFunction
typedef std::function<void(HandlerFunction)> OnNotFoundFunction;                                          // WebServer::onNotFound() to register a ClientHandler when URI is not found
typedef std::function<void(RequestHandler*)> AddHandlerFunction;                                          // WebServer::addHandler() to add HTTP RequestHandler
typedef std::function<int(void)> ArgCountFunction;                                                        // WebServer::argCount() returning number of arguments on current Http Request
typedef std::function<const String&(int)> ArgFunction;                                                    // WebServer::arg(int i) function to return request argument i (name or value)
typedef std::function<WiFiClient(void)> WiFiClientFunction;                                               // WebServer::client() to return WiFiClient
typedef std::function<void(void)> CloseFunction;                                                          // WebServer::close() function

class WebContext {

  public:
  WebContext() {}

  void       setSendFunction( SendFunction f)                                         {if(f != NULL) _sendFunction = f;}
  void       setClientHandler(ClientHandler f)                                        {if(f != NULL) _handleClient = f;}
  void       setURIFunction( URIFunction f)                                           {if(f != NULL) _uriFunction = f;}
  void       setSend_PFunction( Send_PFunction f)                                     {if(f != NULL) _send_PFunction = f;}
  void       setOnFunction( OnFunction f)                                             {if(f != NULL) _onFunction = f;}
  void       setOnNotFoundFunction( OnNotFoundFunction f)                             {if(f != NULL) _onNotFoundFunction = f;}
  void       setAddHandlerFunction(AddHandlerFunction f)                              {if(f != NULL) _addHandlerFunction = f;}
  void       setArgCountFunction( ArgCountFunction f)                                 {if(f != NULL) _argCountFunction = f;}
  void       setArgFunction( ArgFunction f)                                           {if(f != NULL) _argFunction = f;}
  void       setArgNameFunction( ArgFunction f)                                       {if(f != NULL) _argNameFunction = f;}
  void       setWiFiClientFunction(WiFiClientFunction f)                              {if(f != NULL) _wifiClientFunction = f;}
  void       setCloseFunction(CloseFunction f)                                        {if(f != NULL) _closeFunction = f;}

  void       send(int statusCode, const char* const contentType, const char* content) {_sendFunction(statusCode, contentType, content);}
  void       send_P(int statusCode, PGM_P contentType, PGM_P content)                 {_send_PFunction(statusCode, contentType, content);}
  void       on(const char* path, HandlerFunction f)                                  {_onFunction(path,f);}
  void       onNotFound(HandlerFunction f)                                            {_onNotFoundFunction(f);}
  void       addHandler(RequestHandler* h)                                            {_addHandlerFunction(h);}
  int        argCount()                                                               {return _argCountFunction();}
  int        getLocalPort()                                                           {return _port;}
  const      String& arg(int i)                                                       {return _argFunction(i);}
  const      String& argName(int i)                                                   {return _argNameFunction(i);}
  void       handleClient()                                                           {_handleClient();}
  String     uri()                                                                    {return _uriFunction();}
  WiFiClient client()                                                                 {return _wifiClientFunction();}
  void       close()                                                                  {_closeFunction();}

#ifdef ESP8266

  void begin(int port=80) {
     _server.begin(port);
     _port = port;
     setClientHandler([this](){_server.handleClient();});
     setSendFunction([this](int status, const char* contentType, const char* content) {_server.send(status,contentType,content);});
     setSend_PFunction([this](int status, PGM_P contentType, PGM_P content) {_server.send_P(status,contentType,content);});
     setOnFunction([this](const char* path, HandlerFunction f) {_server.on(path,[this,f](){f(this);});});
     setOnNotFoundFunction([this](HandlerFunction f) {_server.onNotFound([this,f](){f(this);});});
     setAddHandlerFunction([this](RequestHandler* h) {_server.addHandler(h);});
     setArgCountFunction([this]()->int{return _server.args();});
     setArgFunction([this](int i)->const String&{return _server.arg(i);});
     setArgNameFunction([this](int i)->const String&{return _server.argName(i);});
     setURIFunction([this]()->const String&{_uri = _server.uri();return _uri;});
     setWiFiClientFunction([this]()->WiFiClient{return _server.client();});
     setCloseFunction([this](){_server.close();});
  }

#elif defined(ESP32)

  void begin(int port=80) {
     _server.begin(port);
     _port = port;
     setClientHandler([this](){_server.handleClient();});
     setSendFunction([this](int status, const char* contentType, const char* content) {_server.send(status,contentType,content);});
     setSend_PFunction([this](int status, PGM_P contentType, PGM_P content) {_server.send_P(status,contentType,content);});
     setOnFunction([this](const char* path, HandlerFunction f) {_server.on(path,[this,f](){f(this);});});
     setOnNotFoundFunction([this](HandlerFunction f) {_server.onNotFound([this,f](){f(this);});});
     setAddHandlerFunction([this](RequestHandler* h) {_server.addHandler(h);});
     setArgCountFunction([this]()->int{return _server.args();});
     setArgFunction([this](int i)->const String&{this->argVal = _server.arg(i);return this->argVal;});
     setArgNameFunction([this](int i)->const String&{this->argNam = _server.argName(i);return this->argNam;});
     setURIFunction([this]()->const String&{_uri = _server.uri();return _uri;});
     setWiFiClientFunction([this]()->WiFiClient{return _server.client();});
     setCloseFunction([this](){_server.close();});
  }
  
#endif
  
/**
 *   Default handler functions do nothing
 */
  private:
  ClientHandler             _handleClient               = [](){};
  URIFunction               _uriFunction                = []()->String{return _empty;};
  SendFunction              _sendFunction               = [](int,const char*,const char*)->void{};
  Send_PFunction            _send_PFunction             = [](int,PGM_P,PGM_P)->void{};
  OnFunction                _onFunction                 = [](const char*,HandlerFunction)->void{};
  OnNotFoundFunction        _onNotFoundFunction         = [](HandlerFunction)->void{};
  AddHandlerFunction        _addHandlerFunction         = [](RequestHandler*)->void{};
  ArgCountFunction          _argCountFunction           = []()->int{return 0;};
  ArgFunction               _argFunction                = [this](int)->const String&{return this->_empty;};
  ArgFunction               _argNameFunction            = [this](int)->const String&{return this->_empty;};
  WiFiClientFunction        _wifiClientFunction         = [this]()->WiFiClient{return _client;};
  CloseFunction             _closeFunction              = [](){};
  
  protected:
  static const String    _empty;
  String                 argVal;
  String                 argNam;
  String                _uri;
  WiFiClient            _client;
  int                   _port = 0;

#ifdef ESP8266
  ESP8266WebServer       _server;
#elif defined(ESP32)
  WebServer              _server;
#endif
  
};

} // End of namespace lsc


#endif
 
