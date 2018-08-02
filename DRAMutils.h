#ifndef DRAMutils_h // If DRAMutils.h is not defined...
#define DRAMutils_h // Define it here!

#include <Arduino.h>  //includes necessary C libraries that need to be used

/*
 * mem_addr type:
 * Special type that holds the DRAM memory address for 1 bit
 * Holds both a row and column address
 */
 typedef struct
{
  uint16_t row;   //Row address
  uint16_t col;   //Column address
} mem_addr;

/*
 * Pin initialization function:
 * Sets up the pins to allow the Arduino to properly output
 * Control, address, and data signals
 */
void DRAM_setup(void);

/*
 * Write Data function:
 * Given a memory address using the mem_addr type and data as a boolean
 * Will write the boolean to that location on the DRAM chip
 */
void DRAM_write(mem_addr addr, bool data);

/*
 * Read Data function:
 * Given a memory address of the mem_addr type
 * Will return the data at that location as a boolean
 */
bool DRAM_read(mem_addr addr);

/*
 * Refresh function:
 * Given a row address
 * Will refresh all data held in that row
 */
void DRAM_refresh(mem_addr addr);


// **************************
// * Special DRAM functions *
// **************************

/*
 * Read-Modify-Write function:
 * Given a memory address of type mem_addr and data as a boolean
 * Will return current contents at address and write new data at address
 */
bool DRAM_rmw(mem_addr addr, bool data);

#endif
