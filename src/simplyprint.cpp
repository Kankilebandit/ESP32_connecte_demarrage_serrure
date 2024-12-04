#include <Arduino.h>
#include <ArduinoJson.h>
#include <simplyprint.h>
#include <HTTPClient.h>
#include <arduino_secrets.h>

// Votre fonction GetState comme définie précédemment
void GetState(int printer_id, char *printer_state_code) {
    HTTPClient http;
    String path = "printers/Get";
    http.begin((API_HOST + path));   
    http.addHeader("X-API-KEY", "9c990e32-173c-4264-bc75-abdb35b93c67"); 
    http.addHeader("accept", "application/json"); 
    int httpResponseCode = http.POST("{}");

    if (httpResponseCode > 0) {
        String response = http.getString();
        const size_t capacity = 10 * JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2) + 1024;
        DynamicJsonDocument doc(capacity);
        deserializeJson(doc, response);
        if (doc["status"] == true) {
            JsonArray data = doc["data"];
            
            for (JsonObject printerObj : data) {
                const char *printerState = printerObj["printer"]["state"];
                int printerId = printerObj["id"];

                if (printerId == printer_id) {
                sprintf(printer_state_code, "%s", printerState);  // Copier la chaîne printerState dans printer_state_code
                }
            }
        }
        http.end();
    }
}




int continuer_impression(int printer_id)
{
    bool status = false;
    HTTPClient http;
    String path = "printers/actions/Resume?pid=";
    http.begin((API_HOST + path + printer_id));
    http.addHeader("accept", "application/json");
    http.addHeader("X-API-KEY", API_KEY);

    int httpResponseCode = http.POST("{}");

    if (httpResponseCode > 0)
    {
        String response = http.getString();

        const size_t capacity = 10 * JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2) + 1024;
        DynamicJsonDocument doc(capacity);
        deserializeJson(doc, response);

        status = doc["status"];
    }
    else
    {
        printf("%d", httpResponseCode);
        printf("No response\r\n");
    }
    http.end();

    if (status == true)
    {
        return 1; // Resume successful
    }
    else
    {
        return 0; // cannot Resume
    }
}

int pauser_impression(int printer_id)
{
    bool status = false;
    HTTPClient http;

    String path = "printers/actions/Pause?pid=";
    http.begin((API_HOST + path + printer_id));
    http.addHeader("accept", "application/json");
    http.addHeader("X-API-KEY", API_KEY);
    // Corps de la requête

    //A CHANGER EN FONCTION DU MENU
    String requestBody = "{\"success\": true, \"rating\": 4}";
    http.addHeader("Content-Type", "application/json");
    
    // Envoi de la requête POST
    int httpResponseCode = http.POST(requestBody);
    //int httpResponseCode = http.POST("{}");

    if (httpResponseCode > 0)
    {
        String response = http.getString();

        const size_t capacity = 10 * JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2) + 1024;
        DynamicJsonDocument doc(capacity);
        deserializeJson(doc, response);
        status = doc["status"];
    }
    else
    {
        printf("%d", httpResponseCode);
        printf("No response\r\n");
    }
    http.end();

    if (status == true)
    {
        return 1; // pause successful
    }
    else
    {
        return 0; // cannot pause
    }
}

int impression_finit(int printer_id)
{
    bool status = false;
    HTTPClient http;

    String path = "printers/actions/ClearBed?pid=";
    http.begin((API_HOST + path + printer_id));
    http.addHeader("accept", "application/json");
    http.addHeader("X-API-KEY", API_KEY);
    int httpResponseCode = http.POST("{}");


    if (status == true)
    {
        return 1; // pause successful
    }
    else
    {
        return 0; // cannot pause
    }
}

int cancel_print(int printer_id, int raison)
{
    bool status = false;
    HTTPClient http;

    String path = "printers/actions/Cancel?pid=";
    http.begin((API_HOST + path + printer_id));
    http.addHeader("accept", "application/json");
    http.addHeader("X-API-KEY", API_KEY);
    char requestBody[100];
    sprintf(requestBody, "{\"reason\": %d, \"comment\": \"Cancel comment\"}", raison); 
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST("{}");
    if (status == true)
    {
        return 1; // pause successful
    }
    else
    {
        return 0; // cannot pause
    }
}

float GetPrice402(void)
{
    bool status = false;
    HTTPClient http;
    float price=0.0;
    //String path = "jobs/GetDetails?id={job.cost}";
    String path = "jobs/GetPaginatedPrintJobs";
    String requestBody = "{\"page\": 1, \"page_size\": 10, \"printer_ids\": [18491], \"cost\": [\"total_cost\", \"finished\"]}";
    http.begin((API_HOST + path));
    http.addHeader("accept", "application/json");
    http.addHeader("X-API-KEY", API_KEY);
    int httpResponseCode = http.POST(requestBody);
    if (httpResponseCode > 0) {
        String response = http.getString();
        const size_t capacity = 10 * JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2) + 1024;
        DynamicJsonDocument doc(capacity);
        deserializeJson(doc, response);
        //Serial.println(response);
        if (doc["status"] == true) {
            JsonArray data = doc["data"]; 
             for (JsonObject job : data) {
            if (job.containsKey("cost")) {
                JsonObject cost = job["cost"];
                
                // Lire le "total_cost"
                if (cost.containsKey("total_cost")) {
                    float totalCost = cost["total_cost"];
                    price = totalCost;
                    Serial.print("Total cost: ");
                    Serial.println(totalCost);
                } else {
                    Serial.println("No total cost found.");
                }
            }
        }
        }
        http.end();
    }
    return price;
}

float GetPriceMag(void)
{
    bool status = false;
    HTTPClient http;
    float price=0.0;
    //String path = "jobs/GetDetails?id={job.cost}";
    String path = "jobs/GetPaginatedPrintJobs";
    String requestBody = "{\"page\": 1, \"page_size\": 10, \"printer_ids\": [21937], \"cost\": [\"total_cost\", \"finished\"]}";
    http.begin((API_HOST + path));
    http.addHeader("accept", "application/json");
    http.addHeader("X-API-KEY", API_KEY);
    int httpResponseCode = http.POST(requestBody);
    if (httpResponseCode > 0) {
        String response = http.getString();
        const size_t capacity = 10 * JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2) + 1024;
        DynamicJsonDocument doc(capacity);
        deserializeJson(doc, response);
        //Serial.println(response);
        if (doc["status"] == true) {
            JsonArray data = doc["data"]; 
             for (JsonObject job : data) {
            if (job.containsKey("cost")) {
                JsonObject cost = job["cost"];
                
                // Lire le "total_cost"
                if (cost.containsKey("total_cost")) {
                    float totalCost = cost["total_cost"];
                    price = totalCost;
                    Serial.print("Total cost: ");
                    Serial.println(totalCost);
                } else {
                    Serial.println("No total cost found.");
                }
            }
        }
        }
        http.end();
    }
    return price;
}