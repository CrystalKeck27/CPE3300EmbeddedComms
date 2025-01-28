/**
 * Name: Crystal Keck
 * Course: CPE2610
 * Section: 121
 * Assignment: Lab 7 - Tunes
 * File: music.h
 * Dependencies: None
 */

#ifndef MUSIC_H_
#define MUSIC_H_

#include <stdint.h>
#include "piezo.h"

#define NOTE_COUNT 128

typedef struct Note {
    uint8_t midi;
    uint8_t divisor;
} Note;

void InitNotes(uint32_t clock_freq);

void PiezoPlayNote(Note note, uint32_t vol_percent, uint32_t beat_period, void (*callback)(void));

void PiezoSetSong(Note* song, uint32_t song_length, uint32_t vol_percent, uint32_t beat_period, bool backwards);

void PauseSong(void);

void ResumeSong(void);

#endif /* PIEZO_H_ */