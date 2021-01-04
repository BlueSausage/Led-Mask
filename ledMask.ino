#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN 13

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN,
  NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS   + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB           + NEO_KHZ800);

int openMouthBig[8][8] = {
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {0,1,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0}
};

int openMouthMedium[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {0,1,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,0,0,0,0,0}
};

int openMouthSmall[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,0},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

int openMouthClosed[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

int sad[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,2,2,2,2,0,0}, 
    {0,2,2,2,2,2,2,0},
    {2,2,0,0,0,0,2,2},
    {2,0,0,0,0,0,0,2},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

int smile[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,1,1},
    {0,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

int off[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

uint16_t color[3] = {};

void drawImage(int image[][8]) {
  for(int y = 0; y < 8; y++) {
    for(int x = 0; x < 8; x++) {
      matrix.drawPixel(x, y, color[image[y][x]]);
    }
  }
  matrix.show();
}

void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.setBrightness(100);
  
  color[0] = matrix.Color(0,0,0);
  color[1] = matrix.Color(249,165,39);
  color[2] = matrix.Color(255,0,0);
  
  matrix.fillScreen(color[0]);
  matrix.show();
}

void loop() {
  drawImage(openMouthBig);
  delay(500);
  drawImage(openMouthMedium);
  delay(500);
  drawImage(openMouthSmall);
  delay(500);
  drawImage(openMouthClosed);
  delay(250);
  drawImage(smile);
  delay(2000);
  drawImage(sad);
  delay(2000);
  drawImage(off);
  delay(1000);
}
