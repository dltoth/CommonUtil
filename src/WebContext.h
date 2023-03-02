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
 *  for a variety of Web Servers (See AsyncContext)
 */

#ifndef WEB_CONTEXT_H
#define WEB_CONTEXT_H

#include <Arduino.h>
#include <functional>
#include <IPAddress.h>

#ifdef ESP8266
#include <ESP8266WebServer.h>
#elif defined(ESP32)
#include <WebServer.h>
#endif

/** Leelanau Software Company namespace 
*  
*/
namespace lsc {

class WebContext;
typedef std::function<void(void)> ClientHandler;                                                          // Handle Client function
typedef std::function<String(void)> URIFunction;                                                          // Return number of arguments on Http Request
typedef std::function<void(WebContext*)> HandlerFunction;                                                 // Web Request Handler
typedef std::function<void(int statusCode, const char* contentType, const char*  content)> SendFunction;  // Send Http Response
typedef std::function<void(int statusCode, PGM_P contentType, PGM_P  content)> Send_PFunction;            // Send_P variant for Http Response
typedef std::function<void(const char* path, HandlerFunction)> OnFunction;                                // Register a RequestHandler
typedef std::function<int(void)> ArgCountFunction;                                                        // Return number of arguments on Http Request
typedef std::function<const String&(int)> ArgFunction;                                                    // Return request argument i (name or value)
typedef std::function<int(void)> PortFunction;                                                            // Return Server port
typedef std::function<IPAddress(void)> IPAddressFunction;                                                 // Return IPAddress server is listening on

class WebContext {

  public:
  WebContext() {}

  void      setSendFunction( SendFunction f)                                   {if(f != NULL) _sendFunction = f;}
  void      setClientHandler(ClientHandler f)                                  {if(f != NULL) _handleClient = f;}
  void      setURIFunction( URIFunction f)                                     {if(f != NULL) _uriFunction = f;}
  void      setSend_PFunction( Send_PFunction f)                               {if(f != NULL) _send_PFunction = f;}
  void      setOnFunction( OnFunction f)                                       {if(f != NULL) _onFunction = f;}
  void      setLocalPortFunction(PortFunction f)                               {if(f != NULL) _localPortFunction = f;}
  void      setLocalIPAddressFunction( IPAddressFunction f)                    {if(f != NULL) _localIPAddressFunction = f;}
  void      setRemotePortFunction(PortFunction f)                              {if(f != NULL) _remotePortFunction = f;}
  void      setRemoteIPAddressFunction( IPAddressFunction f)                   {if(f != NULL) _remoteIPAddressFunction = f;}
  void      setArgCountFunction( ArgCountFunction f)                           {if(f != NULL) _argCountFunction = f;}
  void      setArgFunction( ArgFunction f)                                     {if(f != NULL) _argFunction = f;}
  void      setArgNameFunction( ArgFunction f)                                 {if(f != NULL) _argNameFunction = f;}

  void      send(int statusCode, const char* const contentType, char* content) {_sendFunction(statusCode, contentType, content);}
  void      send_P(int statusCode, PGM_P contentType, PGM_P content)           {_send_PFunction(statusCode, contentType, content);}
  void      on(const char* path, HandlerFunction f)                            {_onFunction(path,f);}
  int       argCount()                                                         {return _argCountFunction();}
  int       getLocalPort()                                                     {return _localPortFunction();}
  IPAddress getLocalIPAddress()                                                {return _localIPAddressFunction();}
  int       getRemotePort()                                                    {return _remotePortFunction();}
  IPAddress getRemoteIPAddress()                                               {return _remoteIPAddressFunction();}
  const     String& arg(int i)                                                 {return _argFunction(i);}
  const     String& argName(int i)                                             {return _argNameFunction(i);}
  void      handleClient()                                                     {_handleClient();}
  String    getURI()                                                           {return _uriFunction();}

#ifdef ESP8266

/**
 *  Since there's no way to pull the local IP and port off of the Web Server, and even worse, it doesn't appear to be
 *  reliably initialized to [0,0,0,0] either. We have to use the initialized values for localIP and port.
 */
  void setup(ESP8266WebServer* svr, IPAddress localAddr, int port) {
     setClientHandler([svr](){svr->handleClient();});
     setSendFunction([svr](int status, const char* contentType, const char* content) {svr->send(status,contentType,content);});
     setSend_PFunction([svr](int status, PGM_P contentType, PGM_P content) {svr->send_P(status,contentType,content);});
     setOnFunction([this,svr](const char* path, HandlerFunction f) {svr->on(path,[this,f](){f(this);});});
     setArgCountFunction([svr]()->int{return svr->args();});
     setLocalPortFunction([port]()->int{return port;});
     setLocalIPAddressFunction([localAddr]()->IPAddress {return localAddr;});
     setRemotePortFunction([svr]()->int {return svr->client().remotePort();});
     setRemoteIPAddressFunction([svr]()->IPAddress{return svr->client().remoteIP();});
     setArgFunction([svr](int i)->const String&{return svr->arg(i);});
     setArgNameFunction([svr](int i)->const String&{return svr->argName(i);});
  }

#elif defined(ESP32)

/**
 *  Since there's no way to pull the local IP and port off of the Web Server, we use the client when available. Otherwise,
 *  we use the initialized value.
 */
  void setup(WebServer* svr, IPAddress localAddr, int port) {
     setClientHandler([svr](){svr->handleClient();});
     setSendFunction([svr](int status, const char* contentType, const char* content) {svr->send(status,contentType,content);});
     setSend_PFunction([svr](int status, PGM_P contentType, PGM_P content) {svr->send_P(status,contentType,content);});
     setOnFunction([svr,localAddr,port](const char* path, HandlerFunction f) {
         if(svr!=NULL) {svr->on(path,[svr,localAddr,port,f]() {
               WebContext ctx;
               ctx.setup(svr,localAddr,port);
               f(&ctx);
            });}
         });
     setArgCountFunction([svr]()->int{return svr->args();});
     setLocalIPAddressFunction([localAddr]()->IPAddress {return localAddr;});
     setLocalPortFunction([port]()->int {return port;});
     setRemotePortFunction([svr]()->int{return svr->client().remotePort();});
     setRemoteIPAddressFunction([svr]()->IPAddress{return svr->client().remoteIP();});
     setArgFunction([svr,this](int i)->const String&{this->argVal = svr->arg(i);return this->argVal;});
     setArgNameFunction([svr,this](int i)->const String&{this->argNam = svr->argName(i);return this->argNam;});
  }
  
#endif
  
/**
 *   Default handler functions do nothing
 */
  private:
  ClientHandler        _handleClient            = [](){};
  URIFunction          _uriFunction             = []()->String{return _empty;};
  SendFunction         _sendFunction            = [](int,const char*,const char*)->void{};
  Send_PFunction       _send_PFunction          = [](int,PGM_P,PGM_P)->void{};
  OnFunction           _onFunction              = [](const char*,HandlerFunction)->void{};
  ArgCountFunction     _argCountFunction        = []()->int{return 0;};
  ArgFunction          _argFunction             = [this](int)->const String&{return this->_empty;};
  ArgFunction          _argNameFunction         = [this](int)->const String&{return this->_empty;};
  ArgCountFunction     _localPortFunction       = []()->int{return 0;};
  IPAddressFunction    _localIPAddressFunction  = []()->IPAddress{return IPAddress(0,0,0,0);};
  ArgCountFunction     _remotePortFunction      = []()->int{return 0;};
  IPAddressFunction    _remoteIPAddressFunction = []()->IPAddress{return IPAddress(0,0,0,0);};
  
  protected:
  static const String    _empty;
  String                 argVal;
  String                 argNam;

};

} // End of namespace lsc


#endif
 
