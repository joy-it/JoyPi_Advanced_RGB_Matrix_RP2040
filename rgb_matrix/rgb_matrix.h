#ifndef rgb_matrix_h
#define rgb_matrix_h

/*necessary library to control RGB matrix*/
#include <Adafruit_NeoPixel.h>

/*constansts to for communictaion with matrix*/
#define PIX_NUM 64
#define MAX_BUF_LEN 128
#define MAX_STR_LEN 256
#define DATA_PACKAGE 32
#define MATRIX_PIN 6

/*enum to encode different functions*/
enum FunctionEnum : uint8_t {
  SHOW = 0,
  SETPIXELCOLOR = 1,
  FILL = 2,
  SETBRIGHTNESS = 3,
  GAMMA8 = 4,
  GAMMA32 = 5,
  NUMPIXEL = 6,
  COLORHSV = 7,
  CLEAR = 8,
  SENDDATA2SHOW = 9,
  SENDALLPIXRGB0 = 10,
  SENDALLPIXRGB1 = 11,
  SENDALLPIXRGB2 = 12,
  SENDALLPIXRGB3 = 13,
  SENDALLPIXRGB4 = 14,
  SENDALLPIXRGB5 = 15
};

/*method to convert seperate RGBW values to one value*/
static inline uint32_t mixRGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | ((uint32_t)b);
}

/*class to save relevant data into structure*/
class TransferData {
public:
  uint8_t funcNum;  // Function ID (see FunctionEnum)
  uint8_t pos;      // Pixel position index
  uint8_t r;        // Red color component (0–255)
  uint8_t g;        // Green color component (0–255)
  uint8_t b;        // Blue color component (0–255)
  uint8_t w;        // White color component (for RGBW LEDs)
  uint8_t c;        // Additional color or control parameter (device-dependent)
  uint8_t bright;   // Global brightness level
  uint8_t first;    // First pixel index for range operations
  uint8_t count;    // Number of pixels affected in range operations
  uint8_t data;     // primary data value for the I²C command packet
  uint8_t data1;    // secondary data value for the I²C command packet
};

/*class to send commands to matrix*/
class RGBMatrix {
public:
  RGBMatrix();
  void begin();
  void setRGBMatrixByPixel();
  void setPixelColor();
  void setBrightness();
  void fillMatrix();
  void clearMatrix();
  void showSendData();
  void updateMatrix();
  uint32_t getGamma8();
  uint32_t getGamma32();
  uint32_t getNumberOfPixel();
  uint32_t getColorHSV();
private:
  Adafruit_NeoPixel _strip;
};

//================== Global Variables ==================//
extern TransferData received_cmd;
extern uint8_t rgbMatrix[PIX_NUM * 3];
extern RGBMatrix matrix;

#endif