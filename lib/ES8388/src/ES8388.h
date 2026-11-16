#ifndef ES8388_H_
#define ES8388_H_

#include <Arduino.h>
#include <Wire.h>
#pragma once
#include <stdint.h>

typedef enum {
  MIXIN1,  // direct line 1
  MIXIN2,  // direct line 2
  MIXRES,  // reserverd es8388
  MIXADC   // Select from ADC/ALC
} mixsel_t;

typedef enum {
  OUT1,    // Select Line OUT L/R 1
  OUT2,    // Select Line OUT L/R 2
  OUTALL,  // Enable ALL
} outsel_t;

typedef enum {
  IN1,      // Select Line IN L/R 1
  IN2,      // Select Line IN L/R 2
  IN1DIFF,  // differential IN L/R 1
  IN2DIFF   // differential IN L/R 2
} insel_t;

typedef enum {
  DACOUT,     // Select Sink From DAC
  SRCSELOUT,  // Select Sink From SourceSelect()
  MIXALL,     // Sink ALL DAC & SourceSelect()
} mixercontrol_t;

typedef enum {
  DISABLE,  // Disable ALC
  GENERIC,  // Generic Mode
  VOICE,    // Voice Mode
  MUSIC     // Music Mode
} alcmodesel_t;

class ES8388 {
 private:
  outsel_t _outSel = OUTALL;
  insel_t _inSel = IN1;
  // I2C bus the codec is connected to. Shared by all instances, so that several
  // ES8388 objects (e.g. one per source file) talk to the same chip over the same bus.
  static TwoWire* _wire;
  bool write_reg(uint8_t reg_add, uint8_t data);
  bool read_reg(uint8_t reg_add, uint8_t& data);

 public:
  // Uses the bus selected with setBus(). Does not touch any hardware, so it is
  // safe for global objects.
  ES8388();
  // Original constructor: starts Wire (I2C bus 0) on the given pins and uses it.
  ES8388(uint8_t _sda, uint8_t _scl, uint32_t _speed);
  ~ES8388();
  // Select an I2C bus that the application has already started with begin().
  static void setBus(TwoWire& wire);
  bool init();
  // Mutes the outputs and puts the codec into its low-power standby state.
  // init() has to be called again before the codec can be used afterwards.
  bool power_down();
  bool identify(int sda, int scl, uint32_t frequency);
  uint8_t* readAllReg();
  bool outputSelect(outsel_t sel);
  bool inputSelect(insel_t sel);
  bool DACmute(bool mute);
  uint8_t getOutputVolume();
  bool setOutputVolume(uint8_t vol);
  uint8_t getInputGain();
  bool setInputGain(uint8_t gain);
  bool setALCmode(alcmodesel_t alc);
  bool mixerSourceSelect(mixsel_t LMIXSEL, mixsel_t RMIXSEL);
  bool mixerSourceControl(bool LD2LO, bool LI2LO, uint8_t LI2LOVOL, bool RD2RO,
                          bool RI2RO, uint8_t RI2LOVOL);
  bool mixerSourceControl(mixercontrol_t mix);
  bool analogBypass(bool bypass);
};

#endif