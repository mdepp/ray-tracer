#!/bin/sh

cd .. && make upload args="-sc 1 -r 1" && cd save-images && python3 save_image.py /dev/ttyACM0 minimal && python3 translate_image.py minimal
cd .. && make upload args="-sc 0 -r 5" && cd save-images && python3 save_image.py /dev/ttyACM0 shadows && python3 translate_image.py shadows
cd .. && make upload args="-sc 2 -r 5" && cd save-images && python3 save_image.py /dev/ttyACM0 straight && python3 translate_image.py straight
cd .. && make upload args="-sc 3 -r 5" && cd save-images && python3 save_image.py /dev/ttyACM0 ring && python3 translate_image.py ring
