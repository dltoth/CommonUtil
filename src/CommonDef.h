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
