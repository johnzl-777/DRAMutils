/*
 * This program demos the DRAMutils library
 * It tests the maximum amount of a time a DRAM cell can hold one bit before needing to be refreshed
 */
 
#include <DRAMutils.h> //Include the DRAMutils library

void setup() 
{
  
  DRAM_setup();  //a function of the DRAMutils library, sets up pins on the Arduino 

  Serial.begin(9600); //Set Serial output to 9600 Baud (standard)

}

int delay_time = 0;   //A variable to hold the delay time is created and set to 0

void loop() 
{
  
  char print_str[20];   //A buffer is created for use with sprintf() to allow formatted string printing

  /*
   * A random memory address declared using the mem_addr type, part of the DRAMutils library
   * .row and .col can be any value between 0x000 to 0x1FF for the Hitachi HM50256-15 being tested
   */
   mem_addr address = {.row = 0x057, .col = 0x054};   //Row address set to 0x057, column address at 0x054 
  
  /*
   * Data is written to the bit that the address points to
   * write_data() is also part of the DRAMutils library,
   * accepting an address in the mem_addr type and a boolean for the "0" or "1" to be written
   */
  DRAM_write(address, 0);   //0 written to specified address  

  
  delay(delay_time);        //Read operation is delayed for "delay_time" milliseconds 
  
  if(DRAM_read(address) == 0)
  {
    //If the data read is still 0...

    /*
     * Data is first read from the DRAM chip using the read_data function, part of DRAMutils
     * Given an address using the mem_addr type it will return the bit read as a boolean
     */
    sprintf(print_str, "%d \nDELAY TIME: %d", DRAM_read(address), delay_time); 
    Serial.println(print_str); //Print current delay time
    delay_time += 1;              //Increment delay_time by 1 millisecond
  }
  else
  {
    // If the data read is anything other than 0...
    sprintf(print_str, "MAXIMUM DELAY: %d", delay_time);  
    Serial.println(print_str); //print current delay time (now its maximum delay time)
    delay(100);                //Give a slight delay to allow the Arduino to finish serial data output
    exit(0);                   //Terminate the loop
  }

 
}
