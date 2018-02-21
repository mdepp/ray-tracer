# Line 1035 of /usr/share/arduino/Arduino.mk *appends* to CXXFLAGS, so
# so changes here carry over to Arduino.mk
# Enable c++ 14, indicate build is for arduino, pass in command-line arguments
CXXFLAGS += -std=gnu++14 -D ARDUINO_BUILD -D ARDUINO_ARGS="$(args)"

# Force rebuild of config.h if args is different than last time
RE:=$(shell sh update_config.sh "$(args)")


BOARD_TAG = mega2560

ARDUINO_LIBS = SPI Adafruit_GFX Adafruit_ILI9341
USER_LIB_PATH = libraries

include /usr/share/arduino/Arduino.mk