
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR
#endif
#define ONE_WIRE_BUS   2
#define PIN            6
#define HEIGHT         8  //Höhe des Spielfeldes 
#define WIDTH          8  //Breite des Spielfeldes 
#define NUMPIXELS      WIDTH * HEIGHT//Anzahl aller Pixel
OneWire wire(ONE_WIRE_BUS);
DallasTemperature sensor(&wire);

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int Pixels [NUMPIXELS] =  {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

int Leben [NUMPIXELS];
int schritte = 0;
int veraenderungen = 0;
int veraenderungen2 = 0;
int lastPixel = NUMPIXELS - 1;
int randNumber1;
int buttonPin = 3;
int buttonState = 0;
float Temperatures; 
int Temp;
void setup() {

  randomSeed(analogRead(0));
  for (int r = 0; r < NUMPIXELS; r++) {
      Pixels[r]  = random(2);
  }
  Serial.begin(9600);
  Serial.println("Temperaturen:");
  pixels.begin();
    pinMode(buttonPin, INPUT);
      matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(10);
  matrix.setTextColor(colors[0]);
};

int x    = matrix.width();
int pass = 0;



void loop() {
  
  buttonState = digitalRead(buttonPin);  
  
  
  sensor.requestTemperatures();
  Temperatures = sensor.getTempCByIndex(0);
  int asdasd = 24;
  String Temperaturen = String(asdasd, DEC);
  
  if (buttonState == HIGH) {
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F(Temperaturen));
  if(--x < -36) {
    x = matrix.width();
    if(++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  delay(100);
  } else {
  showStripe();
  gameoflife();
  veraenderungen = ermittleVeraenderungen();
  syncSteps();



  if (veraenderungen == veraenderungen2) {
    schritte++;

  } else {
    schritte = 0;
  }

  if (schritte >= 20) {
    software_Reset();
  }
  veraenderungen2 = veraenderungen;
  
  }
  
  
    Serial.print(Temperatures); 
   Serial.println(" Grad Celius");
  delay(127);
}


// fuerht zur anzeige als LED matrix von Pixels[]
void showStripe() {
  if (Temp <= 25){
  for (int i = 0; i < NUMPIXELS; i++) {
    if (Pixels[i] == 0) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 10));
    }
    if (Pixels[i] == 1 ) {
      pixels.setPixelColor(i, pixels.Color(15, 10, 0));
    }
  }
  pixels.show();
}
  if (Temp == 26){
  for (int i = 0; i < NUMPIXELS; i++) {
    if (Pixels[i] == 0) {
      pixels.setPixelColor(i, pixels.Color(5, 0, 10));
    }
    if (Pixels[i] == 1 ) {
      pixels.setPixelColor(i, pixels.Color(15, 15, 3));
    }
  }
  pixels.show();
}
  if (Temp == 27){
  for (int i = 0; i < NUMPIXELS; i++) {
    if (Pixels[i] == 0) {
      pixels.setPixelColor(i, pixels.Color(10, 0, 10));
    }
    if (Pixels[i] == 1 ) {
      pixels.setPixelColor(i, pixels.Color(13, 15 , 10));
    }
  }
  pixels.show();
}
  if (Temp == 28){
  for (int i = 0; i < NUMPIXELS; i++) {
    if (Pixels[i] == 0) {
      pixels.setPixelColor(i, pixels.Color(10, 0, 5));
    }
    if (Pixels[i] == 1 ) {
      pixels.setPixelColor(i, pixels.Color(10, 15, 10));
    }
  }
  pixels.show();
}
  if (Temp >= 29){
  for (int i = 0; i < NUMPIXELS; i++) {
    if (Pixels[i] == 0) {
      pixels.setPixelColor(i, pixels.Color(10, 0, 0));
    }
    if (Pixels[i] == 1 ) {
      pixels.setPixelColor(i, pixels.Color(0, 13, 10));
    }
  }
  pixels.show();
}
}


//wendet die regeln von game of life an und speichert den naechsten schritt in einem array (Leben[])
void gameoflife() {
  for (int i = 0; i < NUMPIXELS; i++) {
    int anzahlLebenderPixel = Nachbar(i);
    if ((anzahlLebenderPixel < 2) || (anzahlLebenderPixel > 3)) {
      Leben[i] = 0;
    }
    if (anzahlLebenderPixel == 3) {
      Leben[i] = 1;
    }
  }
}


//syncronisiert den aktuelle, mit den naechsten schritt
void syncSteps() {
  for (int i = 0; i < NUMPIXELS; i++) {
    Pixels[i] = Leben[i];
  }
}

int ermittleVeraenderungen() {
  int veraenderungen = 0;

  for (int i = 0; i < NUMPIXELS; i++) {
    if (Leben[i] != Pixels[i]) {
      veraenderungen = veraenderungen + 1;
    }
  }
  return veraenderungen;
}

void software_Reset()
{
  asm volatile ("  jmp 0");
}


//sorgt fuer die Ränder
int Nachbar(int n) {
  int Nachbar = 0;
  //öffnet if Ecke links unten
  if (n == NUMPIXELS - WIDTH ) { 
    if (Pixels[n - 8] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n - 7] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n + 1] == 1) {
      Nachbar = Nachbar + 1;
    }
    return Nachbar;
  }
  
//öffnet if Ecke rechts unten
  if (n == lastPixel) { 
    if (Pixels[n - 8] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n - 9] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n - 1] == 1) {
      Nachbar = Nachbar + 1;
    }
    return Nachbar;
  }
  
//öffnet if Ecke rechts oben
  if (n == 0 + WIDTH  - 1) { 
    if (Pixels[n + 8] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n + 7 ] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n - 1] == 1) {
      Nachbar = Nachbar + 1;
    }
    return Nachbar;
  }

//öffnet if Ecke links oben
  if (n == 0) {
    if (Pixels[n + 8] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n + 9] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n + 1] == 1) {
      Nachbar = Nachbar + 1;
    }
    return Nachbar;
  }

//öffnet if untere Reihe
  if ((NUMPIXELS - WIDTH < n) && (n < lastPixel)) {
    if (Pixels[n + 1] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n - 1] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n - 7] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n - 8] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n - 9] == 1) {
      Nachbar = Nachbar + 1;
    }
    return Nachbar;
  }

//öffnet if obere Reihe
  if ((0 < n) && (n < 0 + WIDTH - 1)) { 
    if (Pixels[n + 1] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n - 1] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n + 7] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n + 8] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n + 9] == 1) {
      Nachbar = Nachbar + 1;
    }
    return Nachbar;
  }

//öffnet if links
  if (n % 8 == 0) { 
    if (Pixels[n + 1] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n - 8] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n - 7] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n + 8] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n + 9] == 1) {
      Nachbar = Nachbar + 1;
    }
    return Nachbar;
  }
 //öffnet if rechts
  if (n % 8 == 7) {
    if (Pixels[n - 1] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n + 8] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n + 7] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n - 8] == 1) {
      Nachbar = Nachbar + 1;
    }
    if (Pixels[n - 9] == 1) {
      Nachbar = Nachbar + 1;
    }
    return Nachbar;
  }

//öffnet restliches Spielfeld
  if (Pixels[n + 1] == 1) {
    Nachbar = Nachbar + 1;
  }
  if (Pixels[n - 1] == 1) {
    Nachbar = Nachbar + 1;
  }
  if (Pixels[n + 7] == 1) {
    Nachbar = Nachbar + 1;
  }
  if (Pixels[n + 8] == 1) {
    Nachbar = Nachbar + 1;
  }
  if (Pixels[n + 9] == 1) {
    Nachbar = Nachbar + 1;
  }
  if (Pixels[n - 7] == 1) {
    Nachbar = Nachbar + 1;
  }
  if (Pixels[n - 8] == 1) {
    Nachbar = Nachbar + 1;
  }
  if (Pixels[n - 9] == 1) {
    Nachbar = Nachbar + 1;
  }
  return Nachbar;
}
