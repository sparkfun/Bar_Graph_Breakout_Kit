/*
SparkFun Bargraph Breakout example sketch
Mike Grusin, SparkFun Electronics

This sketch shows how to access the Bargraph Breakout by toggling pins manually
without using the library or SPI. It will "slide" random data through the shift registers.

Hardware connections:

Because we're not using the SPI interface, you can use ANY digital pins for these functions.
THIS PROGRAM USES THE UNO PINS LISTED BELOW. If you're using a Mega, just use the
Uno pins (10,11,13), or change the assignments below this comment block.

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

// Default pin assignments for Uno and other non-Mega Arduinos

const int LAT = 10;  // Latch
const int SIN = 11;  // Serial data input (to bargraph)
const int CLK = 13;  // Clock

// Default pin assignments for Mega Arduinos
// to use, uncomment the below lines, and comment the above lines

//const int LAT = 53;  // Latch
//const int SIN = 51;  // Serial data input (to bargraph)
//const int CLK = 52;  // Clock

void setup()
// Runs once upon reboot
{
  // Set pins to output
  pinMode(LAT,OUTPUT);
  pinMode(SIN,OUTPUT);
  pinMode(CLK,OUTPUT);
}

void loop()
// Runs continuously after setup() ends
{
  // This example code will shift random 1s or 0s into the bargraph, and toggle the
  // LAT pin on every bit so we can see the data sliding down the shift registers.
  // (Note that you can't do this with the library or SPI interface, as it sends 8 bits at a time!)

  // To clock data into the Bargraph, set SIN to the state you want, and transition CLK L to H.

  // To transfer the internal shift register data to the output pins (LEDs), transition LAT L to H.

  digitalWrite(LAT,LOW); // Make LAT low

  // Put a random bit (1 or 0) into the shift register:

  digitalWrite(CLK,LOW); // Make CLK low
  digitalWrite(SIN,random(2)); // Put a random 0 or 1 on SIN (this is your input data)
  digitalWrite(CLK,HIGH); // Make CLK high - this loads SIN into the shift register

  // Normally we would  do the above 32 times to fill all the shift registers with the data you want,
  // then toggle the LAT pin to make it visible. But we'll do that here for EACH bit we clock in,
  // to show the data flowing through the shift registers:

  digitalWrite(LAT,HIGH);

  delay(30); // Short delay so we can see the data, try making it faster or slower!
}
