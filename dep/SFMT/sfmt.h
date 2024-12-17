/*****************************    sfmt.h    ***********************************
* Authors:
* Mutsuo Saito (Hiroshima University)
* Makoto Matsumoto (Hiroshima University)
* Agner Fog (Technical University of Denmark)
* Date created:  2006
* Last modified: 2009-02-08
* Project:       randomc
* Platform:      This C++ version requires an x86 family microprocessor 
*                with the SSE2 or later instruction set and a compiler 
*                that supports intrinsic functions.
* Source URL:    www.agner.org/random
*
* Description:
* This header file contains class declarations and other definitions for the 
* "SIMD-oriented Fast Mersenne Twister" (SFMT) random number generator.
*
* The SFMT random number generator is a modification of the Mersenne Twister 
* with improved randomness and speed, adapted to the SSE2 instruction set.
* The SFMT was invented by Mutsuo Saito and Makoto Matsumoto.
* The present C++ implementation is by Agner Fog.
*
* Class description:
* ==================
* class CRandomSFMT:
* Random number generator of type SIMD-oriented Fast Mersenne Twister.
*
* Member functions (methods):
* ===========================
* Constructor CRandomSFMT(int seed, int IncludeMother = 1):
* The seed can be any integer.
* Executing a program twice with the same seed will give the same sequence of
* random numbers. A different seed will give a different sequence.
* If IncludeMother is 1 then the output of the SFMT generator is combined
* with the output of the Mother-Of-All generator. The combined output has an
* excellent randomness that has passed very stringent tests for randomness.
* If IncludeMother is 0 then the SFMT generator is used alone.
*
* void RandomInit(int seed);
* Re-initializes the random number generator with a new seed.
*
* void RandomInitByArray(int seeds[], int NumSeeds);
* Use this function if you want to initialize with a seed with more than 
* 32 bits. All bits in the seeds[] array will influence the sequence of 
* random numbers generated. NumSeeds is the number of entries in the seeds[] 
* array.
*
* double Random();
* Gives a floating point random number in the interval 0 <= x < 1.
* The resolution is 52 bits.
*
* int IRandom(int min, int max);
* Gives an integer random number in the interval min <= x <= max.
* (max-min < MAXINT).
* The precision is 2^(-32) (defined as the difference in frequency between 
* possible output values). The frequencies are exact if (max-min+1) is a
* power of 2.
*
* int IRandomX(int min, int max);
* Same as IRandom, but exact. The frequencies of all output values are 
* exactly the same for an infinitely long sequence.
*
* uint32_t BRandom();
* Gives 32 random bits. 
*
*
* Example:
* ========
* The file EX-RAN.CPP contains an example of how to generate random numbers.
*
* Library version:
* ================
* An optimized version of this random number generator is provided as function
* libraries in randoma.zip. These function libraries are coded in assembly
* language and support only x86 platforms, including 32-bit and 64-bit
* Windows, Linux, BSD, Mac OS-X (Intel based). Use randoma.h from randoma.zip
*
*
* Non-uniform random number generators:
* =====================================
* Random number generators with various non-uniform distributions are available
* in stocc.zip (www.agner.org/random).
*
*
* Further documentation:
* ======================
* The file ran-instructions.pdf contains further documentation and 
* instructions for these random number generators.
*
*
* Copyright notice
* ================
* GNU General Public License http://www.gnu.org/licenses/gpl.html
* This C++ implementation of SFMT contains parts of the original C code
* which was published under the following BSD license, which is therefore
* in effect in addition to the GNU General Public License.
*
Copyright (c) 2006, 2007 by Mutsuo Saito, Makoto Matsumoto and Hiroshima University.
Copyright (c) 2008 by Agner Fog.
All rights reserved.
Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:
    > Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    > Redistributions in binary form must reproduce the above copyright notice, 
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    > Neither the name of the Hiroshima University nor the names of its 
      contributors may be used to endorse or promote products derived from 
      this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#ifndef SFMT_H
#define SFMT_H

#include <emmintrin.h>                 // Define SSE2 intrinsics
#include "randomc.h"                   // Define integer types etc

// Choose one of the possible Mersenne exponents.
// Higher values give longer cycle length and use more memory:
//#define MEXP   607
//#define MEXP  1279
//#define MEXP  2281
//#define MEXP  4253
  #define MEXP 11213
//#define MEXP 19937
//#define MEXP 44497

// Define constants for the selected Mersenne exponent:
#if MEXP == 44497
#define SFMT_N    348                  // Size of state vector
#define SFMT_M    330                  // Position of intermediate feedback
#define SFMT_SL1    5                  // Left shift of W[N-1], 32-bit words
#define SFMT_SL2	  3                  // Left shift of W[0], *8, 128-bit words
#define SFMT_SR1    9                  // Right shift of W[M], 32-bit words
#define SFMT_SR2	  3                  // Right shift of W[N-2], *8, 128-bit words
#define SFMT_MASK	  0xeffffffb,0xdfbebfff,0xbfbf7bef,0x9ffd7bff // AND mask
#define SFMT_PARITY 1,0,0xa3ac4000,0xecc1327a   // Period certification vector

#elif MEXP == 19937
#define SFMT_N    156                  // Size of state vector
#define SFMT_M    122                  // Position of intermediate feedback
#define SFMT_SL1   18                  // Left shift of W[N-1], 32-bit words
#define SFMT_SL2	  1                  // Left shift of W[0], *8, 128-bit words
#define SFMT_SR1   11                  // Right shift of W[M], 32-bit words
#define SFMT_SR2	  1                  // Right shift of W[N-2], *8, 128-bit words
#define SFMT_MASK	  0xdfffffef,0xddfecb7f,0xbffaffff,0xbffffff6 // AND mask
#define SFMT_PARITY 1,0,0,0x13c9e684   // Period certification vector

#elif MEXP == 11213
#define SFMT_N    88                   // Size of state vector
#define SFMT_M    68                   // Position of intermediate feedback
#define SFMT_SL1	14                   // Left shift of W[N-1], 32-bit words
#define SFMT_SL2	 3                   // Left shift of W[0], *8, 128-bit words
#define SFMT_SR1	 7                   // Right shift of W[M], 32-bit words
#define SFMT_SR2	 3                   // Right shift of W[N-2], *8, 128-bit words
#define SFMT_MASK	 0xeffff7fb,0xffffffef,0xdfdfbfff,0x7fffdbfd // AND mask
#define SFMT_PARITY 1,0,0xe8148000,0xd0c7afa3 // Period certification vector

#elif MEXP == 4253
#define SFMT_N    34                   // Size of state vector
#define SFMT_M    17                   // Position of intermediate feedback
#define SFMT_SL1	20                   // Left shift of W[N-1], 32-bit words
#define SFMT_SL2	 1                   // Left shift of W[0], *8, 128-bit words
#define SFMT_SR1	 7                   // Right shift of W[M], 32-bit words
#define SFMT_SR2	 1                   // Right shift of W[N-2], *8, 128-bit words
#define SFMT_MASK	 0x9f7bffff, 0x9fffff5f, 0x3efffffb, 0xfffff7bb // AND mask
#define SFMT_PARITY 0xa8000001, 0xaf5390a3, 0xb740b3f8, 0x6c11486d // Period certification vector

#elif MEXP == 2281
#define SFMT_N    18                   // Size of state vector
#define SFMT_M    12                   // Position of intermediate feedback
#define SFMT_SL1	19                   // Left shift of W[N-1], 32-bit words
#define SFMT_SL2	 1                   // Left shift of W[0], *8, 128-bit words
#define SFMT_SR1	 5                   // Right shift of W[M], 32-bit words
#define SFMT_SR2	 1                   // Right shift of W[N-2], *8, 128-bit words
#define SFMT_MASK	 0xbff7ffbf, 0xfdfffffe, 0xf7ffef7f, 0xf2f7cbbf // AND mask
#define SFMT_PARITY 0x00000001, 0x00000000, 0x00000000, 0x41dfa600  // Period certification vector

#elif MEXP == 1279
#define SFMT_N    10                   // Size of state vector
#define SFMT_M     7                   // Position of intermediate feedback
#define SFMT_SL1	14                   // Left shift of W[N-1], 32-bit words
#define SFMT_SL2	 3                   // Left shift of W[0], *8, 128-bit words
#define SFMT_SR1	 5                   // Right shift of W[M], 32-bit words
#define SFMT_SR2	 1                   // Right shift of W[N-2], *8, 128-bit words
#define SFMT_MASK	  0xf7fefffd, 0x7fefcfff, 0xaff3ef3f, 0xb5ffff7f  // AND mask
#define SFMT_PARITY 0x00000001, 0x00000000, 0x00000000, 0x20000000  // Period certification vector

#elif MEXP == 607
#define SFMT_N     5                   // Size of state vector
#define SFMT_M     2                   // Position of intermediate feedback
#define SFMT_SL1	15                   // Left shift of W[N-1], 32-bit words
#define SFMT_SL2	 3                   // Left shift of W[0], *8, 128-bit words
#define SFMT_SR1	13                   // Right shift of W[M], 32-bit words
#define SFMT_SR2	 3                   // Right shift of W[N-2], *8, 128-bit words
#define SFMT_MASK	  0xfdff37ff, 0xef7f3f7d, 0xff777b7d, 0x7ff7fb2f  // AND mask
#define SFMT_PARITY 0x00000001, 0x00000000, 0x00000000, 0x5986f054  // Period certification vector
#endif

// Class for SFMT generator with or without Mother-Of-All generator
class CRandomSFMT {                              // Encapsulate random number generator
public:
   CRandomSFMT(int seed, int IncludeMother = 0) {// Constructor
      UseMother = IncludeMother; 
      LastInterval = 0;
      RandomInit(seed);}
   void RandomInit(int seed);                    // Re-seed
   void RandomInitByArray(int const seeds[], int NumSeeds); // Seed by more than 32 bits
   int  IRandom  (int min, int max);             // Output random integer
   int  IRandomX (int min, int max);             // Output random integer, exact
   double Random();                              // Output random floating point number
   uint32_t BRandom();                           // Output random bits
private:
   void Init2();                                 // Various initializations and period certification
   void Generate();                              // Fill state array with new random numbers
   uint32_t MotherBits();                        // Get random bits from Mother-Of-All generator
   uint32_t ix;                                  // Index into state array
   uint32_t LastInterval;                        // Last interval length for IRandom
   uint32_t RLimit;                              // Rejection limit used by IRandom
   uint32_t UseMother;                           // Combine with Mother-Of-All generator
   __m128i  mask;                                // AND mask
   __m128i  state[SFMT_N];                       // State vector for SFMT generator
   uint32_t MotherState[5];                      // State vector for Mother-Of-All generator
};

// Class for SFMT generator without Mother-Of-All generator
// Derived from CRandomSFMT
class CRandomSFMT0 : public CRandomSFMT {
public:
   CRandomSFMT0(int seed) : CRandomSFMT(seed,0) {}
};

// Class for SFMT generator combined with Mother-Of-All generator
// Derived from CRandomSFMT
class CRandomSFMT1 : public CRandomSFMT {
public:
   CRandomSFMT1(int seed) : CRandomSFMT(seed,1) {}
};

#endif // SFMT_H
