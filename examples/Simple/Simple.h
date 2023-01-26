/**
 * 
 */

#ifndef SIMPLE_H
#define SIMPLE_H

#include <WebContext.h>
#include <CommonProgmem.h>

class Simple {

  public:

  static void handleRoot(WebContext* c);
  static void handleDevice(WebContext* c);
  static void handleRequest(WebContext* c);
  
};

#endif
