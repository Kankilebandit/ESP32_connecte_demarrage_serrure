#ifndef simplyprint_h
#define simplyprint_h

#include <simplyprint.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
int arreter_impression(int printer_id);
bool isPrinterPrinting(int printer_id);
#endif