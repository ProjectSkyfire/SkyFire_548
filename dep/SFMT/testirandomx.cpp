/************************   testirandomx.cpp   ******************************
* Author:        Agner Fog
* Date created:  2008-11-16
* Last modified: 2008-11-28
* Project:       randoma and randomc
* Description:
* This program tests the difference between the functions IRandom and IRandomX
* in the worst case scenario. In less extreme cases the difference between
* IRandom and IRandomX is negligible.
*
* Instructions:
* Compile for console mode and run.
* The output shows the frequencies of different values modulo period = 3.
* The values are approximately equal if the distribution is perfectly
* uniform (IRandomX) within a few standard deviations (= squareroot of value).
* The values are different by more than a possible sampling error for
* IRandom, which shows that this function does not give an exactly 
* uniform distribution. It is recommended to use IRandomX rather than IRandom
* if the interval length (imax-imin+1) is large.
*
* Copyright 2008 by Agner Fog. 
* GNU General Public License http://www.gnu.org/licenses/gpl.html
*******************************************************************************/

#include <stdio.h>

#if 1  // Choose between Mersenne Twister and SFMT generator
  #include "mersenne.cpp"
  #define  STOC_BASE CRandomMersenne     // define random number generator base class
#else
  #include "sfmt.cpp"
  #define  STOC_BASE CRandomSFMT
#endif

int main() {
   // Define constants
   const int nsamp  =  8005000;        // Number of samples
   const int seed   =  1;              // Random number seed
   const int imin   = -0x5FFFFFFF;     // Lower limit for uniform random integers
   const int imax   =  0x60000000;     // Upper limit for uniform random integers
   const int period =  3;              // Periodicity

   unsigned int i;                     // Loop counter
   unsigned int x[period];             // Sample counters
   int r;                              // Random integer

   STOC_BASE ran1(seed);               // Make instance of random number generator
   for (i=0; i<period; i++) x[i] = 0;  // Initialize

   // Sample loop with IRandom
   for (i = 0; i < nsamp; i++) {
      r = ran1.IRandom(imin, imax);
      if (r > 0) {
         x[(unsigned int)r % period]++;
      }
   }
   printf("\nTest difference between IRandom and IRandomX");
   printf("\n\nSample counts for IRandom:");
   for (i = 0; i < period; i++) printf("\n%i  %9u", i, x[i]);

   // Repeat test with IRandomX
   ran1.RandomInit(seed);              // Reset generator with same seed
   for (i=0; i<period; i++) x[i] = 0;  // Initialize again

   // Sample loop with IRandomX
   for (i = 0; i < nsamp; i++) {
      r = ran1.IRandomX(imin, imax);
      if (r > 0) {
         x[(unsigned int)r % period]++;
      }
   }
   printf("\n\nSample counts for IRandomX:");
   for (i = 0; i < period; i++) printf("\n%i  %9u", i, x[i]);

   printf("\n\n");
   return 0;
}
