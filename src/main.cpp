#include <Arduino.h>
#include <HTTPClient.h>

#include <board_mapping.h>
#include <init.h>

#include <WiFi.h>
#include "esp_wpa2.h"        // pour les réseaux wifi sécurisés
#include "arduino_secrets.h" // fichier contenant les informations de connexion

#include <display.h>
#include <simplyprint.h>


void setup() {
  initialisationSerie();
  initialisationWifi();
}

void loop() {
    
}
