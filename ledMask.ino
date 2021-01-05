#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define MATRIXPIN 13
#define MICPIN 34

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, MATRIXPIN,
  NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS   + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB           + NEO_KHZ800);

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

int smile[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {2,0,0,0,0,0,0,2},
    {2,2,0,0,0,0,2,2},
    {0,2,2,2,2,2,2,0},
    {0,0,2,2,2,2,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

int sad[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,3,3,3,3,0,0}, 
    {0,3,3,3,3,3,3,0},
    {3,3,0,0,0,0,3,3},
    {3,0,0,0,0,0,0,3},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

float vol = 0.0;
uint16_t maxVol = 0;
const uint16_t samples = 1280;

uint16_t color[4] = {};

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
  color[2] = matrix.Color(0,0,255);
  color[3] = matrix.Color(255,0,0);
  
  matrix.fillScreen(color[0]);
  matrix.show();
}

void loop() {
  maxVol = 0;

  for (int i = 0; i < samples; i++){
    uint16_t analog = analogRead(MICPIN);
    uint16_t micline = abs(analog - 3072);
    maxVol = max(micline, maxVol); //Get max Value of Samples
  }
  vol = (maxVol + vol) / 2.0;
  
  if(vol <= 1400) {
    drawImage(smile);
  } else if(vol > 1400 && vol <= 1950) {
    drawImage(openMouthSmall);
  } else if(vol > 1950 && vol <= 2510) {
    drawImage(openMouthMedium);
  } else if(vol > 2510) {
    drawImage(openMouthBig);
  }
}
