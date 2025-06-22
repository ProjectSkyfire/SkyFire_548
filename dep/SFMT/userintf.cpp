/*************************** userintf.cpp **********************************
* Author:        Agner Fog
* Date created:  
* Last modified: 2008-11-21
* Project:       stocc.zip
* Source URL:    www.agner.org/random
*
* Description:
* This file specifies system-dependent input/output functions for test 
* programs. Must be modified according to whether the program has a console-
* mode or a graphical user interface.
*
* Further documentation:
* The file ran-instructions.pdf contains further documentation and 
* instructions.
*
* Copyright 1997-2008 by Agner Fog. 
* GNU General Public License http://www.gnu.org/licenses/gpl.html
*****************************************************************************/

#include <stdio.h>                     // define printf() function
#include <stdlib.h>                    // define exit() function

#if (defined (__BORLANDC__) || defined (_MSC_VER)) && ! defined(_WINDOWS_)
  #include <conio.h>                   // define getch() function
  #define _GETCH_DEFINED_
#endif


/***********************************************************************
                     End of program
***********************************************************************/

void EndOfProgram() {
  // This function takes care of whatever is necessary to do when the 
  // program is finished

  // It may be necessary to wait for the user to press a key
  // in order to prevent the output window from disappearing.
  // Remove the #ifdef and #endif lines to unconditionally wait for a key press;
  // Remove all three lines to not wait:
  #ifdef _GETCH_DEFINED_
  getch();                             // wait for user to press a key
  #endif

  // It may be necessary to end the program with a linefeed:
  #if defined (__unix__) || defined (_MSC_VER)
  printf("\n");                        // end program with a linefeed
  #endif
}


/***********************************************************************
                     Error message function
***********************************************************************/

void FatalError(const char * ErrorText) {
  // This function outputs an error message and aborts the program.

  // Important: There is no universally portable way of outputting an 
  // error message. You may have to modify this function to output
  // the error message in a way that is appropriate for your system.


  // Check if FatalAppExit exists (this macro is defined in winbase.h)
  #ifdef FatalAppExit  

  // in Windows, use FatalAppExit:
  FatalAppExit(0, ErrorText);

  #else

  // in console mode, print error message  
  printf ("\n%s\n", ErrorText);
  EndOfProgram();

  #endif

  // Terminate program with error code
  exit(1);
}
