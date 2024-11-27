#include <Arduino.h>
#include <simplyprint.h>
HTTPClient http;
JsonDocument doc;
int arreter_impression(int printer_id) {
    return 0;
}

bool isPrinterPrinting(int printer_id){
  http.begin("https://api.simplyprint.io/12305/printers/Get");         // on fais une demande a un site(ici un api)
  http.addHeader("X-API-KEY", "03f54a31-04da-403f-bca0-a4c4a7351862"); // Remplace YOUR_API_KEY par ta clé API
  http.addHeader("accept", "application/json");                        // Pour spécifier le format de la réponse
  int httpCode = http.GET(); 
  
  if (httpCode > 0)
    {
      String payload = http.getString();
      DeserializationError error = deserializeJson(doc, payload);
      JsonArray printers = doc["data"];
      for (JsonObject printer : printers)
      {
        int id = printer["id"]; // Récupérer l'ID de l'imprimante
        if (id == printer_id)
        {
          const char *statue = printer["printer"]["state"];
          if (strcmp(statue, "printing") == 0)
          {
            return 1;
          }
          else
          {
            return 0;
          }
        }
      }
      
      http.end();// Termine la requête
    }                     
}