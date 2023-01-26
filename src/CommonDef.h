
/**
 * 
 */

#ifndef COMMONDEF_H
#define COMMONDEF_H

/** Leelanau Software Company namespace 
*  
*/
namespace lsc {

#ifndef EMPTY_STRING
#define EMPTY_STRING {""}
#endif

typedef enum LoggingLevel {
  NONE,
  WARNING,
  INFO,
  FINE,
  FINEST
} LoggingLevel;

  typedef enum ControlMode {
    AUTOMATIC,
    MANUAL
  } ControlMode;

  typedef enum ControlState {
    ON,
    OFF
  } ControlState;

/**
 *   WeMOS D1 to GPIO Pinout
 */
#define WEMOS_D0    16
#define WEMOS_D1    5
#define WEMOS_D2    4
#define WEMOS_D3    0
#define WEMOS_D4    2
#define WEMOS_D5    14
#define WEMOS_D6    12
#define WEMOS_D7    13
#define WEMOS_D8    15
#define WEMOS_D9    3
#define WEMOS_D10   1

} // End of namespace lsc

#endif
