#include "sound.h"

#include <Arduino.h>
#include "tasks.h"
#include "logger.h"

Sound sound;

void fnProcessSoundQueue();

void fnProcessSoundQueue() {
	logger.send(LOG_SOUND, LOG_DEBUG, "process sound queue\n");
	sound.playNextSound();	
}

Sound::Sound() {
}

Sound::~Sound() {
	free(soundQueue);
}

void Sound::initialise(uint8_t chan_, uint16_t queueSize) {
	chan = chan_;
	maxQueue = queueSize;
	soundQueue = reinterpret_cast<SoundNote *>(ps_malloc(queueSize * sizeof(SoundNote)));

	tSound = new Task(100, -1, &fnProcessSoundQueue, &ts, false);
}

bool Sound::addNote(uint16_t freq, uint16_t duration) {
	logger.send(LOG_SOUND, LOG_DEBUG, "addNote\n");
	if (!soundQueue) {
		logger.send(LOG_SOUND, LOG_ERROR, "No buffer\n");
		return false;
	}
	uint16_t next = (queueIn + 1) % maxQueue;
	if (next == queueOut) {
		logger.send(LOG_SOUND, LOG_WARNING, "No space\n");
		return false;
	}
	soundQueue[queueIn].freq = freq;
	soundQueue[queueIn].duration = duration;

	if (queueIn == queueOut) { //queue was empty
		logger.send(LOG_SOUND, LOG_DEBUG, "Enable sound task\n");
		tSound->enable();
	}
	queueIn = next;
	logger.send(LOG_SOUND, LOG_DEBUG, "Done\n");
	return true;
}

void Sound::playNextSound() {
	logger.send(LOG_SOUND, LOG_DEBUG, "play next sound\n");
	if (queueIn == queueOut) {
		logger.send(LOG_SOUND, LOG_DEBUG, "No more sounds\n");
		ledcWriteTone(chan, 0);
		tSound->disable();
	} else {
		logger.send(LOG_SOUND, LOG_DEBUG, "Play sound\n");
		ledcWriteTone(chan, soundQueue[queueOut].freq);
		tSound->enableDelayed(soundQueue[queueOut].duration);
		queueOut = (queueOut + 1) % maxQueue;
	}
}
