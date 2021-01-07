#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define MATRIXPIN 13
#define MICPIN 34

BluetoothSerial SerialBT;
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, MATRIXPIN,
                            NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT +
                            NEO_MATRIX_ROWS   + NEO_MATRIX_PROGRESSIVE,
                            NEO_GRB           + NEO_KHZ800);

int off[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

int openMouthBig[8][8] = {
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0}
};

int openMouthMedium[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

int openMouthSmall[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

int mouthClosed[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

int smile[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {2, 0, 0, 0, 0, 0, 0, 2},
  {2, 2, 0, 0, 0, 0, 2, 2},
  {0, 2, 2, 2, 2, 2, 2, 0},
  {0, 0, 2, 2, 2, 2, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

int sad[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 3, 3, 3, 3, 0, 0},
  {0, 3, 3, 3, 3, 3, 3, 0},
  {3, 3, 0, 0, 0, 0, 3, 3},
  {3, 0, 0, 0, 0, 0, 0, 3},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

const uint16_t samples = 2560;
float vol = 0.0;
uint16_t maxVol = 0;

bool didSpeak = false;
unsigned int mouthClosedTimer = 0;
const uint16_t timeMouthClosed = 1000;

uint16_t color[4] = {};
uint16_t brightness = 100;

String message = "";

void drawImage(int image[][8]) {
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      matrix.drawPixel(x, y, color[image[y][x]]);
    }
  }
  matrix.show();
}

void setMouth(int mouth[][8]) {
  drawImage(mouth);
  didSpeak = true;
  mouthClosedTimer = millis() + timeMouthClosed;
}

void setup() {
  Serial.begin(115200);
  matrix.begin();
  matrix.setBrightness(brightness);

  color[0] = matrix.Color(0, 0, 0);
  color[1] = matrix.Color(249, 165, 39);
  color[2] = matrix.Color(0, 0, 255);
  color[3] = matrix.Color(255, 0, 0);

  matrix.fillScreen(color[0]);
  matrix.show();
  
  SerialBT.begin("Led-Mask");
}

void loop() {
  /* Ideas:
   *  - Add sound visualization
   */
  if (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n') {
      message += String(incomingChar);
    } else {
      message = "";
    }
  }
  delay(20);
  
  if (message == "sad") {
    drawImage(sad);
    delay(2000);
  }
  if (message == "+") {
    if(brightness++ > 255) {
      brightness = 0;
    }
    Serial.println(brightness);
    matrix.setBrightness(brightness);
    matrix.show();
    delay(10);
  }
  if (message == "-") {
    if(brightness-- < 0) {
      brightness = 255;
    }
    Serial.println(brightness);
    matrix.setBrightness(brightness);
    matrix.show();
    delay(10);
  }
  
  maxVol = 0;

  for (int i = 0; i < samples; i++) {
    uint16_t analog = analogRead(MICPIN);
    uint16_t micline = abs(analog - 3072);
    maxVol = max(micline, maxVol);
  }
  vol = (maxVol + vol) / 2.0;

  if (didSpeak && millis() > mouthClosedTimer) {
    didSpeak = false;
  }

  if (vol <= 1400 && !didSpeak) {
    drawImage(smile);
  } else if (vol <= 1400 && didSpeak) {
    drawImage(mouthClosed);
  } else if (vol > 1400 && vol <= 2000) {
    setMouth(openMouthSmall);
  } else if (vol > 2000 && vol <= 2600) {
    setMouth(openMouthMedium);
  } else if (vol > 2600) {
    setMouth(openMouthBig);
  }
}
