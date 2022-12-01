/*
   Copyright (C) 2022 SFini

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
  * @file LedMatrixWithButtons.ino
  *
  * 3 x LED 8x8 Matrix with Buttons
  */

#include <MD_MAX72xx.h>
#include <SPI.h>
#include <ezButton.h>

#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW
#define MAX_DEVICES 3

#define SPI_CLK_PIN   18 // or SCK
#define SPI_DATA_PIN  23 // or MOSI
#define SPI_CS_PIN    15 // or SS

MD_MAX72XX display = MD_MAX72XX(HARDWARE_TYPE, SPI_DATA_PIN, SPI_CLK_PIN, SPI_CS_PIN, MAX_DEVICES);

ezButton   button1( 0);
ezButton   button2(16);
ezButton   button3( 4);

int        number1 = 0;
int        number2 = 0;
int        number3 = 0;

/** I use fix number pixels. */
const uint8_t numbers[10][8] = {
  {   0,  62,  81,  73,  69,  62,   0,   0 }, // 48 - '0'
  {   0,   4,   2, 127,   0,   0,   0,   0 }, // 49 - '1'
  {   0, 113,  73,  73,  73,  70,   0,   0 }, // 50 - '2'
  {   0,  65,  73,  73,  73,  54,   0,   0 }, // 51 - '3'
  {   0,  15,   8,   8,   8, 127,   0,   0 }, // 52 - '4'
  {   0,  79,  73,  73,  73,  49,   0,   0 }, // 53 - '5'
  {   0,  62,  73,  73,  73,  48,   0,   0 }, // 54 - '6'
  {   0,   3,   1,   1,   1, 127,   0,   0 }, // 55 - '7'
  {   0,  54,  73,  73,  73,  54,   0,   0 }, // 56 - '8'
  {   0,   6,  73,  73,  73,  62,   0,   0 }, // 57 - '9'
};

/** Display one number on one of the displays */
void printNumber(int displayNr, int number)
{
   int16_t offset = ((displayNr + 1) * COL_SIZE) - 1;

   display.control(displayNr, displayNr, MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
   for (int i = 0; i < COL_SIZE; i++) {
      display.setColumn(offset - i, numbers[number][i]);
   }
   display.control(displayNr, displayNr, MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}

/** Check if one button is pressed (with debouncing) */
void checkButton(ezButton &button, int &number) 
{
   button.loop();

   if (button.isPressed()) {
      // Serial.println("The button is pressed");
      number++;
      if (number > 9) {
         number = 0;
      }
   }
}

/** Check if two or more buttons are pressed.  */
bool isReset()
{
   int pushed = 0;
   
   if (button1.getStateRaw() == 0) {
      pushed++;
   }
   if (button2.getStateRaw() == 0) {
      pushed++;
   }
   if (button3.getStateRaw() == 0) {
      pushed++;
   }
   return (pushed >= 2);
}

/** Main loop
  * Setup serial
  * Initialize the display driver
  * and Initialize the buttons
  */
void setup()
{
   Serial.begin(19200);
   Serial.print("LED 8x8 Button Test!\n");
  
   display.begin();
   display.control(MD_MAX72XX::INTENSITY, 3);

   button1.setDebounceTime(50);
   button2.setDebounceTime(50);
   button3.setDebounceTime(50);
}

/** Main loop
  * Check the buttons
  * Reset the all numbers if more than 2 buttons are pressed
  * And update the 3 numbers every 100ms
  */
void loop()
{
   static int msec = millis();
   
   checkButton(button1, number1);
   checkButton(button2, number2);
   checkButton(button3, number3);

   if (isReset()) {
      number1 = number2 = number3 = 0;
      Serial.println("Reset");
   }

   if (millis() - msec > 100) {
      msec = millis();

      printNumber(0, number1);
      printNumber(1, number2);
      printNumber(2, number3);
   }
}
