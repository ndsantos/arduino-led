# Wardrobe Led Strip

## Goal

The project's goal is to have a led strip on the wardrobe that turns in when the doors open. The closet has six doors, and we will add a sensor to each door managed by a NodeMCU wifi module which allows the configuration of several properties, including RGB colour and brightness.

## Hardware

To complete the project, we'll need the following hardware:
- NodeMCU ESP8266
- Led strip WS8211
- Resistor 220 ohm
- 6x Door sensor
- 12V Power supply
- 3.3V Power supply

The circuit is straightforward, and the NodeMCU provide enough IO ports so we can attach the sensors directly to it. I'm using D4 to connect the led strip with a resistor for stabilisation. I've connected the door sensors
 to ports D0, D1, D2, D3, D5 and D6.

 ![Circuit schema](https://github.com/ndsantos/wardrobe-led-strip/blob/main/doc/schema.png?raw=true)

## Software
To program the NodeMCU, I'm using the Arduino IDE and the FastLED library to manage the led strip. For the WebServer, I'll use the ESPAsyncWebSrv, both available on Arduino's Library Manager.

Be aware that I've hidden my wifi details in a file not shared in the repo. For your privacy, you should do the same and hide your network details similarly. Here's my post on how to do it: https://www.ndsantos.com/index.php/2023/04/27/hiding-critical-information-in-arduino-projects-protecting-your-wifi-ssid-with-secrets-h/
