/*
SparkFun Bargraph Breakout Arduino library
Mike Grusin, SparkFun Electronics

Revision history:
1.1 12/27/2011 updated for Arduino 1.0, changed begin() parameter order, fixed example which always used pin 10 for LAT, even on a Mega
1.0 10/10/2011 release

Hardware connections:

Connect the following pins on the bargraph board to your Arduino:

Name	Function		Uno pin		Mega pin

+5V		Power supply	5V			5V
GND		Ground			GND			GND
SIN		SPI input		11 (MOSI)	51 (MOSI)
CLK		Clock input 	13 (SCK)	52 (SCK)
LAT		Latch input 	10 (SS)		53 (SS)

Note: you can use any available pin for the LATch pin, see begin() below.

Library usage:

1. Install SparkFunBarGraph folder in your sketches/libraries folder. Restart the Arduino IDE.

2. See the example sketches, located in the Arduino IDE under examples/SFEbarGraph

3. Include the library and SPI at the begining of your sketch:

   #include <SparkFunBarGraph.h>
   #include <SPI.h>

4. Declare a variable of type SFEbarGraph (we call it "BG" here):

   SFEbarGraph BG;

5. Call "BG.begin()" to set up variables. Call without parameters for defaults (one board, 
   LAT pin = 10 for Uno etc. and 53 for Mega), or call with BG.begin(numberofboards) or
   BG.begin(numberofboards,latchpin)  

   BG.begin(); // use default pins and one board
   
6. Now you can drive the bargraph with BG.barGraph(), BG.clear(), BG.paint(), and BG.send().

License:

This code is free to use, change, improve, even sell!  All we ask for is two things:
1. That you give SparkFun credit for the original code,
2. If you sell or give it away, you do so under the same license so others can do the same thing.
More at http://creativecommons.org/licenses/by-sa/3.0/

Have fun! 
-your friends at SparkFun
*/

#ifndef SparkFunBarGraph_h
#define SparkFunBarGraph_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class SFEbarGraph
{
    public:
        SFEbarGraph();

        boolean begin();
        boolean begin(unsigned char numboards);
        boolean begin(unsigned char numboards, unsigned char latchpin);
        // Initialize the library, with optional number of boards (default 1, max 8) and latch pin (default 10)
		// Returns true if successful, false if memory couldn't be allocated
 
        void barGraph(unsigned char bar, unsigned char peak);
		// Use the board as a traditional bargraph
		// Bar = number of LEDs to light (0 for none), 
		// Peak = optional "peak" overlay (0 for none)
		// LED numbering starts at 1, use 0 for no LEDs lit

        // The following functions allow you to turn arbitrary LEDs on / off
        // Note that all "painting" occurs in the background, and does not appear on the LEDs until send()

        void clear();
        // Clear the "canvas" to all LEDs off
 
        void paint(unsigned char position, boolean value);
		// Turn arbitrary LEDs on (1 or true or HIGH), or off (0 or false or LOW) 
		// Note that LED numbering starts at 0

        void send();
		// Send the current "canvas" to the bargraph board
		// LEDs will remain fixed until next send()

        // Misc functions
		
		void sendLong(unsigned long number);
		// Sends the binary representation of "number" directly to the bargraph board
		// Least-significant bit goes to lowest LED
		// Note that this only works effectively for one bargraph board
};

#endif
