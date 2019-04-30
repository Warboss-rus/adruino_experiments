#include "Sound.h"
#include <Arduino.h>

static const uint8_t tonePin = PB9;

#define       NOTE_C_0(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00000000)
#define      NOTE_CS_0(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00000001)
#define       NOTE_D_0(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00000010)
#define      NOTE_DS_0(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00000011)
#define       NOTE_E_0(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00000100)
#define       NOTE_F_0(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00000101)
#define      NOTE_FS_0(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00000110)
#define       NOTE_G_0(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00000111)
#define      NOTE_GS_0(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00001000)
#define       NOTE_A_0(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00001001)
#define      NOTE_AS_0(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00001010)
#define       NOTE_B_0(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00001011)

// Octave 1 Note Codes
#define       NOTE_C_1(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00010000)
#define      NOTE_CS_1(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00010001)
#define       NOTE_D_1(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00010010)
#define      NOTE_DS_1(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00010011)
#define       NOTE_E_1(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00010100)
#define       NOTE_F_1(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00010101)
#define      NOTE_FS_1(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00010110)
#define       NOTE_G_1(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00010111)
#define      NOTE_GS_1(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00011000)
#define       NOTE_A_1(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00011001)
#define      NOTE_AS_1(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00011010)
#define       NOTE_B_1(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00011011)

// Octave 2 Note Codes
#define       NOTE_C_2(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00100000)
#define      NOTE_CS_2(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00100001)
#define       NOTE_D_2(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00100010)
#define      NOTE_DS_2(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00100011)
#define       NOTE_E_2(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00100100)
#define       NOTE_F_2(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00100101)
#define      NOTE_FS_2(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00100110)
#define       NOTE_G_2(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00100111)
#define      NOTE_GS_2(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00101000)
#define       NOTE_A_2(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00101001)
#define      NOTE_AS_2(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00101010)
#define       NOTE_B_2(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00101011)

// Octave 3 Note Codes
#define       NOTE_C_3(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00110000)
#define      NOTE_CS_3(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00110001)
#define       NOTE_D_3(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00110010)
#define      NOTE_DS_3(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00110011)
#define       NOTE_E_3(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00110100)
#define       NOTE_F_3(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00110101)
#define      NOTE_FS_3(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00110110)
#define       NOTE_G_3(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00110111)
#define      NOTE_GS_3(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00111000)
#define       NOTE_A_3(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00111001)
#define      NOTE_AS_3(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00111010)
#define       NOTE_B_3(DURATION) ( (((uint16_t)DURATION)<<8) | 0b00111011)

// Octave 4 Note Codes
#define       NOTE_C_4(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01000000)
#define      NOTE_CS_4(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01000001)
#define       NOTE_D_4(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01000010)
#define      NOTE_DS_4(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01000011)
#define       NOTE_E_4(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01000100)
#define       NOTE_F_4(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01000101)
#define      NOTE_FS_4(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01000110)
#define       NOTE_G_4(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01000111)
#define      NOTE_GS_4(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01001000)
#define       NOTE_A_4(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01001001)
#define      NOTE_AS_4(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01001010)
#define       NOTE_B_4(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01001011)

// Octave 5 Note Codes
#define       NOTE_C_5(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01010000)
#define      NOTE_CS_5(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01010001)
#define       NOTE_D_5(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01010010)
#define      NOTE_DS_5(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01010011)
#define       NOTE_E_5(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01010100)
#define       NOTE_F_5(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01010101)
#define      NOTE_FS_5(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01010110)
#define       NOTE_G_5(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01010111)
#define      NOTE_GS_5(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01011000)
#define       NOTE_A_5(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01011001)
#define      NOTE_AS_5(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01011010)
#define       NOTE_B_5(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01011011)

// Octave 6 Note Codes
#define       NOTE_C_6(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01100000)
#define      NOTE_CS_6(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01100001)
#define       NOTE_D_6(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01100010)
#define      NOTE_DS_6(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01100011)
#define       NOTE_E_6(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01100100)
#define       NOTE_F_6(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01100101)
#define      NOTE_FS_6(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01100110)
#define       NOTE_G_6(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01100111)
#define      NOTE_GS_6(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01101000)
#define       NOTE_A_6(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01101001)
#define      NOTE_AS_6(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01101010)
#define       NOTE_B_6(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01101011)

// Octave 7 Note Codes
#define       NOTE_C_7(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01110000)
#define      NOTE_CS_7(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01110001)
#define       NOTE_D_7(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01110010)
#define      NOTE_DS_7(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01110011)
#define       NOTE_E_7(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01110100)
#define       NOTE_F_7(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01110101)
#define      NOTE_FS_7(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01110110)
#define       NOTE_G_7(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01110111)
#define      NOTE_GS_7(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01111000)
#define       NOTE_A_7(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01111001)
#define      NOTE_AS_7(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01111010)
#define       NOTE_B_7(DURATION) ( (((uint16_t)DURATION)<<8) | 0b01111011)

// Octave 8 Note Codes
#define       NOTE_C_8(DURATION) ( (((uint16_t)DURATION)<<8) | 0b10000000)
#define      NOTE_CS_8(DURATION) ( (((uint16_t)DURATION)<<8) | 0b10000001)
#define       NOTE_D_8(DURATION) ( (((uint16_t)DURATION)<<8) | 0b10000010)
#define      NOTE_DS_8(DURATION) ( (((uint16_t)DURATION)<<8) | 0b10000011)
#define       NOTE_E_8(DURATION) ( (((uint16_t)DURATION)<<8) | 0b10000100)
#define       NOTE_F_8(DURATION) ( (((uint16_t)DURATION)<<8) | 0b10000101)
#define      NOTE_FS_8(DURATION) ( (((uint16_t)DURATION)<<8) | 0b10000110)
#define       NOTE_G_8(DURATION) ( (((uint16_t)DURATION)<<8) | 0b10000111)
#define      NOTE_GS_8(DURATION) ( (((uint16_t)DURATION)<<8) | 0b10001000)
#define       NOTE_A_8(DURATION) ( (((uint16_t)DURATION)<<8) | 0b10001001)
#define      NOTE_AS_8(DURATION) ( (((uint16_t)DURATION)<<8) | 0b10001010)
#define       NOTE_B_8(DURATION) ( (((uint16_t)DURATION)<<8) | 0b10001011)

#define    NOTE_SILENT(DURATION) ((((uint16_t)DURATION)<<8) | 0b00001111)

static const uint16_t Melody0[] PROGMEM = {
  NOTE_C_5( 160 ),     NOTE_D_5( 160 ),    NOTE_DS_5( 160 ),     NOTE_C_5( 160 ),
  NOTE_D_5( 160 ),    NOTE_DS_5( 255 ),    NOTE_DS_5(  65 ),     NOTE_F_5( 160 ),
  NOTE_G_5( 160 ),    NOTE_DS_5( 160 ),     NOTE_F_5( 160 ),     NOTE_G_5( 160 ),
  NOTE_F_5( 160 ),     NOTE_G_5( 160 ),     NOTE_A_5( 160 ),     NOTE_F_5( 160 ),
  NOTE_G_5( 160 ),     NOTE_A_5( 160 ),    NOTE_GS_5( 160 ),     NOTE_B_5( 160 ),
  NOTE_C_6( 160 ),    NOTE_GS_5( 160 ),     NOTE_B_5( 160 ),     NOTE_C_6( 160 ),
  NOTE_E_6( 160 ),  NOTE_SILENT( 160 ),     NOTE_E_6( 230 ),     NOTE_SILENT( 25 ),
  NOTE_E_6( 230 ),   NOTE_SILENT( 25 ),     NOTE_E_6( 230 ),     NOTE_SILENT( 25 ),
  NOTE_E_6( 255 ),     NOTE_E_6( 100 ),
};
static const uint16_t Melody0_Length    = sizeof( Melody0 ) / sizeof(uint16_t);
// 8th Octave Frequencies C8 to B8, lower octaves are calculated from this
static const uint16_t Freq8[] = { 4186, 4435, 4699 , 4978, 5274, 5588, 5920, 6272 , 6645, 7040, 7459, 7902 };

struct MusicPlaying
{
  bool playing;
  const uint16_t* data;
  uint16_t length;
  uint32_t nextNote;
  int16_t delay;
  uint32_t lastPlayedTimestamp;
} music = {false, NULL, 0, 0, 0, 0};

void playIntroMusic()
{
  music = MusicPlaying{true, Melody0, Melody0_Length, 0, 0, millis() };
}

void setupSound()
{
  pinMode(tonePin, OUTPUT);
}

void updateSound()
{
  if (music.playing)
  {
    int playDuration = millis() - music.lastPlayedTimestamp;
    music.lastPlayedTimestamp = millis();
    for (uint16_t  x = music.nextNote; x < music.length; ++x)
    {
      if (music.delay > playDuration)
      {
        music.delay -= playDuration;
        return;
      }
      playDuration -= music.delay;

      uint16_t data = pgm_read_word((uint16_t*)&music.data[x]);
      if ((data & 0xF) == 0xF)
      {
        noTone(tonePin);
      }
      else
      {
        uint16_t Freq = Freq8[data & 0xF] / ( 1 << (8 - (data >> 4 & 0xF)));
        tone(tonePin, Freq);
      }

      int16_t duration = data >> 8;
      music.delay = duration;
      music.nextNote = x + 1;
    }
    noTone(tonePin);
    music.playing = false;
  }
}
