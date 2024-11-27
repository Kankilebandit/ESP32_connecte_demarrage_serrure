#include <Arduino.h>
#include <HTTPClient.h>

#include <board_mapping.h>
#include <init.h>

#include <WiFi.h>
#include "esp_wpa2.h"        // pour les réseaux wifi sécurisés
#include "arduino_secrets.h" // fichier contenant les informations de connexion

#include <display.h>
#include <simplyprint.h>

#define IMP_402 21937
#define IMP_Mag 18491

int paused_print = 0;

void setup()
{
  initialisationSerie();
  initialisationWifi();

  paused_print = pauser_impression(IMP_402);
  Serial.println(paused_print);
  Serial.println(GetState(IMP_402));
  paused_print = continuer_impression(IMP_402);
  Serial.println(paused_print);
}

void loop()
{

  Serial.println(GetState(IMP_402));
  delay(500);
}
