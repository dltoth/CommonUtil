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

#include "CommonProgmem.h"

/** Leelanau Software Company namespace 
*  
*/
namespace lsc {

URNToken URNTokenIterator::first() {
  URNToken result;
  if( _urn != NULL ) {
     _next = _urn;
     if( *_next == _delim) _next++;
     _current  = _next;
    int  len  = 0;
     bool done = false;
     while( (*_next != '\0') && !done) {
        if( *_next != _delim ) {_next++;len++;}
        else {_next++;done=true;}
     }
     result.initialize(_current, len);
  }
  return result;
}

URNToken URNTokenIterator::next() {
  URNToken result;
  if( _next != NULL ) {
     bool  done = false;
     _current   = _next;
     int len    = 0;
     while( (*_next != '\0') && !done ) {
        if( *_next != _delim) {_next++;len++;}
        else {_next++;done=true;}
     }
     result.initialize(_current, len);
     if( *_next == '\0') {_next = NULL;_current = NULL;}
  }
  return result;
}

URNToken URNTokenIterator::getToken(unsigned int index) {
   URNToken result;
   int tknNum = 0;
   int limit = index;
   URNToken tkn = first();
   while(hasNext() && (tknNum < limit)) {tknNum++;tkn = next();}
   if( tknNum == limit) result = tkn;
   return result;
}

/**
 *   Helper function for formated print into char buffer.
 *   Starts formating at character position pos based on a PGM_P format string.
 *   Returns string length of the updated char buffer (thus an updated pos). 
 */
int formatBuffer_P(char buffer[], int size, int pos, PGM_P format, ...) {
  int result = size;
  if( (pos >= 0) && (pos < size-1) ) {
     va_list args;
     va_start (args, format);
     vsnprintf_P(buffer+pos, size-pos, format, args);
     va_end (args);
     result = strlen(buffer);
  }
  return result;
}

/**
 *   Helper function for formated print into char buffer.
 *   Starts formating at character position pos based on a const char* format string.
 *   Returns string length of the updated char buffer (thus an updated pos). 
 */
int formatBuffer(char buffer[], int size, int pos, const char* format, ...) {
  int result = size;
  if( (pos >=0 ) && (pos < size-1) ) {
     va_list args;
     va_start (args, format);
     vsnprintf(buffer+pos, size-pos, format, args);
     va_end (args);
     result = strlen(buffer);
  }
  return result;
}

/** Helper function to insert HTML start tags and title.
 *  HTML start tags are appended at position 0 of the input buffer and if title has non-zero length, title is appended after.
 *  Returns current position of the end of buffer.
 */
int formatHeader(char buffer[], int size, const char* title) {
  int result = formatBuffer_P(buffer,size, 0, html_header);
  if( title != NULL ) {
    if( strlen(title) > 0 ) result = formatBuffer_P(buffer,size,result,html_title,title);
  }
  return result;
}

int formatTail( char buffer[], int size, int pos ) {return formatBuffer_P(buffer,size, pos, html_tail);}

int  base64ToURL(char buffer[], int size, int pos, const char* b64Str) {
  int result = size;
  int strSize = strlen(b64Str);
  int j = pos;
  if( (pos >=0 ) && (pos < size-1) ) {
    for(int i=0; (j<size)&&(i<strSize); i++) {
       if(b64Str[i] == '=')  {buffer[j++]='%';buffer[j++]='3';buffer[j++]='D';}     
       else if(b64Str[i] == '+')  {buffer[j++]='%';buffer[j++]='2';buffer[j++]='B';}
       else buffer[j++] = b64Str[i];
    }
    if( j<size ) {
      buffer[j] = '\0';
      result = j;
    }
    else {
      buffer[size-1] = '\0';
      result = size-1;
    }
  }
  return result;   
}

const char*  _stackStart = NULL;
void  initStackStart(const char* c)                 {_stackStart = c;}
int   stackUsed(const char* c)                      {return _stackStart - c;}
void  reportStackUse(const char* msg,const char* c) {Serial.printf("%s Stack use %d Bytes\n",msg,stackUsed(c));}

} // End of namespace lsc
