# TheWordClock_diy
## DIY Arduino LED WordClock

![Alt text](img/WordClock2.jpg?raw=true "Title")


<br/>

## Contents 
 1. [Features](#features) - WordClock features
 2. [Components](#components) - Components needed to make the WordClock
 3. [Circuit](#circuit) - Circuit and wiring diagram for WordClock electronics
 4. [Fabrication Process](#fabrication-process-step-by-step) - Step-by-step guide for making a WordClock
 5. [Setting the time and data via Bluetooth](#bluetooth-guide-to-follow) 

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
1. [Frame assembly](#1-frame-assembly)
2. [LED array and power input assembly](#2-led-array-and-power-input-assembly)
3. [Arduino circuit soldering](#3-arduino-circuit-soldering)
4. [Button, photoresistor and assembly](#4-push-button-and-photoresistor-assembly)
6. [Backplate fabrication and final assembly](#backplate-fabrication-and-final-assembly)
7. [Loading the code onto the Arduino](#loading-the-code-onto-the-arduino)

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

**LED array assembly**
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

&nbsp;

#### 4. Push button and photoresistor assembly
The colour of the LEDs in the WordClock can be altered using a push button attached to the back of the clock. in addition the brightness of the WordClock is automatically adjusted via a photoresistor which reads the brightness of the environment.

&nbsp;

**making the brightness photoresistor**
- Solder two wires to the photoresistor and tidy the solder contacts with electrical tape or heat shrink tubing.
- Using a drill, drill a hole slightly larger than the photoresistor diameter
> To prevent damage to the top of the clock, stick some tape over the drilling area.
- using a glue gun, glue the photoresistor into the drilled hole ensuring it is flush with the top of the clock and securely in place. 

![Alt text](img/Photoresistor.jpg?raw=true "Title")

&nbsp;

**making the push button**
- Solder a small push button to two wired, tidying the connections with heat shrink/ tape.
- Make sure the wires are quite long so that when attached to the Arduino the back plate can be removed
- *optional* solder two jump cable female connectors to the wires so that it can be easily removed from the main assembly. Then solder two male header pins to two wires that will be then soldered to the Arduino

![Alt text](img/Pushbutton.jpg?raw=true "Title")

&nbsp;

#### 5. Backplate fabrication and final assembly

**Backplate assembly**
- If no pre-cut backplate is available cut a 2-3mm piece MDF or plywood to the same dimensions as the front plate (23x23cm)
- Using a large drill bit (12 mm) cut a hole at the centre bottom of the backplate. Try to align this with where the DC jack will be glued to the inside of the frame.
- Using a smaller drill bit drill a hole the same size as the pushbutton button head.
- Using a glue gun, glue the button assembly to the inside of the backplate, ensure that the button mechanism isn’t glued. 
- Cut some small pieces of square/circle dowel (1cm diameter) and glue them to the inside corners of the frame.
> These should be cut to a length that when the backplate rests on them it is flush with the back of the frame.
- Place the backplate onto the back of the frame and using a very small drill bit drill holes through the base plate and the dowels. 
> These holes should be slightly smaller than the micro screws that will be used to fix the baseplate.

&nbsp;

**Final assembly**

- Using a glue gun, glue the DC Jack to the bottom of the frame, making sure it aligns with the hole in the backplate.
- Solder the photoresistor and the button to the Arduino/ PCB
- Solder the power and ground wires from the DC Jack to the first LED strip and at the same time as solder a data and ground wire from the LED strip. Tidy up the connections with heat shrink or tape
- Solder the data and ground wire from the LED strip to the Arduino / PCB.  

![Alt text](img/Backplate.jpg?raw=true "Title")


#### 6. Loading the code onto the Arduino
The Arduino code (sketches) needed for the WordClock can be found [here](Arduino_sketches/). These include the [HM-10 bluetooth settings](Arduino_sketches/HM10_bluetooth_settings/HM10_bluetooth_settings.ino), The [main WordClock sketch](Arduino_sketches/WordClock_Main/WordClock_Main.ino) and a trouble shooting [Demo real](Arduino_sketches/DemoReel100/DemoReel100.ino) sketch that can be used to check if all the LEDs work. 

These sketches can be loaded onto the Arduino nano using the [Arduino IDE](https://www.arduino.cc/en/software)

&nbsp;


**Changing the bluetooth settings**

The Bluetooth module can be used to set the time and date of the clock via a Bluetooth terminal app. You can check that the Bluetooth module works by running this sketch on the Arduino and monitor the outcome on the Arduino IDEs serial monitor.
- This code will change the name of the BLE module from its default "hmsoft" to "WordClock". 
- If you would like to change the name of the Bluetooth module you can edit the following line in the [HM-10 bluetooth sketch](Arduino_sketches/HM10_bluetooth_settings/HM10_bluetooth_settings.ino)


'https://github.com/mattudakis/TheWordClock_diy/blob/12852df04a07dd08134136e3b312919e72e92018/Arduino_sketches/HM10_bluetooth_settings/HM10_bluetooth_settings.ino#L134'

Changing the `"AT+NAMEWordClock"` to `"AT+NAMEinsertname"`

- Troubleshooting: If no BLE devices are detected in the Arduino IDE serial monitor, make sure the baud rate of the serial monitor is correct, make sure the HM-10 is wired to the correct pins on the Arduino. 
- If both of these still don’t fix the problem you may have a fault HM-10 module which you can replace. 

&nbsp;


**Set the correct time in the WordClock sketch**

To set the correct time without having to use Bluetooth the [main WordClock sketch](Arduino_sketches/WordClock_Main/WordClock_Main.ino) can be edited and loaded onto the Arduino.

 First uncomment the following lines of code changing the date and time to the correct values.
'https://github.com/mattudakis/TheWordClock_diy/blob/12852df04a07dd08134136e3b312919e72e92018/Arduino_sketches/WordClock_Main/WordClock_Main.ino#L106-L108'

Load this sketch onto the Arduino, this will set the time on the RTC. It is important that these lines are re-commented out and the sketch reload to the Arduino, otherwise the time will reset to this time every time the power is cycled.

- If the time/ date need changing in the future these can be altered via Bluetooth, using these [guides](#bluetooth-guide-to-follow) 


&nbsp;


**Altering the dates for birthdays in the WordClock sketch**

The WordClock will light up a 'Happy Birthday' message on set dates this can be edited via the following code in the [main WordClock sketch](Arduino_sketches/WordClock_Main/WordClock_Main.ino)

'https://github.com/mattudakis/TheWordClock_diy/blob/12852df04a07dd08134136e3b312919e72e92018/Arduino_sketches/WordClock_Main/WordClock_Main.ino#L219-L226'

- Change the month and day value and add additional dates with an additional or `||` statement.
- If you'd prefer this can be commented out if you don’t want a Birthday message to show.


&nbsp;


**Finished**

- After loading the main WordClock sketch onto the Arduino replace the USB with the power USB.
- Attach the backplate to the clock with micro screws, plug the WordClock in and check that the colour change button and brightness sensor work.
- Finished!




## Bluetooth guide to follow

 