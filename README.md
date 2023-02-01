# Morse Code Demo for Pico W



### Requirements:

1. Arduino 2.X (For editing the code and uploading the sketch)
2. Arduino 1.X (For uploading files to LittleFS)
3. A Raspberry Pi Pico W/WH
4. Arduino-Pico Board/Libraries (https://arduino-pico.readthedocs.io/en/latest/)
5. Pico LittleFS Upload Plugin for Arduino 1.X (https://github.com/earlephilhower/arduino-pico-littlefs-plugin)
6. ArduinoJson Library (https://arduinojson.org/)
7. ArduinoWebSockets that supports Pico W (https://github.com/earlephilhower/arduinoWebSockets/tree/picow)
8. Button/Switch/Key between GND and GPIO Pin 13 (https://www.amazon.com/dp/B01MT3T676)
9. Tone Beeper on GPIO Pin 14 (https://www.amazon.com/dp/B075PT19J2)

### Installation:

1. Install both versions of Arduino
2. Install the Arduino-Pico Board/Libraries according to directions in link.
3. Install Pico LittleFS Upload Plugin according to directions in link.
4. Install ArduinoJson Library according to directions in link.
5. Download ArduinoWebSockets to your local Documents/Arduino/libraries folder
6. Connect your Pico W and load up Arduino 1.X
   1. Download this Repo and open the .INO
   2. Set the board to Raspberry Pi Pico W
   3. Set the port to the serial port of the Pico W
   4. Run the Pico LittleFS Data Upload Utility to upload the files from the data folder onto the Pico W
   5. After it's uploaded, close Arduino 1.X
7. Open Arduino 2.X
   1. Open the .INO
   2. Set the Board to Pico W
   3. Set the Port to the Pico
   4. Compile and Upload the sketch.

### Usage:

- Once the sketch and files are uploaded, if you plug in the pico to power, you should see a CW DEMO access point.
- Connect to this access point and go to the IP that it showed in the serial output
- Start trying to use the key to send morse code.

### Hardware Example:

![](https://github.com/nledevil/ArduinoMorseCodeDemoPicoW/blob/master/hardwareImages/IMG_4847.JPG?raw=true)