import serial
import sys
import progressbar

IMAGE_WIDTH = 320
IMAGE_HEIGHT = 240

try:
    port_name = sys.argv[1] if len(sys.argv) > 1 else 'COM3'
    file_name = sys.argv[2] if len(sys.argv) > 2 else 'img'

    arduino_args = [arg.encode('utf-8') for arg in sys.argv[3:]]
    
    with serial.Serial(port_name, 115200, timeout=1) as ser, open(file_name, 'wb') as file:
        # Send command-line arguments
        while ser.read() == 0:
            pass
        ser.write(bytes([len(arduino_args)]))
        for arg in arduino_args:
            while ser.read() == 0:
                pass
            ser.write(bytes([len(arg)]))
            ser.write(arg)


        bar = progressbar.ProgressBar()
        for x in bar(range(IMAGE_WIDTH)):
            for y in range(IMAGE_HEIGHT):
                # Wait for image data
                while True:
                    byte_read = ser.read()
                    if byte_read == b'\x02':
                        break
                    print(chr(byte_read))
                # Read in image data
                while True:
                    low = ser.read()
                    if low:
                        file.write(low)
                        break
                while True:
                    high = ser.read()
                    if high:
                        file.write(high)
                        break

        file.flush()
                

    print('Finished.')

except Exception as e:
    print(e)
except:
    print('\nException caught')
