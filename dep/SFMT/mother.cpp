/**************************   mother.cpp   ************************************
* Author:        Agner Fog
* Date created:  1999
* Last modified: 2008-11-16
* Project:       randomc.h
* Platform:      This implementation uses 64-bit integers for intermediate calculations.
*                Works only on compilers that support 64-bit integers.
* Description:
* Random Number generator of type 'Mother-Of-All generator'.
*
* This is a multiply-with-carry type of random number generator
* invented by George Marsaglia.  The algorithm is:             
* S = 2111111111*X[n-4] + 1492*X[n-3] + 1776*X[n-2] + 5115*X[n-1] + C
* X[n] = S modulo 2^32
* C = floor(S / 2^32)
*
* Further documentation:
* The file ran-instructions.pdf contains further documentation and 
* instructions.
*
* Copyright 1999-2008 by Agner Fog. 
* GNU General Public License http://www.gnu.org/licenses/gpl.html
******************************************************************************/

#include "randomc.h"


// Output random bits
uint32_t CRandomMother::BRandom() {
  uint64_t sum;
  sum = (uint64_t)2111111111UL * (uint64_t)x[3] +
     (uint64_t)1492 * (uint64_t)(x[2]) +
     (uint64_t)1776 * (uint64_t)(x[1]) +
     (uint64_t)5115 * (uint64_t)(x[0]) +
     (uint64_t)x[4];
  x[3] = x[2];  x[2] = x[1];  x[1] = x[0];
  x[4] = (uint32_t)(sum >> 32);                  // Carry
  x[0] = (uint32_t)sum;                          // Low 32 bits of sum
  return x[0];
} 


// returns a random number between 0 and 1:
double CRandomMother::Random() {
   return (double)BRandom() * (1./(65536.*65536.));
}


// returns integer random number in desired interval:
int CRandomMother::IRandom(int min, int max) {
   // Output random integer in the interval min <= x <= max
   // Relative error on frequencies < 2^-32
   if (max <= min) {
      if (max == min) return min; else return 0x80000000;
   }
   // Assume 64 bit integers supported. Use multiply and shift method
   uint32_t interval;                  // Length of interval
   uint64_t longran;                   // Random bits * interval
   uint32_t iran;                      // Longran / 2^32

   interval = (uint32_t)(max - min + 1);
   longran  = (uint64_t)BRandom() * interval;
   iran = (uint32_t)(longran >> 32);
   // Convert back to signed and return result
   return (int32_t)iran + min;
}


// this function initializes the random number generator:
void CRandomMother::RandomInit (int seed) {
  int i;
  uint32_t s = seed;
  // make random numbers and put them into the buffer
  for (i = 0; i < 5; i++) {
    s = s * 29943829 - 1;
    x[i] = s;
  }
  // randomize some more
  for (i=0; i<19; i++) BRandom();
}
