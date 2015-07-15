# Ard-Roboteq
Library to Use Roboteq Motor Controller [AX2550](https://www.roboteq.com/index.php/docman/ax-documents-files/ax-documents-1/ax-userman-1/ax25xx-user-manual/88-ax2550-user-manual/file) using Arduino.

### Note:

You must have the library for **Message** installed, you can found it [here](https://github.com/Azrrael-exe/ArduinoMSG)


## Installation

Install it as an usual Arduino Librarie, as described in [here](https://www.arduino.cc/en/guide/libraries).

## Usage
To create an object you need to use a Hardware Serial

`AX2550 trac = AX2550(Serial1)`

To Start the device properly have to use the method `init` and define the id of each channel inside the messages.

`trac.init(id_A, id_B);`

After that 2 options can be use to move the Motor controller:

Send directly the values for each channel using this:

`trac.move(int8_t A, int8_t B, bool confirmation);`

Or using `messages` with the IDs for each channel.

`trac.move(message, bool confirmation);`


If you want to use the reads in `message` format, you need  to setup the ids for the measures from the controller use:

`trac.set_report(id_v,id_l,id_r);`

After that you can use the method `get_report`to get all the reads in `message` format with his respective IDs.

`Message msg = trac.report();`

To get each value separately use the methods:

`float Vol = trac.volts();` 

`int amps = trac.amps(channel);`

## Credits

Tecnologias Marte.SAS