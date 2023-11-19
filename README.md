# Capacitive-Rainmeter
Measure rain intensity in Homeassistant with a capacitive sensor 

The code uses mqtt to send data and also self register the sensor in Homeassistant. 

## Sensor:

__Capacitive Rain Sensor RC-SPC1K__  
https://radiocontrolli.eu/Capacitive-Rain-Sensor-RC-SPC1K-p242943346

The sensor heater runs on 12V so that is the voltage you need to supply. Any ESP32 board will do but since you need to run this on 12V you normally need a DC-DC buck converter to power the ESP32 at 5V. But these converters are cheap and easy to find. I happened to have an ESP32-S3 board that can run on 12V natively so I just added a few components to interface with the rain sensor. A true high tech schematic is included below on how to connect the different GPIOs, also see the config.h file where you can specify what goes where. 

The code as it is now keeps the sensor at around 45 degrees Celsius. I don't know if that's the optimal temperature to for example keep snow melting and dry off rain not too fast and not too slow. I have a sensor installed and log it in Homeassistant and I might tweak the code during the winter.

Why a capacitive sensor? I've previously used those common resistive sensors that have copper traces that the rain falls on. The problem is that these boards tend to wear down after a while due to oxidization. There are tricks to minimize the problems, reducing the current and the time current is flowing, but in the end you still have metal and electrical current in a harsh environment. But a capacitive sensor is totally electrically isolated from the rain and dampness. No external parts of metal. This removes problems with wear or tear in an outdoor environment.   

## Things to find out

- Should the sensor be mounted 100% horizontal or should it be tilted slightly? Will how it is mounted change if rain accumulates and stays long after it has stopped raining? Currently it is mounted a bit slanted and it seems to work very well..

- What temperature is best for the sensor's built in heater to run at? I guess the optimal is when it is warm enough to help dry off rain so when the rain stops the sensor will report "no rain" in a decent amount of time?

- The reported value seems a bit unlinear to what I see as heavy rain and drizzle and everything in between. But I have added some code for this, it "feels" quite reasonable so far but it might need some tweaking down the line?


![kit](https://github.com/MagnusThome/Capacitive-Rainmeter/assets/32169384/79b76135-ba25-49dc-8c92-0bc4a8b4002f)

![image](https://github.com/MagnusThome/Capacitive-Rainmeter/assets/32169384/e9ea9603-9da6-4906-bcef-7d26ac795914)

![arm closed](https://github.com/MagnusThome/Capacitive-Rainmeter/assets/32169384/fd34311f-c391-4b5f-9df3-93f9d417c394)

![arm open](https://github.com/MagnusThome/Capacitive-Rainmeter/assets/32169384/7e2d9be2-ed48-42fb-946f-4e2924a92be1)

![esp32-box](https://github.com/MagnusThome/Capacitive-Rainmeter/assets/32169384/ed867767-c6a1-400a-98c2-c41546993336)

![pcb top](https://github.com/MagnusThome/Capacitive-Rainmeter/assets/32169384/10883a02-48e6-4aa7-8237-53d67ebee8c7)


Transistor is a 2N2222A because I had one.

![schema](https://github.com/MagnusThome/Capacitive-Rainmeter/assets/32169384/0baf334a-89e0-46e6-a923-def7d31eb94a)
