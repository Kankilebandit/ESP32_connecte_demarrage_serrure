#ifndef display_h

#define display_h

#include <display.h>
void init_ecran(void);  
void afficher_message_accueil(int curseurValue);
void Afficher_message_Cancel(int curseurValue);
void Afficher_message_End(int curseurValue);
void Afficher_message_Note(int curseurValue);
void Afficher_message_Pause();
void Afficher_message_Continuer();
void Afficher_message_clearbed();
void clear_screen(void);
int select(void);
int curseur(void);
#endif