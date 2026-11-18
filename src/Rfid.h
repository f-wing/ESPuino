#pragma once

#include <atomic>

constexpr uint8_t cardIdSize = 4u;
constexpr uint8_t cardIdStringSize = (cardIdSize * 3u) + 1u;

extern char gCurrentRfidTagId[cardIdStringSize];
// [custom] Set by the reader tasks right before they queue a card that was put on the reader, as opposed to the
// other sources of gRfidCardQueue (virtual cards, MQTT, resuming after an announcement, WiFi retry, ...).
// Used for "no autoplay when a card is presented".
extern std::atomic<bool> gRfidCardFromReader;

void Rfid_ResetOldRfid(void);
void Rfid_ResetLastTag(void);
// Reader-task internal: returns true once after Rfid_ResetLastTag() was called.
bool Rfid_ConsumeLastTagReset(void);
void Rfid_Init(void);
void Rfid_WakeupHandling(void);
void Rfid_StartTask(void);
void Rfid_Cyclic(void);
void Rfid_Exit(void);
void Rfid_TaskPause(void);
void Rfid_TaskResume(void);
void Rfid_TaskReset(void);
void Rfid_WakeupCheck(void);
void Rfid_PreferenceLookupHandler(void);
// Writes one card assignment to NVS, see RfidCommon.cpp.
bool Rfid_SaveAssignment(const char *cardId, const char *fileOrUrl, uint8_t playModeOrModId,
	uint32_t lastPlayPos = 0, uint16_t trackLastPlayed = 0);
