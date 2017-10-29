# DRAMutils library

A Library that allows the Arduino UNO to read/write data to older DIP DRAM
chips

## How it Works

Before explaining how the functions in the library work it is necessary to
briefly describe the layout of the DRAM chip.

The chip in question is the Hitachi HM50256-15 which has:
* 9 Address pins
* 1 Data Out pin
* 1 Data In pin
* 1 Row Address Strobe (RAS) pin
* 1 Column Address Strobe (CAS) pin
* 1 Vcc (+5V) pin
* 1 Vss (GND) pin

More information about the chip can be found here:
[Hitachi HM50256-15 Datasheet](http://www.minuszerodegrees.net/memory/41256/datasheet_HM50256.pdf)


Most DRAM chips follow a standard sequence of signals for reading, writing,
and refreshing, as well as other specialized read/write operations like
Read-Modify-Write, Extended Data Out (EDO), etc.

DRAMutils only provides the simple read, write, and refresh functions. Their
operation in conjunction with the chip will be described below.

The timing diagrams used to explain each function as well as further information
on the previously mentioned Read-Modify-Write and EDO modes can be found here:
[Understanding DRAM operation](https://www.ece.cmu.edu/~ece548/localcpy/dramop.pdf)

### Setup

Prior to being able to perform any operation on the DRAM chip, it must first
be wired correctly to the Arduino. The setup should be as follows:

* Digital pins 2 - 7 should be wired to A0 - A5 of the DRAM address bus
* Analog pins A0 - A2 should be wired to A6 - A8 of the DRAM address bus
* Digital pin 13 should be wired to the RAS pin
* Digital pin 12 should be wired to the CAS pin
* Digital pin 11 should be wired to the WE pin

Once everything is properly connected, the Arduino pins must be setup internally.

DRAMutils provides a `DRAM_setup` function that, when called in the `setup()`
function of the Arduino sketch will setup all the pins for proper operation
like so:

```Arduino
void setup()
{
  DRAM_setup();

  //insert remaining code here
}

```

### Read Operation

The timing diagram below

### Write Operation

### Refresh Operation
