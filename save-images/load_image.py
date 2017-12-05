import serial
import sys
import progressbar
import time

IMAGE_WIDTH = 320
IMAGE_HEIGHT = 240

BUFFER_SIZE = 60

try:
    port_name = sys.argv[1] if len(sys.argv) > 1 else 'COM3'
    file_name = sys.argv[2] if len(sys.argv) > 2 else 'img'
    with serial.Serial(port_name, 115200, timeout=1) as ser, open(file_name, 'rb') as file:

        with progressbar.ProgressBar(max_value=320*240*2) as bar:
            contents = bytearray(file.read())
            #with progressbar.ProgressBar(max_value=len(contents)) as bar:
            i = 0
            while i < len(contents):
                # Wait for next signal
                while True:
                    b = ser.read()
                    if b == b'd':
                        break
                # Write buffer
                ser.write(contents[i:i+BUFFER_SIZE])

                bar.update(i)
                i += BUFFER_SIZE


            #low = file.read(1)
            #high = file.read(1)
            #ser.write(low)
            #ser.write(high)

    print('Finished.')

except Exception as e:
    print('\nException:',e)
except:
    print('\nException caught')
