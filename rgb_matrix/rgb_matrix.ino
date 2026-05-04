#include "rgb_matrix.h"
#include <Wire.h>

/* Setup I2C connection with I2C adress*/
#define I2C_ADDRESS 0x66
#define I2C_SDA_PIN 20
#define I2C_SCL_PIN 21

/*variables to read data from I2C connection*/
uint8_t receivedData[MAX_BUF_LEN];
uint8_t localData[MAX_BUF_LEN];
volatile bool commandAvailable = false;
volatile uint8_t receivedLength = 0;

/* setup: Initialize I2C*/
void setup() {
  /* Set I2C bus as slave device */
  Wire.setSDA(I2C_SDA_PIN);
  Wire.setSCL(I2C_SCL_PIN);
  Wire.setClock(400000);
  Wire.begin(I2C_ADDRESS);

  /*I2C Peripheral methods*/
  Wire.onReceive(i2c_receive_handler);
  Wire.onRequest(i2c_request_handler);

  /* Setup object to control RGB matrix*/
  matrix.begin();
}

void loop() {
  /*check if data was received*/
  if (commandAvailable) {
    /*copy read data to local array and call matrix handler*/
    noInterrupts();
    memcpy(localData, receivedData, receivedLength);
    commandAvailable = false;
    interrupts();
    matrix_handler(localData);
  }
}

/*method which is executed if address receives data*/
void i2c_receive_handler(int numBytes) {
  /*check if data is empty*/
  if (numBytes <= 0) {
    return;
  }
  /*prepare to write data into array*/
  uint8_t index = 0;
  memset(&received_cmd, 0, sizeof(received_cmd));
  memset(receivedData, 0, MAX_BUF_LEN);

  /*write data to array*/
  while (Wire.available() && index < sizeof(receivedData)) {
    receivedData[index++] = Wire.read();
  }
  /*save length of data & set flag to process data*/
  receivedLength = index;
  commandAvailable = true;
}

/*method which is executed if address is requested to send data*/
void i2c_request_handler() {
  uint32_t value = 0;
  switch (received_cmd.funcNum) {
    case GAMMA8:
      value = matrix.getGamma8();
      break;
    case GAMMA32:
      value = matrix.getGamma32();
      break;
    case NUMPIXEL:
      value = matrix.getNumberOfPixel();
      break;
    case COLORHSV:
      value = matrix.getColorHSV();
      break;
    default:
      value = 0;
      break;
  }
  Wire.write(reinterpret_cast<uint8_t *>(&value), sizeof(value));
}

/*method to process received data*/
void matrix_handler(const uint8_t _receivedData[MAX_BUF_LEN]) {
  const uint8_t command = _receivedData[0];

  /*If SENDALLPIXRGB is the command, save data into rgbMatrix to the corresponding position*/
  if (command >= SENDALLPIXRGB0 && command <= SENDALLPIXRGB5) {
    uint8_t packetIndex = command - SENDALLPIXRGB0;
    uint16_t offset = packetIndex * DATA_PACKAGE;
    for (uint16_t i = 0; i < DATA_PACKAGE; i++) {
      rgbMatrix[offset + i] = _receivedData[i + 2];
    }
    /*all SENDALLPIXRGB are send - display instruction on RGB matrix*/
    if (command == SENDALLPIXRGB5) {
      matrix.setRGBMatrixByPixel();
    }
    return;
  }

  /*Start from the 3rd data byte. First two bytes in Python program: 1st=command byte, 2nd=parameter count (usually fixed at 0x0B)*/
  uint8_t index = 2;
  /*save date to class*/
  received_cmd.funcNum = _receivedData[index++];
  received_cmd.pos = _receivedData[index++];
  received_cmd.r = _receivedData[index++];
  received_cmd.g = _receivedData[index++];
  received_cmd.b = _receivedData[index++];
  received_cmd.w = _receivedData[index++];
  received_cmd.c = _receivedData[index++];
  received_cmd.bright = _receivedData[index++];
  received_cmd.first = _receivedData[index++];
  received_cmd.count = _receivedData[index++];
  received_cmd.data = _receivedData[index++];
  received_cmd.data1 = _receivedData[index++];

  /*execute commund corresponding to FunctionEnum*/
  switch (received_cmd.funcNum) {
    case SHOW:
      matrix.updateMatrix();
      break;
    case SETPIXELCOLOR:
      matrix.setPixelColor();
      break;
    case FILL:
      matrix.fillMatrix();
      break;
    case SETBRIGHTNESS:
      matrix.setBrightness();
      break;
    case CLEAR:
      matrix.clearMatrix();
      break;
    case SENDDATA2SHOW:
      matrix.showSendData();
      break;
    default:
      break;
  }
}