# JoyPi_Advanced_RGB_Matrix_RP2040
Software for RP2040 on Joy-Pi Advanced with the RGB matrix

>[!NOTE]
> This code has dependencies to [Adafruit Neopixel](https://github.com/adafruit/Adafruit_NeoPixel).

## Pinout
| Function             | GPIO    | Description                                     |
| -------------------- | ------- | ----------------------------------------------- |
| **Pin to matrix**     | GPIO 6  | LED matrix signal                       |
| **I2C SDA**          | GPIO 20 | I2C data line                                   |
| **I2C SCL**          | GPIO 21 | I2C clock line                                  |


## I2C Communictaion Protocol
| Byte Offset | Field     | Type | Description                        |
| ----------- | --------- | ---- | ---------------------------------- |
| 0           | Command   | `uint8_t`   | Reserved/Command prefix            |
| 1           | ParamCnt  | `uint8_t`   | Parameter count (fixed at 0x0B)    |
| 2           | `funcNum` | `uint8_t`   | Function code (see table below)    |
| 3           | `pos`     | `uint8_t`   | LED position index (0-63)          |
| 4           | `r`       | `uint8_t`   | Red component (0-255)              |
| 5           | `g`       | `uint8_t`   | Green component (0-255)            |
| 6           | `b`       | `uint8_t`   | Blue component (0-255)             |
| 7           | `w`       | `uint8_t`   | White component (0-255)            |
| 8           | `c`       | `uint8_t`   | Mixed color parameter              |
| 9           | `bright`  | `uint8_t`   | Global brightness (0-255)          |
| 10          | `first`   | `uint8_t`   | Starting pixel (for FILL function) |
| 11          | `count`   | `uint8_t`   | Pixel count (for FILL function)    |
| 12          | `data`    | `uint8_t`   | Extended data / Interrupt flag     |
| 13          | `data1`   | `uint8_t`   | Extended data 2                    |


## Function Encoding
Function Codes (FunctionEnum)
| Code    | Name               | Description                                         |
| ------- | ------------------ | --------------------------------------------------- |
| `0`     | `SHOW`             | Refresh display (push RAM data to LEDs)             |
| `1`     | `SETPIXELCOLOR`    | Set single pixel color                              |
| `2`     | `FILL`             | Fill a range of pixels with color                   |
| `3`     | `SETBRIGHTNESS`    | Set global brightness                               |
| `4`     | `GAMMA8`           | Read Gamma8 correction table value (Request)        |
| `5`     | `GAMMA32`          | Read Gamma32 correction table value (Request)       |
| `6`     | `NUMPIXEL`         | Read LED count (Request)                            |
| `7`     | `COLORHSV`         | Read HSV to RGB conversion value (Request)          |
| `8`     | `CLEAR`            | Clear all pixels                                    |
| `9`    | `SENDDATA2SHOW`    | Bitmap mode display (8×8 matrix control)            |
| `10-15` | `SENDALLPIXRGB0-5` | Bulk pixel data block transfer (32 bytes per block) |

### Bulk Pixel Data Transfer
For SENDALLPIXRGB0 through SENDALLPIXRGB5 (codes 10-15):
> Each command transmits 32 bytes of RGB data
> Total of 6 blocks, accumulating 192 bytes (64 pixels × 3 colors)
> Upon receiving SENDALLPIXRGB5, the entire matrix automatically refreshes

## File Structure
JoyPi_Advanced_RGB_Matrix_RP2040/
├── rgb_matrix.h                 # Global header: macros, data structures, definitions
├── matrix_functions.cpp         # Control RGB matrix: methods to use teh Adafruit Neopixel code
└── rgb_matrix.ino               # Main program: initialization, setup of i2c handlers

## Key Components
- `void i2c_receive_handler(int numBytes)` > Method to save data from I2C communication
- `void i2c_request_handler()` > Method to write requested value to communication
- `void matrix_handler(const uint8_t _receivedData[MAX_BUF_LEN])` > Method to execute other methods to manipulate matrix depending on `funcNum`
- `FunctionEnum` > Enum holds the encoding of the different function which are possible execute
- `TransferData` > Class to hold all relevant read data from I2C communication 
- `RGBMatrix` > Class to manipulate matrix and uses `Adafruit Neopixel`