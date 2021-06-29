#pragma once

#include <vector>
#include <cstdint>

class BLEScan;
class MyBLEClient;
class BLEAddress;
class MySecurityCallbacks;

struct RegisteredBLEClient {
  RegisteredBLEClient(MyBLEClient &c): client(c) {}
  MyBLEClient &client;
  bool found = false;
};

class BLEManager {
public:
    void initialise();

    void connect();
    void keepAlive();

    void addBLEClient(MyBLEClient &client);
    void setFound(BLEAddress addr);

    void setPasscode(uint32_t passcode);
    //bool victronSolarFound = false;
    bool scanInProgress = false;

private:
    BLEScan *pScanner = nullptr;
    MySecurityCallbacks *pSecurityCallbacks = nullptr;
    std::vector<RegisteredBLEClient> clients; 
};

void BLEConnect();
void BLEKeepAlive();

extern BLEManager bleManager;

/* {
  victronSmartSolar.connectIfNecessary();
}*/