#include <Arduino.h>
#include <simplyprint.h>
HTTPClient http;
JsonDocument doc;
int arreter_impression(int printer_id) {
    return 0;
}

int test(int printer_id){
  http.begin("https://api.simplyprint.io/12305/printers/Get");         // on fais une demande a un site(ici un api)
  http.addHeader("X-API-KEY", "cd0b5d0b-745a-4228-9cee-48c1aa8226da"); // Remplace YOUR_API_KEY par ta clé API
  http.addHeader("accept", "application/json");                        // Pour spécifier le format de la réponse
  int httpCode = http.GET(); 
  String requestBody = "{\"page\": 1, \"page_size\": 10, \"search\": \"Printer 1\"}";   

  if (httpCode > 0)
    {
      String payload = http.getString();
      Serial.println();
      DeserializationError error = deserializeJson(doc, payload);
      JsonArray printers = doc["data"];
      for (JsonObject printer : printers)
      {
        int id = printer["id"]; // Récupérer l'ID de l'imprimante
        // Serial.println(id);
        if (id == printer_id)
        {
          const char *name = printer["printer"]["name"];
          Serial.print("Nom de l'imprimante : ");
          Serial.println(name);
          const char *statue = printer["printer"]["state"];
          // Serial.print("etat : ");

          Serial.println(statue);
          if (strcmp(statue, "printing") == 0)
          {
            Serial.println("Je print");
          }
          else
          {
            Serial.println("Je print pas");
          }
        }
      }
      // Termine la requête
      http.end();
    }                     
}