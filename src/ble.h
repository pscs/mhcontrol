#pragma once

#include <vector>
#include <cstdint>

class NimBLEScan;
class MyBLEClient;
class NimBLEAddress;
class MySecurityCallbacks;
class MyBLEServer;

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
    void setFound(NimBLEAddress addr);

	bool isServerPinRequired() const;
	void requestServerPin();
	void closeServerPinScreen();

    bool scanInProgress = false;

private:
	uint32_t scanStarted = 0;
    NimBLEScan *pScanner = nullptr;
    MySecurityCallbacks *pSecurityCallbacks = nullptr;
    std::vector<RegisteredBLEClient> clients; 
	MyBLEServer *server = nullptr;
};

void BLEConnect();
void BLEKeepAlive();

extern BLEManager bleManager;
