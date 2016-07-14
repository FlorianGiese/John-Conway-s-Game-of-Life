
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#ifdef __AVR__
#endif
#define PIN            6
#define HEIGHT         8  //Höhe des Spielfeldes 
#define WIDTH          8  //Breite des Spielfeldes 
#define NUMPIXELS      WIDTH * HEIGHT//Anzahl aller Pixel


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int Pixels [NUMPIXELS] =  {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 1, 0, 0, 0, 0, 0,
  0, 0, 1, 0, 0, 0, 0, 0,
  0, 1, 1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 0, 0, 1, 1, 1, 0,
  1, 0, 1, 0, 1, 0, 0, 0,
  0, 1, 1, 0, 0, 0, 0, 0
};

int Leben [NUMPIXELS];
int schritte = 0;
int veraenderungen = 0;
int veraenderungen2 = 0;
int lastPixel = NUMPIXELS - 1;
int randNumber1;
const int buttonPin = 2;
int buttonState = 0;


void setup() {

  randomSeed(analogRead(0));
  for (int r = 0; r < NUMPIXELS; r++) {
      Pixels[r]  = random(2);
  }

  Serial.begin(9600);

  pixels.begin();
};


void loop() {

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
  Serial.println(schritte);
  delay(127);
}


// fuerht zur anzeige als LED matrix von Pixels[]
void showStripe() {
  for (int i = 0; i < NUMPIXELS; i++) {
    if (Pixels[i] == 0) {
      pixels.setPixelColor(i, pixels.Color(10, 3, 0));
    }
    if (Pixels[i] == 1 ) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 15));
    }
  }
  pixels.show();
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
