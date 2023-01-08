# TheWordClock_diy
## DIY Arduino LED Word Clock

![Alt text](img/WordClock2.jpg?raw=true "Title")


<br/>

## Contents 
 1. [Features](#features) - Wordclock features
 2. [Components](#components) - Components needed to make the Wordclock
 3. [Circuit](#circuit) - Circuit and wiring diagram for Wordclock electronics
 4. [Fabrication Process](#fabrication-process-step-by-step) - Step-by-step guide for making a Wordclock

&nbsp;

## Features

* LED colour change button
* On the hour and quarter hour LED animations 
* Birthday LED message on set dates
* Automatic LED brightness control via photoresistor
* Real time clock (RTC) time keeping to keep time without power
* Automatic daylight-saving time changes

&nbsp;


## Components


| Component             | Description                              | Source		              |Reference            | Quantity  |
|-----------------------|------------------------------------------|----------------------------------|---------------------|-----------|
|Frame		        | Box photo Frame 23x23 cm                 | Ikea	                      | Black RIBBA*        | 1         |
|Letter cut outs        | Laser cut panel 3mm birch plywood        | online laser cutting service     | 		    | 1	        | 
|Baking paper	        | 23 x23 sheet to diffuse LED light        | grocery store     		      | 		    | 1	        | 
|LED dividers	        | Dividers to isolate LED light            | online laser cutting services    | 1.5mm backing board | 1         |  
|LED array template     | Card template to attach LED strips       | online laser cutting services    | 1.5mm backing board | 1         |
|MDF board	        | 230 x 230 x3mm for back of clock         | Hardware store/ online	      | 3mm thickness	    | 1	        | 
|LED strip              | addressable LEDs 60/meter 12mm interval  | Amazon/AliExpress                | 60 IP30, WS2812BECO | 121 LEDs  |  
|Arduino nano           | Arduino/Elegoo nano                      | Amazon/AliExpress	              | ATmega328P	    | 1         |
|Real time clock (RTC)  | RTC with battery to keep time            | Amazon/AliExpress	              | DS3231		    | 1         |
|Bluetooth BLE module   | BLE Bluetooth for remote connection       | Amazon/AliExpress	              | HM-10		    | 1         |
|Push button (momentary)| button to cycle through LED colour       | Amazon/AliExpress	              | Tactile Push Button | 1         |
|Photoresistor          | measures ambient light to set brightness | Amazon/AliExpress	              | GL5516		    | 1         |
|Wire                   | wire to connect components together      | Amazon/AliExpress	              | 		    | <1 m      |
|Heat shrink tubing	| heat shrink to tidy soldered connections | Amazon					|		| 				|
|Soldering iron/solder  | soldering iron to solder all components  | Amazon/hardware store            |                     |           |
|Custom PCB	        | optional custom PCB to solder components | online PCB manufacturer          | 		    | 1         |
|Resistors 		| 1x330 1x1K, 1x2K, 2x10k 		   | Amazon/AliExpress	              | 		    | 5         |
|Capacitor              | 100uF 50V for power surge protection	   | Amazon/AliExpress	              | 		    | 1         |
|Power Jack		| 5.5 X 2.1mm DC power Jack 		   | Amazon/AliExpress	              | 		    | 1         |
|DC power supply	| 5Volts 3Amp  		  		   | Amazon		              | 		    | 1         |
|Mini USB-A cable	| Cable  used for Arduino power            | Amazon/AliExpress	              | 		    | 1         |
|Square wood dowel	| 10 x 10 x 30mm square dowel for backplate| Hardware store/ online	      | 		    | 4         |
|M2x10 micro screws	| flat head screws for attaching backplate | Hardware store/ online	      | 		    | 4         |
|Glue gun with glue	| glue gun to fix components 		   | Hardware store/ online	      | 		    | 1         |
|Drill bits and drill	| 12mm, 2mm, and 6mm drill bits and drill  | Hardware store/ online	      | 		    | 3         |

*The Ikea RIBBA frame in black has been discontinued. An oak version is still available, which could be spray painted. Alternative frames of the same size can be found on Etsy.
&nbsp;

## Circuit

#### Circuit components can be connected following the WordClock circuit

![Alt text](img/WordClock_circuit_diagram.jpg?raw=true "Title")

&nbsp;

#### Alternatively a Custom PCB can also be fabricated for ease of assembly.
PCB files found in TheWordClock_diy/CustomPCB

![Alt text](img/WordClock_Schematic.jpg?raw=true "Title")


&nbsp;

## Fabrication process step-by-step
The assembly process involves the following steps 
1. [Frame assembly](#1-frame-essembly)
2. [LED array and power input assembly](#2-led-array-and-power-input-essembely)
3. [Arduino circuit soldering](#3-arduino-circuit-soldering)
4. Button, photoresistor and power supply
6. Backplate assembly
7. Bringing it all together 

&nbsp;
 
#### 1. Frame assembly
- Discard all parts of the frame except for frame itself
- Take the wooden laser cut clock face and place it face down within the frame
- Cut out a piece of baking paper the same size as the clock face and place on top of the clock. 
> This will diffuse the LED light to better disperse the light for each letter. 
- Assemble the dividers and place on top of the baking paper/ clock face. 

![Alt text](img/Frame_essemble.jpg?raw=true "Title")

 &nbsp;

#### 2. LED array and power input assembly

**Power input assembly** 
- Take the USB cable and strip the wires to expose the 4 wires
- Trim off the two data wires (these are usually green and white) leaving just the power and ground wires
- Extend the power and ground wires, tidy up the connections to add more stability.
- With two additional pieces of wire solder the capacitor between the ground and power wire (see [circuit diagram](#circuit)) this will connect to and power the LED array. 
- Connect both sets of wires to the DC jack making sure the wires are securely in place.

 ![Alt text](img/powerJack.jpg?raw=true "Title")

&nbsp;

**LED array essembely**
- Cut the addressable LEDs to 11 LED strips.
- Solder the LED strips in a snake like orientation starting from the bottom left.
> The LEDs are directional so make sure they are solder in the correct direction. This soldering is the most time consuming part. Be patient and use soldering flux to help.
- Tidy up the soldering connections with some electrical tape or heat shrink tubing
- Get the LED array template 
- Using a glue gun add a little glue between each hole in the template and glue the LED strip to the template.
> Do this one strip at a time to prevent glue drying. Don't glue the first part of the first strip as we will need access to solder this later

 ![Alt text](img/LEDarray.jpg?raw=true "Title")

&nbsp;

#### 3. Arduino circuit soldering
Using either the custom PCB or manual wiring, connect the HM-10, DS3231 RTC and the Arduino nano together. 
If using manual wiring follow the [circuit diagram](#circuit) ensuring the correct resistors are included. 

> The custom PCB will save a lot of time and add more stability to the electronics so this is recommended
  
For assembly via the PCB
- Solder the Arduino nano, HM-10 and DS3231 RTC to the PCB along with the various resistors.
![Alt text](img/PCBSoldered.jpg?raw=true "Title")

