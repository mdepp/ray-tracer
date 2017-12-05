import serial
import sys
import progressbar

IMAGE_WIDTH = 320
IMAGE_HEIGHT = 240

try:
    port_name = sys.argv[1] if len(sys.argv) > 1 else 'COM3'
    file_name = sys.argv[2] if len(sys.argv) > 2 else 'img'

    with serial.Serial(port_name, 115200, timeout=1) as ser, open(file_name, 'wb') as file:
        bar = progressbar.ProgressBar()
        while True:
            if ser.in_waiting > 0:
                if ser.read() == b'\x03':
                    break
        for x in bar(range(IMAGE_WIDTH)):
            for y in range(IMAGE_HEIGHT):
                # Wait for image data
                while True:
                    byte_read = ser.read()
                    if byte_read == b'\x02':
                        break
                    print(byte_read.decode('utf-8', errors='replace'), end='')
                # Read in image data
                while ser.in_waiting < 2:
                    pass
                file.write(bytes(ser.read()))  # low
                file.write(bytes(ser.read()))  # high

        file.flush()


    print('Finished.')

except Exception as e:
    print(e)
except:
    print('\nException caught')
