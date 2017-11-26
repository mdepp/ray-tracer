import serial
import sys
import progressbar

IMAGE_WIDTH = 320
IMAGE_HEIGHT = 240

try:
    port_name = sys.argv[1] if len(sys.argv) > 1 else 'COM3'
    file_name = sys.argv[2] if len(sys.argv) > 2 else 'img'
    with serial.Serial(port_name, 9600, timeout=1) as ser, open(file_name, 'wb') as file:
        bar = progressbar.ProgressBar()
        for x in bar(range(IMAGE_WIDTH)):
            for y in range(IMAGE_HEIGHT):
                low = file.read()
                high = file.read()
                ser.write(low)
                ser.write(high)

    print('Finished.')

except Exception as e:
    print(e)
except:
    print('\nException caught')