# CommonUtil
Common utilities necessary for [ssdp](https://github.com/dltoth/ssdp/), [UPnPDevice](https://github.com/dltoth/UPnPDevice/), and [DeviceLib](https://github.com/dltoth/DeviceLib/). In particular:

&nbsp;

|File|Description|
|---|---|
|[WebContext](https://github.com/dltoth/CommonUtil/blob/main/src/WebContext.h)|Provides a Web Server abstraction for ESP8266 and ESP32|
|[CommonProgmem](https://github.com/dltoth/CommonUtil/blob/main/src/CommonProgmem.h)|Defines formatting functions useful for PROGMEM templates for HTML and various PROGMEM templates for formatting HTML, including the stylesheet used by libraries|

&nbsp;

For an example sketch using WebContext and associated formatting functions, see [Simple](https://github.com/dltoth/CommonUtil/blob/main/examples/Simple/Simple.ino), with class definition [Simple.h](https://github.com/dltoth/CommonUtil/blob/main/examples/Simple/Simple.h) and implementation [Simple.cpp](https://github.com/dltoth/CommonUtil/blob/main/examples/Simple/Simple.cpp).

In the sketch, note the following:

**WebContext Instantiation**

WebContext is declared in global scope above the setup() routine of the sketch, and passed as a pointer. A pointer to either ESP8266WebServer or WebServer for ESP32 is passed in to setup().

```
WebContext       ctx;
ctx.setup(svr,WiFi.localIP(),SERVER_PORT);
```
**Register HTTP Request Handlers**

HTTP request handlers are registered with an *on()* function

```
  ctx.on("/",[](WebContext* c){Simple::handleRoot(c);});
  ctx.on("/device",[](WebContext* c){Simple::handleDevice(c);});
  ctx.on("/request",[](WebContext* c){Simple::handleRequest(c);});
  ctx.on("/styles.css",[](WebContext* svr){Simple::styles(svr);});
```

Note that a WebContext* is passed by the on() function in its [definition](https://github.com/dltoth/CommonUtil/blob/main/src/WebContext.h). Also note the handler registered for styles.css.

Now, turning to the implementation file [Simple.cpp](https://github.com/dltoth/CommonUtil/blob/main/examples/Simple/Simple.cpp), notice the following:

**HTML Templates Defined in PROGMEM**

```
const char html_header1[]    PROGMEM = "<!DOCTYPE html><html><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
              "<head><link rel=\"stylesheet\" type=\"text/css\" href=\"/styles.css\"></head>"
              "<body style=\"font-family: Arial\">";
const char html_title1[]     PROGMEM = "<H1 align=\"center\"> %s </H1><br><br>";
const char html_body1[]      PROGMEM = "<H3 align=\"center\"> Arg %d name is %s and value is %s </H3>";
const char html_body2[]      PROGMEM = "<H3 align=\"center\"> Request local IP is %s:%d </H3>"
                               "<H3 align=\"center\"> Request remote IP is %s:%d </H3>";
const char html_tail1[]      PROGMEM = "</body></html>";
```

**Define the Handlers**

All of the handler functions have a common form:

```
  char buffer[1000];
  int pos = 0;
  size_t len = sizeof(buffer);
  pos = formatBuffer_P(buffer,len,pos,html_header1);
  pos = formatBuffer_P(buffer,len,pos,html_title1,"Hello from the application \"Simple\"");
  pos = formatBuffer_P(buffer,len,pos,html_tail1);
  c->send(200,"text/html",buffer);
```

Define a char buffer for HTML conent, then format the HTML. Each call to formatBuffer_P updates the write position *pos*.

Lastly note the handler for CSS Style.

```
void Simple::styles(WebContext* c) {
  c->send_P(200,TEXT_CSS,styles_css);
}
```
TEXT_CSS and styles_css are defined in [CommonProgmem.h](https://github.com/dltoth/CommonUtil/blob/main/src/CommonProgmem.h)
