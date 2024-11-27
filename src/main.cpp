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
int getCurseur = 0;
enum Etat
{
  ETAT_menu,  // Etat 0 : Valeur entre 0 et 1022
  ETAT_test0_1, // Etat 1 : Valeur entre 1023 et 2045
  ETAT_test2, // Etat 2 : Valeur entre 2046 et 3068
  ETAT_test3  // Etat 3 : Valeur >= 3069
};
Etat currentState = ETAT_menu;
void setup()
{
  initialisationSerie();
  initialisationWifi();
  init_ecran();
  clear_screen();
}

void loop()
{
  switch (currentState)
  {
  case ETAT_menu:
    clear_screen();
    getCurseur = curseur();
    Serial.println(getCurseur);
    afficher_message_accueil(getCurseur);
    if (select())
    {
      if (getCurseur == 0 || getCurseur == 1)
      {
        currentState = ETAT_test0_1;
      }
      else if (getCurseur == 2)
      {
        currentState = ETAT_test2;
      }
      else
      {
        currentState = ETAT_test3;
      }
    }
    break;
  case ETAT_test0_1:
    clear_screen();
    Serial.println("tu es a test 1");
    break;
  case ETAT_test2:
    clear_screen();
    Serial.println("tu es a test 2");
    break;
  case ETAT_test3:
    clear_screen();
    Serial.println("tu es a test 3");
    break;
  }
  delay(100);
}
