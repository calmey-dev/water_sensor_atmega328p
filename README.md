# Water level sensor on ATmega328p

## Pinout
![image](https://github.com/calmey-dev/water_sensor_atmega328p/assets/57754989/2a632bef-d81e-45e4-92df-e2860e64e795)

## How to use it
- Assemble the device as shown above.
- Download the .hex file and flash your microcontroller.
- Enjoy.

## Problems
Parameters from the sensor to the sensor are different. If you want to achieve good accuracy you should measure minimum and maximum output voltage (dry sensor and dived into the water on a full length). After that open "main.c" file and change the next variables:
- minSignalValue = (Vmin * 1024) / Vref (Vref = 5V).
- maxSignalValue = (Vmax * 1024) / Vref.

## Mock-up
![image](https://github.com/calmey-dev/water_sensor_atmega328p/assets/57754989/a16406c4-1463-4238-842b-a7547fee3f84)
