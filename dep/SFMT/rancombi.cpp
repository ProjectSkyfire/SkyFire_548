/*************************** rancombi.cpp **********************************
* Author:        Agner Fog
* Date created:  2001-10-18
* Last modified: 2008-11-30
* Project:       randomc.zip
* Source URL:    www.agner.org/random
*
* Description:
* This file defines a template class for combining two different
* random number generators. A combination of two random number
* generators is generally better than any of the two alone.
* The two generators should preferably be of very different design.
*
* Instructions:
* To make a combined random number generator, insert the class names
* of any two random number generators, as shown in the example below.
*
* Further documentation:
* The file ran-instructions.pdf contains further documentation and 
* instructions.
*
* Copyright 2001-2008 by Agner Fog. 
* GNU General Public License http://www.gnu.org/licenses/gpl.html
*****************************************************************************/

#include "randomc.h"

// This template class combines two different random number generators
// for improved randomness. R1 and R2 are any two different random number
// generator classes.
template <class RG1, class RG2>
class TRandomCombined : private RG1, private RG2 {
public:
   TRandomCombined(int seed) : RG1(seed), RG2(seed+1) {};

   void RandomInit(int seed) {        // re-seed
      RG1::RandomInit(seed);
      RG2::RandomInit(seed+1);
   }

   double Random() {
      double r = RG1::Random() + RG2::Random();
      if (r >= 1.) r -= 1.;
      return r;
   }

   int IRandom(int min, int max){       // output random integer
      // get integer random number in desired interval
      int iinterval = max - min + 1;
      if (iinterval <= 0) return 0x80000000; // error
      int r = int(iinterval * Random()); // truncate
      if (r >= iinterval) r = iinterval-1;
      return min + r;
   }
};

#if 1
//////////////////////////////////////////////////////////////////////////
// Example showing how to use the combined random number generator:
#include <stdio.h>
#include <time.h>
#include "userintf.cpp"
#include "mersenne.cpp"
#include "mother.cpp"

int main() {
   // Make an object of the template class. The names inside <> define the
   // class names of the two random number generators to combine.
   // Use time as seed.
   int seed = (int)time(0);
   int i;
   // Declare object of combined generator
   TRandomCombined<CRandomMersenne,CRandomMother> RG(seed);

   for (i = 0; i < 20; i++) {
      // generate 20 random floating point numbers and 20 random integers
      printf("\n%14.10f   %2i",  RG.Random(),  RG.IRandom(0,99));
   }

   printf("\n");
   EndOfProgram();
   return 0;
}
#endif