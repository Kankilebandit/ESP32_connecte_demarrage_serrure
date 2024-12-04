#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <board_mapping.h>
#include <init.h>

#include <WiFi.h>
#include "esp_wpa2.h"        // pour les réseaux wifi sécurisés
#include "arduino_secrets.h" // fichier contenant les informations de connexion

#include <display.h>
#include <simplyprint.h>

#define IMP_402 21937
#define IMP_Mag 18491

#define GREEN 14
#define RED 32
#define ORANGE 15

int paused_print = 0;
int getCurseur = 0;
int ClearBedFlag = 0;
char printerstate[20] = {'\0'};
char prevprinterstate[20] = {'\0'};
enum Etat
{
  ETAT_menu,     // Etat 0 : Valeur entre 0 et 1022
  ETAT_pause,    // Etat 1 : Valeur entre 1023 et 2045
  ETAT_clearbed, // Etat 2 : Valeur entre 2046 et 3068
  ETAT_finit,
  ETAT_note,
  ETAT_merci,
  ETAT_PRIX,
  ETAT_Cancel, // Etat 3 : Valeur >= 3069
  ETAT_erreur
};
Etat currentState = ETAT_menu;
void setup()
{
  initialisationSerie();
  initialisationWifi();
  init_ecran();
  clear_screen();
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(ORANGE, OUTPUT);
  GetState(IMP_Mag, &prevprinterstate[0]);
}

void loop()
{
  /*GetState(IMP_Mag, &printerstate[0]);
  if((strcmp(printerstate, prevprinterstate))>0){
    if((strcmp(prevprinterstate, "printing")) and (strcmp(printerstate, "operational"))){
      ClearBedFlag = 1;
    }else{
      prevprinterstate = printerstate;
    }
  }*/

  getCurseur = curseur();
  clear_screen();

  switch (currentState)
  {
  case ETAT_menu:
    afficher_message_accueil(getCurseur);
    GetState(IMP_Mag, &printerstate[0]);
    Serial.println(printerstate);
    // Serial.println(printerstate);

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

    //------------------------------------------------------------------------------------------------------------------------
    // etat pause/depause
  case ETAT_pause:

    if (strcmp(printerstate, "printing") == 0)
    {
      Afficher_message_Pause();
      if (select())
      {
        pauser_impression(IMP_Mag);
        currentState = ETAT_merci;
      }
    }
    else if (strcmp(printerstate, "paused") == 0)
    {
      Afficher_message_Continuer();
      if (select())
      {
        continuer_impression(IMP_Mag);
        currentState = ETAT_merci;
      }
    }
    else
    {
      currentState = ETAT_erreur;
    }
    break;

    //------------------------------------------------------------------------------------------------------------------------
    // Imprimante a fini d'imprimer fail/reussi
  case ETAT_clearbed:
    Afficher_message_clearbed();
    if (select())
    {
      // envoyer reussi
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
        currentState = ETAT_merci;
      }
      else
      {
        // aller anote(rien a ajouter)
        // ajouter la fonction dans etat note
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
      currentState = ETAT_merci;
    }
    break;

    //------------------------------------------------------------------------------------------------------------------------
    // Cancl print
  case ETAT_Cancel:
    Afficher_message_Cancel(getCurseur); // curseur a 1 ou 2 = fail  || 3  ou 4 = fail
    if (strcmp(printerstate, "printing") == 0)
    {
      if (select())
      {
        if (getCurseur == 0 || getCurseur == 1)
        {
          cancel_print(IMP_Mag, 1);
          currentState = ETAT_merci;
        }
        else
        {
          cancel_print(IMP_Mag, 2);
          currentState = ETAT_merci;
        }

        currentState = ETAT_merci;
      }
    }
    else
    {
      currentState = ETAT_erreur;
    }
    break;
    //------------------------------------------------------------------------------------------------------------------------
    // Fonction API réussi et utiliser au bon moment=merci
    // fonction api pas utiisable dans le contexte donc erreur
  case ETAT_merci:
    Afficher_message_Merci();
    currentState = ETAT_menu;
    break;

  case ETAT_erreur:
    Afficher_message_erreur();
    currentState = ETAT_menu;
    break;

  case ETAT_PRIX:
    float prixMag = GetPriceMag();
    float prix402 = GetPrice402();
    Serial.println(prixMag);
    // afficher_message_prix(prixMag, prixMag);
    if (select())
    {
      currentState = ETAT_menu;
    }

    break;
  }

  if (((strcmp(printerstate, "operational")) == 0) and (!ClearBedFlag))
  {
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
    digitalWrite(ORANGE, LOW);
  }
  else if (ClearBedFlag)
  {
    digitalWrite(GREEN, !digitalRead(GREEN));
    digitalWrite(RED, LOW);
    digitalWrite(ORANGE, LOW);
  }
  else if ((strcmp(printerstate, "printing")) == 0)
  {
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, HIGH);
    digitalWrite(ORANGE, LOW);
  }
  else if ((strcmp(printerstate, "paused")) == 0)
  {
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);
    digitalWrite(ORANGE, HIGH);
  }
  else
  {
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);
    digitalWrite(ORANGE, LOW);
  }

  delay(1000);
}
