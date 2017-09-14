#include <SPI.h>

const unsigned int NB_PAS_PAR_180_DEGRES = 200;
const unsigned int DEBATTEMENT_POUR_INIT = 4;

const int CW = 1;
const int CCW = 0;

const int DIR_PIN = 6;
const int PULSE_PIN = 7;
const int FIN_DE_COURSE_PIN = 5;
const int ADC_SS_PIN = 10;

bool enOpertion = false;
unsigned int nbPasRealise;


inline void startADCcomm() {
  digitalWrite(ADC_SS_PIN, LOW);
}

inline void stopADCcomm() {
  digitalWrite(ADC_SS_PIN, HIGH);
}

uint16_t readADC() {

  #define DUMMY  0x00

  startADCcomm();
  SPI.transfer(0x6);
  unsigned int MSB = SPI.transfer(DUMMY) & 0x0F;
  unsigned int LSB = SPI.transfer(DUMMY);
  stopADCcomm();
  return (MSB << 8) | LSB;
}

inline bool conditionArret( int dir) {
  return (dir == CCW) && (digitalRead(FIN_DE_COURSE_PIN) == HIGH);
}

void tourne( unsigned int nbPas, int dir, unsigned int delai) {

  digitalWrite(DIR_PIN, dir);
  while (nbPas && !conditionArret(dir)) {
    digitalWrite(PULSE_PIN, HIGH);
    delay(1);
    digitalWrite(PULSE_PIN, LOW);
    delay(delai);
    nbPas--;
  }
}

void setup() {

  Serial.begin(9600);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);

  pinMode(DIR_PIN, OUTPUT);
  pinMode(PULSE_PIN, OUTPUT);
  pinMode(ADC_SS_PIN, OUTPUT);
  pinMode(FIN_DE_COURSE_PIN, INPUT);

  digitalWrite(PULSE_PIN, LOW);
  stopADCcomm();
}

void loop() {
  switch (Serial.read()) {
    case 'd':
      enOpertion = true;
      nbPasRealise = 0;
      tourne(4, CW, 10);
      tourne(1000, CCW, 10);
      break;

    case 's':
      enOpertion = false;
      break;
  }

  if (enOpertion) {

    tourne(1, CW, 0);
    delay(500);
    Serial.println(readADC());
    nbPasRealise++;
    if (nbPasRealise == (NB_PAS_PAR_180_DEGRES - DEBATTEMENT_POUR_INIT)) {
      enOpertion = false;
      Serial.println("FIN");
    }
  }
}
