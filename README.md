# ray-tracer

Authors: Alex Epp (1487716), Mitchell Epp (1498821)
Section: LBL EA1

Hardware Setup
===========================================================

Accessories:
 * Arduino Mega board
 * Adafruit ILI9341 Breakout


Wiring instructions: the Adafruit ILI9341 was connected according to the wiring given on eclass, reproduced below:

    Board Pin <---> Arduino Pin
    ===========================
    GND             GND
    Vin             5V
    3Vo             NOT CONNECTED
    CLK             52
    MISO            50
    MOSI            51
    CS              10
    D/C             9
    RST             NOT CONNECTED
    Lite            NOT CONNECTED
    Y+              A2 (analog pin)
    X+              4  (digital pin)
    Y-              5  (digital pin)
    X-              A3 (analog pin)
    IM0 - IM3       NOT CONNECTED (they expect a 3.3v signal, DON'T CONNECT!)
    CCS             6
    CD              NOT CONNECTED

Building and Running
===========================================================

How to Build (Arduino)
  * Requires: avr-g++ (with c++ 14 support), arduino-ua (should include Adafruit_GFX and Adafruit_ILI9341)
  * Make sure the terminal is in the base folder. Type:
      make

How to Run (Arduino)
  * Make sure the terminal is in the base folder, and that the Arduino is connected to /dev/ttyACM0. Type
     make upload

How to Build (Linux)
  * Requires:
    *  SDL 2 (sudo apt-get install libsdl2-dev)
    *  CMake
  * Make sure the terminal is in the base folder. First, setup the makefile by typing:
      mkdir build-cmake
      cd build-cmake
      cmake ..
  * Now the project can be built with:
      make

How to Run (Linux)
  * From the build-cmake folder (see How to Build), type
      ./ray-tracer

How to save an Arduino render
  * Requirements:
    * pyserial (pip3 install pyserial)
    * progressbar2 (pip3 install progressbar2)
  * After building and running on the Arduino, navigate to the save-images subfolder, and type
      python3 save_image.py /dev/ttyACM0 filename
  * If the process does not begin saving immediately, press the reset button on the Arduino.
  * This will collect the image data from the Arduino via the Serial port, storing it for later

How to restore an Arduino render
  * Requirements:
      pyserial (pip3 install pyserial)
    * progressbar2 (pip3 install progressbar2)
  * Navigate to the save-images subfolder. Run
      make upload
  * Then type
      python3 load_image.py /dev/ttyACM0 filename
  * This will stream the image data to the Arduino, where it will display to the screen

File Structure
===========================================================

List of files and folders that should be present:
 * save-images
     |- display_image.cpp
     |- load_image.py
     |- save_image.py
     |- Makefile
  * application.h
  * array.h
  * camera.h
  * ILI9341framework.cpp
  * ILI9341framework.h
  * light.h
  * object.h
  * plane.cpp
  * plane.h
  * ray.h
  * raytracer.cpp
  * raytracer.h
  * reverse_sphere.h
  * sdlframework.cpp
  * sdlframework.h
  * sphere.cpp
  * sphere.h
  * transformation.h
  * util.cpp
  * util.h
  * vec.h
  * windowframework.cpp
  * windowframework.h
  * CMakeLists.txt
  * Makefile
  * README.md