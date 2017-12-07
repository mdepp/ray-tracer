from PIL import Image
import sys

IMAGE_WIDTH = 320
IMAGE_HEIGHT = 240


def decodeColour(low, high):
    #try:
    col16 = ord(high)*256 + ord(low)
    #except:
    #    col16 = int(0)

    b = (col16 & 31) / 32
    g = ((col16 >> 5) & 63) / 64
    r = ((col16 >> 11) & 31) / 32
    return (int(r*255), int(g*255), int(b*255))


file_name = sys.argv[1] if len(sys.argv) > 1 else 'img'

img = Image.new('RGB', (IMAGE_WIDTH, IMAGE_HEIGHT), "black")
pixels = img.load()
with open(file_name, 'rb') as source:
    for i in range(img.size[0]):
        for j in reversed(range(img.size[1])):
            low = source.read(1)
            high = source.read(1)
            pixels[i, j] = decodeColour(low, high)

img.show(title=file_name)
