/*
 * DRAM serialized Read/Write Demo
 * This program demonstrates the DRAMutils DRAM_write(), DRAM_read(),
 * and DRAM_refresh() functions by first breaking up a type char variable
 * to its individual bits, writing those bits to the DRAM chip
 * and then reading those bits back.
 */

#include <DRAMutils.h>

// Macro used to obtain number of bits on a given variable
#define sizeof_bits(x) (sizeof(x) * 8)
// Macro used to obtain individual bits from a variable
#define serializer(data, shift) ((data >> shift) & 1)

void setup()
{

  DRAM_setup();       // Arduino pins are first setup

  Serial.begin(9600); // Serial output set to 9600 Baud (Standard)
}

void loop()
{

  char data = 0b01100001;                         //Data to be written declared
  char text_buff[2];                              //Text buffer for formatted string printing
  mem_addr addr = {.row = 0x000, .col = 0x000};   //Memory address to start with

  /*
   *  Bits from variable are extracted one by one and then written to DRAM
   */

  for(int i = 0; i < sizeof_bits(char) - 1; ++i)
  {
    DRAM_write(addr, serializer(data, i));        //Write extracted bit to DRAM
    addr.col += 1;                                //Increment column address by 1
  }

  delay(7000);           //Delay all operations for 7 seconds
  DRAM_refresh(addr);    //Refresh memory
  delay(7000);           //Delay all operations for 7 seconds
  DRAM_refresh(addr);    //Refesh memory

  /*
   *  Bits from memory are read one by one and printed
   */
  for(int i = 0 ; i < sizeof_bits(char); ++i)
  {
    sprintf(text_buff, "%d", DRAM_read(addr));  //Read bit at address
    Serial.print(text_buff);                    //Print extracted bit
    addr.col -= 1;  //Decrement column address by 1
  }


  delay(500); //Delay to allow priting to complete
  exit(0);    //Terminate program



}
