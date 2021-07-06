#include "thingsboard.h"

#include <HTTPClient.h>
#include "victron_smartsolar.h"
#include "victron_mainscharger.h"
#include "victron_B2B.h"
#include "ksenergy_battery.h"
#include "location.h"
#include "mybuffer.h"
#include "config.h"
#include "logger.h"

void updateThingsBoard() {
    if (WiFi.status() != WL_CONNECTED) {
        return;
    }
    return;

    logger.send(LOG_THINGSBOARD, LOG_VERBOSE, "Update Thingsboard\n");

    //MyBuffer json(1000); 

    HTTPClient http;
    char server[200];

    char json[1000];


    
    logger.send(LOG_THINGSBOARD, LOG_DEBUG, "Update Thingsboard2\n");
    if (gps.location.isValid()) {
        snprintf(server, sizeof(server), "%s/api/v1/%s/telemetry",
            THINGSBOARDURL, THINGSBOARDAPIKEY);
        logger.send(LOG_THINGSBOARD, LOG_DEBUG, "Update Thingsboard 2a\n");
        snprintf(json, sizeof(json), "{\"lat\":%s,\"lng\":%s}", 
            String(gps.location.lat(), 5).c_str(), String(gps.location.lng(), 5).c_str());
        logger.send(LOG_THINGSBOARD, LOG_DEBUG, "Update Thingsboard 2b\n");
        http.begin(server);
        http.addHeader("Content-Type", "application/json");
        logger.send(LOG_THINGSBOARD, LOG_DEBUG, "Update Thingsboard 2m\n");
        logger.printf(LOG_THINGSBOARD, LOG_DEBUG, "%s\n", server);
        logger.printf(LOG_THINGSBOARD, LOG_DEBUG, "%s\n", json);
        logger.send(LOG_THINGSBOARD, LOG_DEBUG, "Update Thingsboard 2n\n");
        int httpResponseCode = http.POST(json);
        logger.send(LOG_THINGSBOARD, LOG_DEBUG, "Update Thingsboard 2o\n");

        http.end();
    }
        logger.send(LOG_THINGSBOARD, LOG_DEBUG, "Update Thingsboard3\n");

    if (victronSmartSolar.isConnected()) {
        snprintf(server, sizeof(server), "%s/api/v1/%s/telemetry",
            THINGSBOARDURL, THINGSBOARDSOLARAPIKEY);

        snprintf(json, sizeof(json), "{\"v0\":%d,\"v1\":%d, \"p0\":%d,\"p1\":%d,\"i0\":%d,\"i1\":%d,\"st\":%d,\"temp\":%d}", 
            victronSmartSolar.getVoltage(0), victronSmartSolar.getVoltage(1),
            victronSmartSolar.getPower(0),victronSmartSolar.getPower(1),
            victronSmartSolar.getCurrent(0),victronSmartSolar.getCurrent(1),
            victronSmartSolar.getState(), victronSmartSolar.getTemperature());
        http.begin(server);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(json);
        http.end();
    }

    if (victronB2B.isConnected()) {
        snprintf(server, sizeof(server), "%s/api/v1/%s/telemetry",
            THINGSBOARDURL, THINGSBOARDB2BAPIKEY);

        snprintf(json, sizeof(json), "{\"v0\":%d,\"v1\":%d, \"p0\":%d,\"p1\":%d,\"i0\":%d,\"i1\":%d,\"st\":%d,\"temp\":%d}", 
            victronB2B.getVoltage(0), victronB2B.getVoltage(1),
            victronB2B.getPower(0),victronB2B.getPower(1),
            victronB2B.getCurrent(0),victronB2B.getCurrent(1),
            victronB2B.getState(), victronB2B.getTemperature());
        http.begin(server);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(json);
        http.end();
    }

    if (victronMainsCharger.isConnected()) {
        snprintf(server, sizeof(server), "%s/api/v1/%s/telemetry",
            THINGSBOARDURL, THINGSBOARDMAINSAPIKEY);

        snprintf(json, sizeof(json), "{\"v0\":%d,\"v1\":%d, \"p0\":%d,\"p1\":%d,\"i0\":%d,\"i1\":%d,\"st\":%d,\"temp\":%d}", 
            victronMainsCharger.getVoltage(0), victronMainsCharger.getVoltage(1),
            victronMainsCharger.getPower(0),victronMainsCharger.getPower(1),
            victronMainsCharger.getCurrent(0),victronMainsCharger.getCurrent(1),
            victronMainsCharger.getState(), victronMainsCharger.getTemperature());
        http.begin(server);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(json);
        http.end();
    }

    if (ksEnergyBattery.isConnected()) {
        snprintf(server, sizeof(server), "%s/api/v1/%s/telemetry",
            THINGSBOARDURL, THINGSBOARDBATTERYAPIKEY);

        snprintf(json, sizeof(json), "{\"v\":%d,\"i\":%d,\"soc\":%d,\"st\":%d,\"temp\":%d}", 
            ksEnergyBattery.getVoltage(), 
            ksEnergyBattery.getCurrent(), ksEnergyBattery.getSOC(),
            ksEnergyBattery.getState(), ksEnergyBattery.getTemperature());
        http.begin(server);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(json);
        http.end();
    }

}