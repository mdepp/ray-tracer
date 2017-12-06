#include <Arduino.h>
#include <Adafruit_ILI9341.h>

const uint32_t IMAGE_WIDTH = 320;
const uint32_t IMAGE_HEIGHT = 240;

uint16_t encodeColour(float fr, float fg, float fb)
{
    uint16_t r = constrain(fr * 31, 0, 31);
    uint16_t g = constrain(fg * 63, 0, 63);
    uint16_t b = constrain(fb * 31, 0, 31);
    return (r << 11) + (g << 5) + b;
}

const uint32_t BUFFER_SIZE = 60;

int main()
{
    init();
    Serial.begin(115200);

    Adafruit_ILI9341 tft(10,9);
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(encodeColour(0.f, 0.f, 0.f));

    Serial.write('b');

    for(uint32_t i=0; i<IMAGE_WIDTH*IMAGE_HEIGHT; i += BUFFER_SIZE/2)
    {
        Serial.write('d');
        while (Serial.available() < (int64_t)BUFFER_SIZE)
        {
            //delay(1000);
            //tft.fillScreen(encodeColour((rand()%255)/255.f, (rand()%255)/255.f, (rand()%255)/255.f));
        }
        for(uint32_t k=0; k<BUFFER_SIZE/2; ++k)
        {
            uint32_t x = (i+k)/IMAGE_HEIGHT;
            uint32_t y = (i+k)%IMAGE_HEIGHT;
            uint32_t low = Serial.read();
            uint32_t high = Serial.read();

            tft.drawPixel(x, IMAGE_HEIGHT-1-y, high*256ul + low);
        }
    }
/*
    for(int x=0; x<IMAGE_WIDTH; ++x)
    {
        for(int y=0; y<IMAGE_HEIGHT; ++y)
        {
            Serial.print('d');

            uint8_t lowByte, highByte;
            while (Serial.available() <= 0);
            lowByte = Serial.read();
            while (Serial.available() <= 0);
            highByte = Serial.read();

            uint16_t col = uint16_t(highByte)*256ul + uint16_t(lowByte);
            if (col != 0)
                tft.drawPixel(x, y, col);
            else
                tft.drawPixel(x, y, encodeColour(1.f,1.f,1.f));
        }
    }*/
    return 0;
}
