#define A0 21
#define A_0 22
#define B0 23
#define C1 24
#define C_1 25
#define D1 26
#define D_1 27
#define E1 28
#define F1 29
#define F_1 30
#define G1 31
#define G_1 32
#define A1 33
#define A_1 34
#define B1 35
#define C2 36
#define C_2 37
#define D2 38
#define D_2 39
#define E2 40
#define F2 41
#define F_2 42
#define G2 43
#define G_2 44
#define A2 45
#define A_2 46
#define B2 47
#define C3 48
#define C_3 49
#define D3 50
#define D_3 51
#define E3 52
#define F3 53
#define F_3 54
#define G3 55
#define G_3 56
#define A3 57
#define A_3 58
#define B3 59
#define C4 60
#define C_4 61
#define D4 62
#define D_4 63
#define E4 64
#define F4 65
#define F_4 66
#define G4 67
#define G_4 68
#define A4 69
#define A_4 70
#define B4 71
#define C5 72
#define C_5 73
#define D5 74
#define D_5 75
#define E5 76
#define F5 77
#define F_5 78
#define G5 79
#define G_5 80
#define A5 81
#define A_5 82
#define B5 83
#define C6 84
#define C_6 85
#define D6 86
#define D_6 87
#define E6 88
#define F6 89
#define F_6 90
#define G6 91
#define G_6 92
#define A6 93
#define A_6 94
#define B6 95
#define C7 96
#define C_7 97
#define D7 98
#define D_7 99
#define E7 100
#define F7 101
#define F_7 102
#define G7 103
#define G_7 104
#define A7 105
#define A_7 106
#define B7 107
#define C8 108

#define buttonCount 12 // Number of touch pads
#define jump 2         // Jump CC value

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

#define MAJOR 0
#define MINOR 1

bool ccMode = false;

int CCval[] = { 64, 64, 64, 64, 64, 64 }; // Default CC Values

int Offsets[2][15] = {
  { 0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21, 23, 24 }, // Major
  { 0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24 }  // Minor
};

int notes[11];

uint16_t lasttouched = 0;
uint16_t currtouched = 0;
uint8_t  offsetNum;


void makeScale(int _root, int _scale) {
  for ( int v = 0; v < buttonCount; v++ ) {
    notes[v] = _root + Offsets[_scale][v];
  }
}
