#pragma once

#include <cstdint>
#include "tasks.h"

struct SoundNote {
	uint16_t freq;
	uint16_t duration;
};

class Sound {
public:
	Sound();
	~Sound();

	void initialise(uint8_t chan, uint16_t queueSize);

	void playNextSound();
	bool addNote(uint16_t freq, uint16_t duration);
	void clear();

private:
	Task *tSound;
	uint8_t chan = 0;
	SoundNote *soundQueue = nullptr;
	uint16_t maxQueue = 20;
	uint16_t queueIn = 0;
	uint16_t queueOut = 0;
};

extern Sound sound;
