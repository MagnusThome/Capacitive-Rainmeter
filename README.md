# Capacitive-Rainmeter
Measure rain intensity in Homeassistant with a capacitive sensor 

The code uses mqtt to send data and also for self-registering the sensor in Homeassistant. 

## Sensor:

__Capacitive Rain Sensor RC-SPC1K__  
https://radiocontrolli.eu/Capacitive-Rain-Sensor-RC-SPC1K-p242943346

<img align=right width=16% src=https://github.com/MagnusThome/Capacitive-Rainmeter/assets/32169384/3e2ffd15-967a-4578-a91d-5cff18d661b9>
Why a capacitive sensor? I've previously used those common resistive sensors that have copper traces that the rain falls on. The problem is that these boards tend to deteriorate after a while due to oxidization. There are tricks to minimize the problem, reducing the current and the duration the current is flowing, but in the end you still have metal and electrical current in a harsh environment. A capacitive sensor is totally electrically isolated from the rain and damp. By having no external electrically connected sensor parts the main problems with wear in an outdoor environment are removed. A bonus I found with the capacitive sensor I use is that it reacts much faster to changes in rain intensity compared to the resistive one I had previously. The new sensor also has a built in heater and an NTC resistor which means I can heat it up and control the temperature precisely during the winter. This is a clear plus in snowy Sweden.  
<br><br>   
The sensor's heater runs on 12V so that is the voltage you need to supply to your build. Any ESP32 board will do but since you need to run this on 12V you normally need a DC-DC buck converter to power the ESP32 at 5V. But these converters are cheap and easy to find. I happened to have an ESP32 board that can run on 12V natively so I just had to add a few components to interface it with the rain sensor. A basic schematic is included below on how to connect the different GPIOs. Also see the config.h file where you can specify what goes to what pin on your board.   
<br><br>   
<img align=right width=50% src=https://github.com/MagnusThome/Capacitive-Rainmeter/assets/32169384/79b76135-ba25-49dc-8c92-0bc4a8b4002f>
The code as it is now keeps the sensor at around 45 degrees Celsius by sensing the NTC resistor value and turning the heater on and off (with a transistor) quite quickly around a small temperature hysteresis. Yeah, just how temperature control has been done since the stone age. I don't know if I've set it to its optimal temperature, to for example keep snow melting and also drying off rain not too fast and not too slow. I have a sensor installed and I log it in Homeassistant and I might tweak the code during the winter.
<br><br>  
The measurement of the capacitance is done by first making sure the capacitor/sensor is fully discharged by pulling the GPIO pin connected to it to ground. Then that pin is turned to a high impedance input and another pin goes high and starts charging the capacitor slowly through a 1Mohm resistor while the first pin is measuring the rising voltage directly on the capacitor. When the voltage reaches 63% of the full voltage the time between the start and stop of this charging sequence is saved. The larger the capacitance, when raining, the longer the charging time. 
<br><br>  
Parts used:  
  
- NPN transistor (I used a 2N2222A because I had one and it can handle the current of a bit under one ampere)  
- Two 1Kohm resistors  
- One 1Mohm resistor  
<br clear=all><br>

![build](https://github.com/MagnusThome/Capacitive-Rainmeter/assets/32169384/30b21fa1-9477-4a0a-895d-b27a03148101)

![image](https://github.com/MagnusThome/Capacitive-Rainmeter/assets/32169384/e9ea9603-9da6-4906-bcef-7d26ac795914)
  
![diagram](https://github.com/MagnusThome/Capacitive-Rainmeter/assets/32169384/59e8d16f-f9e6-4c7c-a9ad-352ebda1ae79)
    
  
## Things to find out

- Should the sensor be mounted 100% horizontal or should it be tilted slightly? Will how it is mounted change if rain accumulates and stays long after it has stopped raining? Or does the heater handle that with my currently set temperature? Currently it is mounted a bit slanted and it seems to work very well..

- What temperature is best for the sensor's built in heater to run at? I guess the optimal is when it is warm enough to help dry off rain so when the rain stops the sensor will report "no rain" in a decent amount of time?

- The reported value seems a bit unlinear to what I see as heavy rain and drizzle and everything in between. But I have added some code for this, it "feels" quite reasonable so far but it might need some tweaking down the line?
   
## Schematic
  
![image](https://github.com/MagnusThome/Capacitive-Rainmeter/assets/32169384/85a5f1c8-7c96-44b4-8671-84539ff1c8dd)  
  
  
<hr>
