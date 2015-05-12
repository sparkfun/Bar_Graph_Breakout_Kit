/*
SparkFun Bargraph Breakout example sketch
Mike Grusin, SparkFun Electronics

This sketch shows how to use the SFEbarGraph library to send data to SparkFun's Bargraph Breakout board

Hardware connections:

Connect the following pins on the bargraph board to your Arduino:

Name	Function	Uno pin		Mega pin

+5V	Power supply	5V		5V
GND	Ground		GND		GND
SIN	SPI input	11 (MOSI)	51 (MOSI)
CLK	Clock input 	13 (SCK)	52 (SCK)
LAT	Latch input 	10 (SS)		53 (SS)

License:

This code is free to use, change, improve, even sell!  All we ask for is two things:
1. That you give SparkFun credit for the original code,
2. If you sell or give it away, you do so under the same license so others can do the same thing.
More at http://creativecommons.org/licenses/by-sa/3.0/

Have fun! 
-your friends at SparkFun
*/

// Step one: include both the SFEbarGraph and SPI libraries

#include <SparkFunBarGraph.h>
#include <SPI.h>

// Step two: create a bargraph object; we call it BG in this example but you can call it whatever you wish
// (use the same name in front of all the library functions)

SFEbarGraph BG;

// Some of the math we're doing in this example requires the number of bargraph boards
// you have connected together (normally this is one, but you can have a maximum of 8).

const int numbargraphs = 1;

void setup()
// Runs once upon reboot
{
  // Let's initialize the bargraph library!
  // You can call BG.begin() several ways:

  // Without parameters, it defaults to one bargraph board and LATch pin set to 10 on Uno, or 53 on Mega.

  // You can also call it with the number of daisy-chained boards (1-8, default 1), using the default LATch pin.
  
  // Or you can call it with both the number of boards and the LATch pin you wish to use.
  // For two bargraphs and pin 9 for the latch pin, call BG.begin(2,9);
  
  BG.begin(numbargraphs);
}

void loop()
// Runs continuously after setup() ends
{
  unsigned long endtime;

  // BASIC BARGRAPH!
  
  // Turn all LEDs on then off using BG.barGraph()

  // BG.BarGraph() takes two values, one for the "bar" (how many LEDs to light), 
  // and a separate floating "peak" which lights up one LED.
  // When you call BG.barGraph(), the values are immediately displayed on the LEDs.
  // Numbering starts at 1, 0 = all LEDs off.

  unsigned char bar;

  for(bar=0;bar<=(numbargraphs*30);bar++) // Increase value from 0 (all off) to the number of LEDs we have
  {
    BG.barGraph(bar,0); // Make a bargraph! (No "peak" indicator)
    delay(50);
  }
  
  for(bar=(numbargraphs*30)+1;bar>0;bar--) // Now decrease the number of LEDs to zero (all off)
  {
    BG.barGraph(bar-1,0); // Make a bargraph! (No "peak" indicator)
    delay(50);
  }
  delay(500);


  // BASIC PAINT!

  // Turn all LEDs on then off using BG.paint()

  // BG.Paint() lets you turn individual LEDs on and off. This is done on an internal "canvas".
  // Use clear() to set the canvas to all LEDs off. Use paint(LED, state) to turn an individual LED
  // on or off (for state, 1 or true or HIGH = on, 0 or false or LOW = off). LED numbering starts at 0.
  // When you're done "painting" the canvas and are ready to display it on the LEDs, call send();
  // The bargraph will continue displaying that data until you call send() again.
  
  unsigned char LED;

  for(LED=0;LED<(numbargraphs*30);LED++) // Increment through all the LEDs from 0 to as many as we have
  {
    BG.paint(LED,HIGH); // Turn LED on
    BG.send(); // Send the canvas to the bargraph
    delay(50);
  }

  for(LED=0;LED<(numbargraphs*30);LED++) // Increment through all the LEDs from 0 to as many as we have
  {
    BG.paint(LED,LOW); // Turn LED off
    BG.send(); // Send the canvas to the bargraph
    delay(50);
  }
  delay(500);


  // ENTERPRISE!
  
  // Put this under your viewport, and stare down the enemy.
  
  unsigned char spot;

  // Do this for 10 seconds
  endtime = millis() + 10000;
  while (millis() < endtime)
  {
    // Step through all the LEDs from 0 to as many as we have
    for (spot = 0; spot < (numbargraphs*30); spot++)
    {
      BG.clear(); // Clear the canvas

      BG.paint(spot,HIGH); // Turn on one LED

      BG.send(); // Send the canvas to the display
      delay(50); // Pause a moment (decrease delay to increase speed!)
    }
  }
  BG.clear(); BG.send(); delay(500);


  // PONG!

  // A very simple pong field.
  
  unsigned char ball;

  // Do this for 10 seconds
  endtime = millis() + 10000;
  while (millis() < endtime)
  {
    // Step through all the LEDs from 0 to as many as we have
    for (ball = 0; ball < (numbargraphs*30); ball++)
    {
      BG.clear(); // Clear the canvas

      BG.paint(ball,HIGH); // Turn on one LED

      BG.send(); // Send the canvas to the display
      delay(50); // Pause a moment (decrease delay to increase speed!)
    }

    // Go the other way...
    for (ball = (numbargraphs*30); ball > 0; ball--)
    {
      BG.clear(); // Clear the canvas

      BG.paint(ball-1,HIGH); // Turn on one LED

      BG.send(); // Send the canvas to the display
      delay(50); // Pause a moment (decrease delay to increase speed!)
    }
  }
  BG.clear(); BG.send(); delay(500);


  // CYLON!

  // By your command.
  
  float angle = 0.0;
  unsigned char center;
  unsigned char y;

  // Do this for 10 seconds
  endtime = millis() + 10000;
  while (millis() < endtime)
  {
    // Slowly step through an entire sine wave
    for (angle = 0; angle < (PI * 2); angle += 0.0005) 
    {
      // Clear the canvas
      BG.clear();

      // Compute the center LED to light
      center = (sin(angle)* numbargraphs * 13)+(numbargraphs*15);

      // Light up a few on either side of the center
      for (y = center-2; y <= center+2; y++)
        BG.paint(y,HIGH);

      // Send the canvas to the display
      BG.send();
    }
  }
  BG.clear(); BG.send(); delay(500);


  // FAKE MUSIC!

  // This shows a traditional bargraph, with peak detector
  // We're making a fake waveform here; replace "wave" with your sensor value
  // (scaled from 0 to as many LEDs as you have)
  
  float sine = 0.0, peak = 0.0, wave;

  // Do this for 10 seconds
  endtime = millis() + 10000;
  while (millis() < endtime)
  {
    // Make a fake music wave by adding up some sine waves
    sine = sine + 0.01;
    wave = (numbargraphs * 15) +
        sin(sine) * (numbargraphs * 5) +
        sin(sine*.7) * (numbargraphs * 5) +
        sin(sine*.3) * (numbargraphs * 5);   

    // Simulate the "peak" value
    if (wave > peak) // If the wave is bigger than the peak, increase the peak        
      peak = wave;
    else // If the wave is smaller than the peak, allow the peak to slowly drop
      peak = peak - 0.001;
      
    // Generate the bargraph
    BG.barGraph(wave,peak);
  }
  BG.clear(); BG.send(); delay(500);


  // DANCING SINE BUGS!
  
  // Paint 8 moving bugs onto the bargraph, and watch them dance

  // Create 8 bugs
  float bug[8];
  int x;

  // Zero them all
  for(x=0; x<8; x++)
    bug[x] = 0.0;  
    
  // Do this for 30 seconds
  endtime = millis() + 30000;
  while (millis() < endtime)
  {
    // Clear the canvas
    BG.clear();

    // Move each bug along a sine wave at different rates
    for(x=0; x<8; x++)
    {
      bug[x] += ((PI/8000.0) * (x+1));
      BG.paint((numbargraphs*15)+(sin(bug[x])*(numbargraphs * 15)),HIGH);
    }

    // Send the canvas to the display
    BG.send();
  }
  BG.clear(); BG.send(); delay(500);


  // BINARY COUNTING!

  // We'll use the BG.sendLong() command to write a "long" (4-byte) number
  // directly to the bargraph
  
  static unsigned long number;
  
  // Do this for 30 seconds
  endtime = millis() + 30000;
  while (millis() < endtime)
  {
    // increment the number
    number++;
    // send it to the bargraph
    BG.sendLong(number);

    //delay(1);
  }
  BG.clear(); BG.send(); delay(500);


  // RANDOM SHIFT REGISTER!

  // We'll use the BG.sendLong() command to shift random bits into a "long" (4-byte) number
  // and send it directly to the bargraph
  
  static unsigned long number2;
  
  // Do this for 20 seconds
  endtime = millis() + 20000;
  while (millis() < endtime)
  {
    // shift all the bits in "number2" left by 1
    number2 = (number2 << 1);
    // put a random 0 or 1 into the least-significant bit of "number2"
    number2 |= random(2);
    // send it to the bargraph
    BG.sendLong(number2);
    // short delay
    delay(40);
  }
  BG.clear(); BG.send(); delay(500);
}

