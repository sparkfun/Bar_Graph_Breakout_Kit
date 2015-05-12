/*
SparkFun Bargraph Breakout Arduino library
Mike Grusin, SparkFun Electronics

Revision history:
1.2 05/12/2015 Updated for Arduino 1.6 - fixing directory formatting, updating file names
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

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "SparkFunBarGraph.h"
#include "SPI.h"

// Private global variables:

unsigned char *_SFEbarGraphCanvas; // Pointer to array declared in begin()
unsigned char _SFEbarGraphLatchPin; // Latch pin
unsigned char _SFEbarGraphNumBoards; // Number of boards

SFEbarGraph::SFEbarGraph(){}


boolean SFEbarGraph::begin()
// Initialize the library with default values (latch pin = 10 or 53, number of boards = 1)
{
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
  return (begin(1,10)); // Uno, etc.
#else
  return (begin(1,53)); // Mega, etc.
#endif
}


boolean SFEbarGraph::begin(unsigned char numboards)
// Initialize the library, with user-defined latch pin and default number of boards (1)
{
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
  return (begin(numboards,10)); // Uno, etc.
#else
  return (begin(numboards,53)); // Mega, etc.
#endif
}


boolean SFEbarGraph::begin(unsigned char numboards, unsigned char latchpin)
// Initialize the library, with user-defined latch pin and user-defined number of boards (1 to 8)
{
  if ((numboards > 8) || (numboards < 1)) // Error check
  {
//  Serial.print("SFEbarGraph::begin ERROR: illegal number of boards");
    return(false);
  }

  _SFEbarGraphNumBoards = numboards;
//  Serial.print("numboards: "); Serial.println(_SFEbarGraphNumBoards,DEC);

  _SFEbarGraphLatchPin = latchpin;
//  Serial.print("latchpin: "); Serial.println(_SFEbarGraphLatchPin,DEC);

  pinMode(_SFEbarGraphLatchPin,OUTPUT);
	
  // The bargraph will operate at any SPI speed the Arduino is capable of.
  // If you are also using other SPI devices such as Serial 7-Segment displays,
  // you may need to slow the bus down by uncommenting the following line:
  // SPI.setClockDivider(SPI_CLOCK_DIV128);

  SPI.begin(); 

  // Allocate 4 bytes of memory for each connected bargraph
  _SFEbarGraphCanvas = (unsigned char*)malloc(_SFEbarGraphNumBoards * 4);

  // Make sure memory allocation was successful, return false if there was a problem
  if (_SFEbarGraphCanvas != 0)
  {
    clear();
    return(true);
  }
  else
    return(false);
}


void SFEbarGraph::barGraph(unsigned char bar, unsigned char peak)
// Use the board as a traditional bargraph
// Bar = number of LEDs to light (0 for none), 
// Peak = optional "peak" overlay (0 for none)
// LED numbering starts at 1, use 0 for no LEDs lit
{
  unsigned char x, affectedByte, affectedBit;
  
  bar += ((bar-1) / 30) * 2; // Add two bits for every 30 (shift register is 32 bits, but there are only 30 LEDs)
  
  if (bar <= (_SFEbarGraphNumBoards * 32)) // error check
  {
    affectedByte = bar / 8; // Compute byte and bit that "bar" resides in
	affectedBit = bar % 8;

	//Serial.print(bar,DEC);
	//Serial.print(" ");
	//Serial.print(affectedByte,DEC);
	//Serial.print(" ");
	//Serial.print(affectedBit,DEC);
	//Serial.print(" ");
	//Serial.println((1 << affectedBit)-1,DEC);

	clear(); // Zero all bytes
	
	// Fill all bytes below affected byte with 0xFF (all LEDs on)
	for (x = 0; x < affectedByte; x++)
	  _SFEbarGraphCanvas[x] = 0xFF;

	// Turn on all LEDs in affectedbyte up to affectedbit (note the "-1" below)
	_SFEbarGraphCanvas[affectedByte] = (1 << affectedBit)-1;
	
	// Handle peak LED if required
	if (peak > 0) paint(peak-1,true);
	
	// Send the data to the bargraph
	send();
  }
}


// The following functions allow you to turn arbitrary LEDs on / off
// Note that all "painting" occurs in the background, and does not appear on the LEDs until send()


void SFEbarGraph::clear()
// Clear the "canvas" to all LEDs off
{
  int x;

  // Zero all bytes (four per board)
  for (x = 0; x < (_SFEbarGraphNumBoards * 4); x++)
    _SFEbarGraphCanvas[x] = 0;
}


void SFEbarGraph::paint(unsigned char position, boolean value)
// Turn arbitrary LEDs on (1 or true or HIGH), or off (0 or false or LOW) 
// Note that LED numbering starts at 0
{
  unsigned char affectedByte, affectedBit;

  //Serial.print(position,DEC);
  
  position += (position / 30) * 2; // Add two bits for every 30 (shift register is 32 bits, but there are only 30 LEDs)

  if (position < (_SFEbarGraphNumBoards * 32)) // Error check
  {
    affectedByte = position / 8; // Compute byte and bit that "position" resides in
    affectedBit = position % 8;

    //Serial.print(" ");
    //Serial.print(position,DEC);
    //Serial.print(" ");
    //Serial.print(affectedByte,DEC);
    //Serial.print(" ");
    //Serial.println(affectedBit,DEC);

    if (value) // Make bit 1
      _SFEbarGraphCanvas[affectedByte] |= (1 << affectedBit); // Mask and set bit
    else // Make bit 0
      _SFEbarGraphCanvas[affectedByte] &= ~(1 << affectedBit); // Mask and clear bit
  }
}


void SFEbarGraph::send()
// Send the current "canvas" to the bargraph board
// LEDs will remain fixed until next send()
{
  unsigned char x;
 
  digitalWrite(_SFEbarGraphLatchPin,LOW); // Make LAT low

  // Send all bytes to board(s) (four bytes per board)
  for (x = (_SFEbarGraphNumBoards*4); x > 0; x--)
    SPI.transfer(_SFEbarGraphCanvas[x-1]);

  digitalWrite(_SFEbarGraphLatchPin,HIGH); // Make LAT high, L-H transition causes data to appear at LEDs
}


void SFEbarGraph::sendLong(unsigned long number)
// Sends the binary representation of "number" directly to the bargraph board
// Least-significant bit goes to lowest LED
// Note that this only works effectively for one bargraph board
{
  char x;
  unsigned char *z;
  unsigned char a;
  
  digitalWrite(_SFEbarGraphLatchPin,LOW); // Make LAT low

  // A long is four bytes
  z = (unsigned char*)&number; // pointer to first byte

  // Retrieve each byte from memory
  for (x = 3; x >= 0; x--)
  {
    a = *(z+x); // get each byte in the long
    SPI.transfer(a); // send it to the bargraph
  }
  digitalWrite(_SFEbarGraphLatchPin,HIGH); // Make LAT high, L-H transition causes data to appear at LEDs
}
