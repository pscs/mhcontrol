#include "thingsboard.h"

#include <HTTPClient.h>
#include "victron_smartsolar.h"
#include "location.h"
#include "mybuffer.h"
#include "config.h"

void updateThingsBoard() {
    if (WiFi.status() != WL_CONNECTED) {
        return;
    }

    printf("Update Thingsboard\n");

    MyBuffer json(1000); 

    HTTPClient http;
    char server[200];


    
    printf("Update Thingsboard2\n");
    if (gps.location.isValid()) {
        snprintf(server, sizeof(server), "%s/api/v1/%s/telemetry",
            THINGSBOARDURL, THINGSBOARDAPIKEY);
        snprintf(json.getString(), json.getSize(), "{\"lat\":%s,\"lng\":%s}", 
            String(gps.location.lat(), 5).c_str(), String(gps.location.lng(), 5).c_str());
        http.begin(server);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(json.getString());
        http.end();
    }
        printf("Update Thingsboard3\n");

    if (victronSmartSolar.isConnected()) {
        snprintf(server, sizeof(server), "%s/api/v1/%s/telemetry",
            THINGSBOARDURL, THINGSBOARDSOLARAPIKEY);

        snprintf(json.getString(), json.getSize(), "{\"v0\":%d,\"v1\":%d, \"p0\":%d,\"p1\":%d,\"i0\":%d,\"i1\":%d,\"st\":%d,\"temp\":%d}", 
            victronSmartSolar.getVoltage(0), victronSmartSolar.getVoltage(1),
            victronSmartSolar.getPower(0),victronSmartSolar.getPower(1),
            victronSmartSolar.getCurrent(0),victronSmartSolar.getCurrent(1),
            victronSmartSolar.getState(), victronSmartSolar.getTemperature());
        http.begin(server);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(json.getString());
        http.end();
    }

}