#include "DRAMutils.h"
#include <Arduino.h>  // Includes necessary C and Arduino-specific libraries

/*
 * Macros to control Row Address Strobe (RAS) signals
 */
#define RAS_HIGH()  digitalWrite(13, HIGH)
#define RAS_LOW()   digitalWrite(13, LOW)

/*
 * Macros to control the Column Address Strobe (CAS) signals
 */
#define CAS_HIGH()  digitalWrite(12, HIGH)
#define CAS_LOW()   digitalWrite(12, LOW)

/*
 * Macros to control the WE (Write Enable) signals
 */
#define WE_HIGH()   digitalWrite(11, HIGH)
#define WE_LOW()    digitalWrite(11, LOW)

/*
 * Pin initialization function:
 * Sets up the pins to allow the Arduino to properly output
 * Control, address, and data signals
 */
void DRAM_setup(void)
{
  DDRD = 0b11111100; //Set digital pins 2-7 as lower part of address
  DDRC = 0b00000111; //Set analog pins A0-A2 as upper part of address
  DDRB = 0b00111011; //Set digital pins 9 - 13 as Control Lines and Data I/O
}

/*
 * Set Address function
 * NOTE: This function is only for internal use by other functions
 *       It cannot be accessed externally even with the library imported
 * Given any row or column address
 * The address will be properly shifted to align with the port registers
 * And the address value will be outputted
 */
void set_addr(uint16_t addr)
{
  PORTD = (addr << 2) & 0xFF;
  PORTC = (addr >> 5) & 0x07;
}

/*
 * Write Data function:
 * Given a memory address using the mem_addr type and data as a boolean
 * Will write the boolean to that location on the DRAM chip
 */
void DRAM_write(mem_addr addr, bool data)
{
  RAS_HIGH();             //Row Address Strobe (RAS) set HIGH
  CAS_HIGH();             //Column Address Strobe (CAS) set HIGH
  WE_LOW();               //Write Enable (WE) set LOW

  set_addr(addr.row);     //Set desired row address on pins

  RAS_LOW();              //RAS set LOW

  set_addr(addr.col);     //set desired column address out on pins

  digitalWrite(9, data);  //Data sent to D_in on chip through Digital pin 9

  CAS_LOW();              //CAS set LOW

  RAS_HIGH();             //RAS is set back to HIGH
  CAS_HIGH();             //CAS is set back to HIGH

}

/*
 * Read Data function:
 * Given a memory address of the mem_addr type
 * Will return the data at that location as a boolean
 */
bool DRAM_read(mem_addr addr)
{
  RAS_HIGH();             //Row Address Strobe (RAS) set HIGH
  CAS_HIGH();             //Column Address Strobe (CAS) set HIGH
  WE_HIGH();              //Write Enable (WE) set HIGH

  set_addr(addr.row);     //Set desired row address out on pins

  RAS_LOW();              //RAS set LOW

  set_addr(addr.col);     //Set desired column address out on pins

  CAS_LOW();              //CAS set LOW

  return digitalRead(10); //Data on D_out of DRAM chip returned by DigitalRead()

  RAS_HIGH();             //RAS is set back to HIGH
  CAS_HIGH();             //CAS is set back to HIGH


}

/*
 * Refresh function:
 * given an address, the function will refresh all data in the row
 */
void DRAM_refresh(mem_addr addr)
{
  set_addr(addr.row);   //Set row address on pins

  RAS_LOW();            //Set Row Address Strobe (RAS) LOW
  RAS_HIGH();           //Set RAS back HIGH
}
