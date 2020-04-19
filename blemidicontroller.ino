#include <MIDI.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"
#include "Notes.h"
#include <bluefruit.h>

BLEDis bledis;
BLEMidi blemidi;
Adafruit_MPR121 cap; // = Adafruit_MPR121();

MIDI_CREATE_INSTANCE(BLEMidi, blemidi, MIDI);

int root  = C3;
int scale = MAJOR;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello!");

  makeScale(root, scale);

  Bluefruit.begin();
  Bluefruit.Periph.setConnInterval(9, 16);
  Bluefruit.setTxPower(4);
  Bluefruit.setName("MIDI Thingy");

  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();

  MIDI.begin(MIDI_CHANNEL_OMNI);

  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(blemidi);
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);
  Bluefruit.ScanResponse.addName();
  Bluefruit.Advertising.restartOnDisconnect(false);
  Bluefruit.Advertising.setInterval(32, 244);
  Bluefruit.Advertising.start(0);
  
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found");
    while (1);
  }
  Serial.println("MPR121 found!");
  
  Serial.print("Waiting for bluetooth...");
  while (!Bluefruit.connected()) { Serial.print('.'); delay(1000); }
  Serial.println("bluetooth connected!");
  Serial.print("\nWaiting for MIDI...");
  while (!blemidi.notifyEnabled()) { Serial.print('.'); delay(1000); }
  Serial.println("MIDI connected!");
}

void loop() {
  
  while (Bluefruit.connected() && blemidi.notifyEnabled()) {
    currtouched = cap.touched();
    if (currtouched == lasttouched) return;
    //Serial.println(currtouched); // debuging

    switch(currtouched) {  // special cmds / do stuff
      case 64:  // Pad 6
        delay(50); // debounce
        currtouched = cap.touched();
        //Serial.println(currtouched);
        if (currtouched == 96) { // Pads 6 & 5
          ccMode = !ccMode;
          Serial.println(ccMode ? "CC mode ON" : "CC mode off");
        }
        else readNotes();
        break;
      default:
        readNotes();
        break;
    }

    lasttouched = currtouched;
  }
}

void readNotes() {
  for (int i=0; i<buttonCount; i++) {
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print("\nOn: "); Serial.println(i);
      
      if (!ccMode) MIDI.sendNoteOn(notes[i], 100, 1);
      else {
        if (i <= 4 && i >= 0) {
          CCval[i] = CCval[i] - jump;
          if (CCval[i] < 0) {
            CCval[i] = 0;
            return;
          }
          MIDI.send(midi::ControlChange, i, CCval[i], 1);
          Serial.print("CC: ");Serial.print(i);Serial.print("Val: ");Serial.println(CCval[i]);
        }
        if (i >= 7 && i <= 11) {
          offsetNum = correct(i);
          CCval[offsetNum] = CCval[offsetNum] + jump;
          if (CCval[offsetNum] > 127) {
            CCval[offsetNum] = 127;
            return;
          }
          MIDI.send(midi::ControlChange, offsetNum, CCval[offsetNum], 1);
          Serial.print("CC: ");Serial.print(offsetNum);Serial.print("Val: ");Serial.println(CCval[offsetNum]);
        }
      }
    }
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print("\nOff: "); Serial.println(i);
      
      if (!ccMode) MIDI.sendNoteOff(notes[i], 0, 1);
    }
  }
}

int correct(int number) {
  if (number <= 5) return number;
  if (number >= 6) {
    int offsets[] = { 5, 4, 3, 2, 1, 0 };
    return offsets[number - 6];
  }
}
