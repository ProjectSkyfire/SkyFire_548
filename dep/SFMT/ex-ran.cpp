/*************************** ex-ran.cpp **********************************
* Author:        Agner Fog
* Date created:  2007-09-23
* Last modified: 2008-11-28
* Project:       randomc.zip
* Source URL:    www.agner.org/random
*
* Description:
* Example showing how to use random number generators from class library.    *
*                                                                            *
* Instructions:                                                              *
* Compile for console mode, and run.                                         *
*                                                                            *
* This example uses the "Mersenne twister" random number generator.          *
* To use another random number generator, replace the class name
* CRandomMersenne with another random number generator class and 
* include the necessary source file or library for that class.
*                                                                            *
* Further documentation:
* The file ran-instructions.pdf contains further documentation and 
* instructions.
*
* Copyright 2007-2008 by Agner Fog. 
* GNU General Public License http://www.gnu.org/licenses/gpl.html
*****************************************************************************/

#include <time.h>                      // define time()
#include "randomc.h"                   // define classes for random number generators

#ifndef MULTIFILE_PROJECT
// If compiled as a single file then include these cpp files, 
// If compiled as a project then compile and link in these cpp files.
   // Include code for the chosen random number generator:
   #include "mersenne.cpp"
   // define system specific user interface:
   #include "userintf.cpp"
#endif


int main() {
   int seed = (int)time(0);            // random seed

   // choose one of the random number generators:
   CRandomMersenne RanGen(seed);       // make instance of random number generator

   int i;                              // loop counter
   int ir;                             // random integer number
   double fr;                          // random floating point number


   // make random integers in interval from 0 to 99, inclusive:
   printf("\n\nRandom integers in interval from 0 to 99:\n");
   for (i = 0; i < 40; i++) {
      ir = RanGen.IRandom(0,99);
      printf ("%6i  ", ir);
   }

   // make random floating point numbers in interval from 0 to 1:
   printf("\n\nRandom floating point numbers in interval from 0 to 1:\n");
   for (i = 0; i < 32; i++) {
      fr = RanGen.Random();
      printf ("%8.6f  ", fr);
   }

   // make random bits
   printf("\n\nRandom bits (hexadecimal):\n");
   for (i = 0; i < 32; i++) {
      ir = RanGen.BRandom();
      printf ("%08lX  ", ir);
   }

   EndOfProgram();                     // system-specific exit code
   return 0;
}

