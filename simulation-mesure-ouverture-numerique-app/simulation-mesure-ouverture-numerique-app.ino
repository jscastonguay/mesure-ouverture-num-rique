#include "donnees.h"

bool enOpertion = false;
unsigned int nbPasRealise;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  switch (Serial.read()) {
    case 'd':
      enOpertion = true;
      nbPasRealise = 0;
      delay(1000);
      break;

    case 's':
      enOpertion = false;
      break;
  }

  if (enOpertion) {
    delay(500);
    Serial.println(donneesSimulees[nbPasRealise]);
    nbPasRealise++;
    if (nbPasRealise == (NB_PAS_PAR_180_DEGRES - DEBATTEMENT_POUR_INIT)) {
      enOpertion = false;
      Serial.println("FIN");
    }
  }

  digitalWrite(LED_BUILTIN, enOpertion);
}
