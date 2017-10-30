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

![DRAM pins](https://github.com/johnzl-777/DRAMutils/blob/master/Schematics/DRAM%20chip%20layout.png)

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

### Write Operation

The timing diagram below shows how a write operation is performed

![Write Timing Diagram](https://github.com/johnzl-777/DRAMutils/blob/master/Timing%20Diagrams/IBM%20Write%20Timing%20Diagram.png)

The steps that allow a write operation to occur are as follows:

1. `RAS` and `CAS` must be set HIGH first. `WE` can be LOW or HIGH when
starting but for simplicity is set LOW in the beginning and must be set LOW
later (if it is HIGH) to signal a write operation
2. A row address is applied to the address bus
3. `RAS` must go from HIGH to LOW
4. A column address is applied to the address bus
5. The desired data to be written is sent on the Data In pin
6. `CAS` must go from HIGH to LOW
7. `RAS` and `CAS` are set to their original HIGH states and the operation is
complete

The `DRAM_write` function included in the DRAMutils library allows you to
write a bit to the DRAM chip given a memory address and the desired bit to
be written, expressed as a boolean.

DRAMutils defines a variable type known as `mem_addr` which is a `struct`
holding a row and column address as type `uint16_t`. The address that you
would like to write to can be expressed as such:

```Arduino
void loop()
{
  mem_addr address = {.row = 0x076, .col = 0x1AA}
}
```

Thus, an example of a full write operation can be seen below:

```Arduino
void setup()
{
  DRAM_setup(); //sets up Arduino pins
}

void loop()
{
  mem_addr write_addres = {.row = 0x076, .col = 0x1AA} // Declare DRAM address
  bool Data = 0;  //Data to be written

  DRAM_write(write_addres, Data); //Data written to specified address
}
```


### Read Operation

The timing diagram below shows how a read operation is performed

![Read Timing Diagram](https://github.com/johnzl-777/DRAMutils/blob/master/Timing%20Diagrams/IBM%20Read%20Timing%20Diagram.png)

The steps that allow a read operation occurs as follows:

1. `RAS` and `CAS` must be set HIGH in the beginning. `WE` can be either LOW
or HIGH but for the sake of convenience is set HIGH in the very beginning
to denote a Read operation is being performed.
2. A row address is applied to the address bus
3. `RAS` goes from HIGH to LOW
4. A column address is applied to the address bus
5. `CAS` goes from HIGH to LOW
6. Data is outputted via the Data Out pin
7. `RAS` and `CAS` are set back to their original states, which is HIGH

The `DRAM_read` function included in the DRAMUtils library reads data from the
DRAM chip given an address declared in the mem_addr type as displayed
in the Write Operation description and returns the data read as a boolean.

An example of a full read operation can be seen below:

```Arduino
void setup()
{
  DRAM_setup(); //Setup Arduino pins for read operation
}

void loop()
{
  mem_addr mem_address = {.row = 0x076, .col = 0x1AA}; // Memory address declared
  bool write_data = 0; // Data to be written
  bool data_read; // Place to store data created

  DRAM_write(mem_address, write_data); //Write data to address
  data = DRAM_read(read_address); //Read data written to address
}
```
It should be noted that you should only read a memory address that has already
been written to. So in the example above there would be an accompanying
`DRAM_write` call before `DRAM_read`

### Refresh Operation

The timing diagram below shows how a standard Row refresh is performed
![Row Refresh](https://github.com/johnzl-777/DRAMutils/blob/master/Timing%20Diagrams/HM50256%20Original%20Refresh%20Timing%20Diagram.png)

The steps that allow a Refresh operation are as follows:

1. The row address of the data to be refreshed is put out on pins
2. `RAS` must be set from HIGH to LOW
3. `RAS` must be set from LOW back to HIGH

The `DRAM_refresh` function included in the DRAMutils library performs such
an operation and can be invoked between `DRAM_write` and `DRAM_read` operations
should certain data require storage for more than ~7-8 seconds ( in the case
of the DRAM chip this library was designed for )

An example of its usage can be seen below:

```Arduino
void setup()
{
  DRAM_setup(); //Setup pins for DRAM operations
}

void loop()
{
  mem_addr memory_addres = {.row = 0x077, 0x011}; //Declare memory address
  bool data_write = 0;  //Data to be written
  bool data_read;       //Storage for data read

  DRAM_write(memory_address, data_write); //Write data to address

  delay(7000);                //Delay read operation for 7 seconds
  refresh(memory_address);    //Refresh data at memory address
  delay(7000);                //Delay read for another 7 seconds

  data_read = DRAM_read(memory_address);  //Store data read in variable
}
