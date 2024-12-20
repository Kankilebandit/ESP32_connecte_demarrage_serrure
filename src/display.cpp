#include <Arduino.h>
#include <display.h>
#include <board_mapping.h>
#define boutonSelect A0
#define pot A2
#define DEBOUNCE_DELAY 50   // Temps de debounce en millisecondes
HardwareSerial MySerial(1); // define a Serial for UART1
const int MySerialRX = 16;
const int MySerialTX = 17;

void init_ecran(void)
{
    MySerial.begin(9600, SERIAL_8N1, MySerialRX, MySerialTX);
}

int select(void)
{
    pinMode(boutonSelect, INPUT_PULLUP);
    if (digitalRead(boutonSelect))
    {
        return 0;
    }
    else
    {
        int test = digitalRead(boutonSelect);
        while (test == 0)
        {
            test = digitalRead(boutonSelect);
        }
        return 1;
    }
}
int curseur(void)
{
    int potValue = analogRead(pot);
    if (potValue < 1023)
    {
        return 0; // Valeur entre 0 et 1022 (inclus)
    }
    else if (potValue >= 1023 && potValue < 2046)
    {
        return 1; // Valeur entre 1023 et 2045 (inclus)
    }
    else if (potValue >= 2046 && potValue < 3069)
    {
        return 2; // Valeur entre 2046 et 3068 (inclus)
    }
    else
    {
        return 3; // Valeur supérieure ou égale à 3069
    }
}
void clear_screen(void)
{
    MySerial.write(0xFE);
    MySerial.write(0x51);
}
void afficher_message_prix(float mag, float imprimante2)
{
    char message[50]; // Un tampon pour stocker le message à envoyer
    snprintf(message, sizeof(message), "%.2f", mag);
    char message2[50]; // Un tampon pour stocker le message à envoyer
    snprintf(message, sizeof(message), "%.2f",imprimante2);
    MySerial.write(" prix impression:");
    MySerial.write(0xFE);
    MySerial.write(0x45);
    MySerial.write(0x40);
    MySerial.write("mag:");
    for (int i = 0; i < strlen(message); i++) {
        MySerial.write(message[i]);
    }
    MySerial.write(0xFE);
    MySerial.write(0x45);
    MySerial.write(0x50);
    for (int i = 0; i < strlen(message); i++) {
        MySerial.write(message2[i]);
    }
}
void afficher_message_accueil(int curseurValue)
{
    if (curseurValue == 0 || curseurValue == 1)
    {
        MySerial.write(" [Pause/continue]");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(0x40);
        MySerial.write("Finished");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(0x14);
        MySerial.write("Cancel?");
    }

    else if (curseurValue == 2)
    {
        MySerial.write(" Pause/continue");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(0x40);
        MySerial.write("[Finished]");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(0x14);
        MySerial.write("Cancel?");
    }
    else
    {
        MySerial.write(" Pause/continue");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(0x40);
        MySerial.write("Finished");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(0x14);
        MySerial.write("[Cancel?]");
    }
}
void Afficher_message_Cancel(int curseurValue)
{
    if (curseurValue == 0 || curseurValue == 1)
    {
        MySerial.write(" [Fail]");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(0x40);
        MySerial.write("Regret");
    }

    else
    {
        MySerial.write(" Fail");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(0x40);
        MySerial.write("[Regret]");
    }
}
void Afficher_message_clearbed()
{
    MySerial.write(" bed is empty?");
    MySerial.write(0xFE);
    MySerial.write(0x45);
    MySerial.write(0x40);
    MySerial.write("if yes press button");
}
void Afficher_message_End(int curseurValue)
{
    if (curseurValue == 0 || curseurValue == 1)
    {
        MySerial.write(" [Failed]");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(0x40);
        MySerial.write("Finished");
    }

    else
    {
        MySerial.write(" Failed");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(0x40);
        MySerial.write("[Finished]");
    }
}

void Afficher_message_Note(int curseurValue)
{
    if (curseurValue == 0)
    {
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(2);
        MySerial.write("[1]");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(8);
        MySerial.write("2");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(13);
        MySerial.write("3");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(18);
        MySerial.write("4");
    }
    else if (curseurValue == 1)
    {
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(3);
        MySerial.write("1");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(7);
        MySerial.write("[2]");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(13);
        MySerial.write("3");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(18);
        MySerial.write("4");
    }
    else if (curseurValue == 2)
    {
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(3);
        MySerial.write("1");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(8);
        MySerial.write("2");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(12);
        MySerial.write("[3]");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(18);
        MySerial.write("4");
    }
    else
    {
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(2);
        MySerial.write("1");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(8);
        MySerial.write("2");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(13);
        MySerial.write("3");
        MySerial.write(0xFE);
        MySerial.write(0x45);
        MySerial.write(17);
        MySerial.write("[4]");
    }
}

void Afficher_message_Pause()
{
    MySerial.write("printing");
    MySerial.write(0xFE);
    MySerial.write(0x45);
    MySerial.write(0x40);
    MySerial.write("Do you want to pause");
    MySerial.write(0xFE);
    MySerial.write(0x45);
    MySerial.write(0x14);
    MySerial.write("if yes press button");
}
void Afficher_message_Continuer()
{
    MySerial.write("pause");
    MySerial.write(0xFE);
    MySerial.write(0x45);
    MySerial.write(0x40);
    MySerial.write("resume printing?");
    MySerial.write(0xFE);
    MySerial.write(0x45);
    MySerial.write(0x14);
    MySerial.write("if yes press button");
}
void Afficher_message_Merci()
{
    MySerial.write(" Thank you!");
    delay(1000);
}

void Afficher_message_erreur()
{
    MySerial.write(" Erreur!");
    MySerial.write(0xFE);
    MySerial.write(0x45);
    MySerial.write(0x40);
    MySerial.write(" retour au menu");
    delay(1000);
}