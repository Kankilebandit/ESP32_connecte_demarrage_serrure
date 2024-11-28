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
  ETAT_menu,     // Etat 0 : Valeur entre 0 et 1022
  ETAT_pause,    // Etat 1 : Valeur entre 1023 et 2045
  ETAT_clearbed, // Etat 2 : Valeur entre 2046 et 3068
  ETAT_finit,
  ETAT_note,
  ETAT_Cancel // Etat 3 : Valeur >= 3069
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
  getCurseur = curseur();
  clear_screen();
  switch (currentState)
  {
  case ETAT_menu:
    afficher_message_accueil(getCurseur);
    if (select())
    {
      if (getCurseur == 0 || getCurseur == 1)
      {
        currentState = ETAT_pause;
      }
      else if (getCurseur == 2)
      {
        currentState = ETAT_clearbed;
      }
      else
      {
        currentState = ETAT_Cancel;
      }
    }
    break;
  case ETAT_pause:
    /*//String test = GetState(IMP_Mag);
    if (ispaused == "printing")
    {
      Afficher_message_Pause();
    }
    else if (ispaused == "paused")
    {
      Afficher_message_Continuer();
    }*/
    break;
  case ETAT_clearbed:
    Afficher_message_clearbed();
    if (select())
    {
      currentState = ETAT_finit;
    }
    break;
  case ETAT_finit:
    Afficher_message_End(getCurseur);
    if (select())
    {
      if (getCurseur == 0 || getCurseur == 1)
      {
        // envoyer failed
        currentState = ETAT_menu;
      }
      else
      {
        currentState = ETAT_note;
      }
    }
    break;
  case ETAT_note:
    Afficher_message_Note(getCurseur);
    if (select())
    {
      if (getCurseur == 0)
      {
        // envoyer note 1
      }
      else if (getCurseur == 1)
      {
        // envoyer note 2
      }
      else if (getCurseur == 2)
      {
        // envoyer note 3
      }
      else
      {
        // envoyer note 4
      }
      currentState = ETAT_menu;
    }
    break;
  case ETAT_Cancel:
    Afficher_message_Cancel(getCurseur); // curseur a 1 ou 2 = fail  || 3  ou 4 = fail
    if (select())
    {
      if (getCurseur == 0 || getCurseur == 1)
      {
        // envoyer fail
      }
      else
      {
        // envoyer cancel
      }
      currentState = ETAT_menu;
    }
    break;
  }
  delay(100);
}
