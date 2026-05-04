#include "rgb_matrix.h"

TransferData received_cmd;
uint8_t rgbMatrix[PIX_NUM * 3];
RGBMatrix matrix;

/*constructor sets up strip to communicate with matrix*/
RGBMatrix::RGBMatrix()
  : _strip(PIX_NUM, MATRIX_PIN, NEO_GRB + NEO_KHZ800) {}

/*start matrix connection*/
void RGBMatrix::begin() {
  _strip.begin();
  _strip.setBrightness(40);
  _strip.clear();
  _strip.show();
}

/*method to set matrix with all values from SENDALLPIXRGB*/
void RGBMatrix::setRGBMatrixByPixel() {
  for (int j = 0; j < PIX_NUM; j++) {
    _strip.setPixelColor(j, rgbMatrix[j * 3 + 0], rgbMatrix[j * 3 + 1], rgbMatrix[j * 3 + 2]);
  }
  _strip.show();
  memset(rgbMatrix, 0, sizeof(rgbMatrix));
}

/*set a pixel at a specific location to a specific colour*/
void RGBMatrix::setPixelColor() {
  if (received_cmd.c) {
    uint32_t mixed_colours = mixRGBW(received_cmd.r, received_cmd.g, received_cmd.b, received_cmd.w);
    _strip.setPixelColor(received_cmd.pos, mixed_colours);
    _strip.show();
  } else {
    _strip.setPixelColor(received_cmd.pos, received_cmd.r, received_cmd.g, received_cmd.b, received_cmd.w);
    _strip.show();
  }
}

/*set whole matrix to a specific colour*/
void RGBMatrix::fillMatrix() {
  uint32_t mixed_colours = mixRGBW(received_cmd.r, received_cmd.g, received_cmd.b, received_cmd.w);
  _strip.fill(mixed_colours, received_cmd.first, received_cmd.count);
  _strip.show();
}

/*set brightness level of matrix*/
void RGBMatrix::setBrightness() {
  _strip.setBrightness(received_cmd.bright);
  _strip.show();
}

/*turn matrix off*/
void RGBMatrix::clearMatrix() {
  _strip.clear();
  _strip.show();
}

/*set all marked pixels to a specific colour*/
void RGBMatrix::showSendData() {
  uint8_t r = received_cmd.r;
  uint8_t g = received_cmd.g;
  uint8_t b = received_cmd.b;
  /* Check each member value, if non-zero (has data), display the pixel */
  const uint8_t rows[8] = {
    received_cmd.pos,
    received_cmd.w,
    received_cmd.c,
    received_cmd.bright,
    received_cmd.first,
    received_cmd.count,
    received_cmd.data,
    received_cmd.data1
  };
  for (uint8_t row = 0; row < 8; row++) {
    uint8_t mask = rows[row];
    if (mask == 0) {
      continue;
    }
    for (uint8_t col = 0; col < 8; col++) {
      if (mask & (1 << col)) _strip.setPixelColor(row * 8 + col, r, g, b, 255);
    }
  }
  _strip.show();
}

/*update send data on matrix*/
void RGBMatrix::updateMatrix() {
  _strip.show();
}

/*return gamma8 value*/
uint32_t RGBMatrix::getGamma8() {
  return _strip.gamma8(received_cmd.data);
}

/*return gamma32 value*/
uint32_t RGBMatrix::getGamma32() {
  return _strip.gamma32(received_cmd.data);
}

/*return number of pixel with which the matrix is initialized*/
uint32_t RGBMatrix::getNumberOfPixel() {
  return _strip.numPixels();
}

/*return Color HSV value*/
uint32_t RGBMatrix::getColorHSV() {
  return _strip.ColorHSV((uint16_t)received_cmd.data);
}
