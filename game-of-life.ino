
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
  /*



    delay (100);

    if (lebendePixel == 0) {
      delay (1000);
      software_Reset();
    }

  */
  //

  /*
    if (veraenderungen == veraenderungen2) {
      schritte++;
    }
    // Serial.println(schritte);
    //     Serial.print(veraenderungen);

    veraenderungen2 = veraenderungen;


    if (schritte == 50) {
      software_Reset();
    }
  */
}//schließt den loop




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


















int Nachbar(int n) {
  int Nachbar = 0;
  if (n == NUMPIXELS - WIDTH ) { //öffnet if Ecke links unten
    if (Pixels[n - 8] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. oben. ");
    }//schließt if
    if (Pixels[n - 7] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. rechts oben. ");
    }//schließt if
    if (Pixels[n + 1] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. Rechts. ");
    }//schließt if
    return Nachbar;
  }//schließt Ecke links unten



  if (n == lastPixel) { //öffnet if Ecke rechts unten
    if (Pixels[n - 8] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. oben. ");
    }//schließt if
    if (Pixels[n - 9] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. links oben. ");
    }//schließt if
    if (Pixels[n - 1] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. links. ");
    }//schließt if
    return Nachbar;
  }//schließt Ecke rechts unten





  if (n == 0 + WIDTH  - 1) { //öffnet if Ecke rechts oben
    if (Pixels[n + 8] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. unten. ");
    }//schließt if
    if (Pixels[n + 7 ] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. links unten. ");
    }//schließt if
    if (Pixels[n - 1] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. links. ");
    }//schließt if
    return Nachbar;
  }//schließt Ecke rechts oben






  if (n == 0) { //öffnet if Ecke links oben
    if (Pixels[n + 8] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. unten. ");
    }//schließt if
    if (Pixels[n + 9] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. rechts unten. ");
    }//schließt if
    if (Pixels[n + 1] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. rechts. ");
    }//schließt if
    return Nachbar;
  }//schließt Ecke links oben






  //if ((32<n)&&(n<39)){ //öffnet if untere Reihe
  if ((NUMPIXELS - WIDTH < n) && (n < lastPixel)) {
    if (Pixels[n + 1] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. Rechts. ");
    }//schließt if
    if (Pixels[n - 1] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. Links. ");
    }//schließt else
    if (Pixels[n - 7] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. rechts oben. ");
    }//schließt if
    if (Pixels[n - 8] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. oben. ");
    }//schließt if
    if (Pixels[n - 9] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. links oben. ");
    }//schließt if
    return Nachbar;
  }//schließt if untere Reihe






  if ((0 < n) && (n < 0 + WIDTH - 1)) { //öffnet if obere Reihe
    if (Pixels[n + 1] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. Rechts. ");
    }//schließt if
    if (Pixels[n - 1] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. Links. ");
    }//schließt else
    if (Pixels[n + 7] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. links unten. ");
    }//schließt if
    if (Pixels[n + 8] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. unten. ");
    }//schließt if
    if (Pixels[n + 9] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. rechts unten. ");
    }//schließt if
    return Nachbar;
  }//schließt if obere Reihe






  if (n % 8 == 0) { //öffnet if links
    //if ((n== 8)||(n==16)||(n==24)){
    if (Pixels[n + 1] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. Rechts. ");
    }//schließt if
    if (Pixels[n - 8] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. oben. ");
    }//schließt else
    if (Pixels[n - 7] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. rechts oben. ");
    }//schließt if
    if (Pixels[n + 8] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. unten. ");
    }//schließt if
    if (Pixels[n + 9] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. rechts unten. ");
    }//schließt if
    return Nachbar;
  }//schließt if links







  if (n % 8 == 7) { //öffnet if rechts
    //if ((n==15)||(n==23)||(n==31)){
    if (Pixels[n - 1] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. links. ");
    }//schließt if
    if (Pixels[n + 8] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. unten. ");
    }//schließt else
    if (Pixels[n + 7] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. links unten. ");
    }//schließt if
    if (Pixels[n - 8] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. oben. ");
    }//schließt if
    if (Pixels[n - 9] == 1) {
      Nachbar = Nachbar + 1;
      //Serial.print(n);
      //Serial.println(". Zelle. links oben. ");
    }//schließt if
    return Nachbar;
  }//schließt if rechts







  if (Pixels[n + 1] == 1) {
    Nachbar = Nachbar + 1;
    //Serial.print(n);
    //Serial.println(". Zelle. Rechts. ");
  }//schließt if
  if (Pixels[n - 1] == 1) {
    Nachbar = Nachbar + 1;
    //Serial.print(n);
    //Serial.println(". Zelle. Links. ");
  }//schließt else
  if (Pixels[n + 7] == 1) {
    Nachbar = Nachbar + 1;
    //Serial.print(n);
    //Serial.println(". Zelle. links unten. ");
  }//schließt if
  if (Pixels[n + 8] == 1) {
    Nachbar = Nachbar + 1;
    //Serial.print(n);
    //Serial.println(". Zelle. unten. ");
  }//schließt if
  if (Pixels[n + 9] == 1) {
    Nachbar = Nachbar + 1;
    //Serial.print(n);
    //Serial.println(". Zelle. rechts unten. ");
  }//schließt if
  if (Pixels[n - 7] == 1) {
    Nachbar = Nachbar + 1;
    //Serial.print(n);
    //Serial.println(". Zelle. rechts oben. ");
  }//schließt if
  if (Pixels[n - 8] == 1) {
    Nachbar = Nachbar + 1;
    //Serial.print(n);
    //Serial.println(". Zelle. oben. ");
  }//schließt if
  if (Pixels[n - 9] == 1) {
    Nachbar = Nachbar + 1;
    //Serial.print(n);
    //Serial.println(". Zelle. links oben. ");
  }//schließt if
  /*Serial.print(n);
    Serial.print(". Zelle. Nachbarn am Leben: ");
    Serial.println(Nachbar);
  */
  return Nachbar;
}//schließt int Nachbar
