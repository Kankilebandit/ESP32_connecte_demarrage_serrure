#ifndef simplyprint_h
#define simplyprint_h

#include <simplyprint.h>

void GetState(int printer_id, char *printer_state_code);
int continuer_impression(int printer_id);
int pauser_impression(int printer_id);
int impression_finit(int printer_id);

#endif