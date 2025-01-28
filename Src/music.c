
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "music.h"
#include "piezo.h"
#include "lcd.h"

static uint32_t notes[NOTE_COUNT] = {0};

static const char *note_names[] = {
    "C_N",
    "C#N",
    "D_N",
    "D#N",
    "E_N",
    "F_N",
    "F#N",
    "G_N",
    "G#N",
    "A_N",
    "A#N",
    "B_N",
    "C_0",
    "C#0",
    "D_0",
    "D#0",
    "E_0",
    "F_0",
    "F#0",
    "G_0",
    "G#0",
    "A_0",
    "A#0",
    "B_0",
    "C_1",
    "C#1",
    "D_1",
    "D#1",
    "E_1",
    "F_1",
    "F#1",
    "G_1",
    "G#1",
    "A_1",
    "A#1",
    "B_1",
    "C_2",
    "C#2",
    "D_2",
    "D#2",
    "E_2",
    "F_2",
    "F#2",
    "G_2",
    "G#2",
    "A_2",
    "A#2",
    "B_2",
    "C_3",
    "C#3",
    "D_3",
    "D#3",
    "E_3",
    "F_3",
    "F#3",
    "G_3",
    "G#3",
    "A_3",
    "A#3",
    "B_3",
    "C_4",
    "C#4",
    "D_4",
    "D#4",
    "E_4",
    "F_4",
    "F#4",
    "G_4",
    "G#4",
    "A_4",
    "A#4",
    "B_4",
    "C_5",
    "C#5",
    "D_5",
    "D#5",
    "E_5",
    "F_5",
    "F#5",
    "G_5",
    "G#5",
    "A_5",
    "A#5",
    "B_5",
    "C_6",
    "C#6",
    "D_6",
    "D#6",
    "E_6",
    "F_6",
    "F#6",
    "G_6",
    "G#6",
    "A_6",
    "A#6",
    "B_6",
    "C_7",
    "C#7",
    "D_7",
    "D#7",
    "E_7",
    "F_7",
    "F#7",
    "G_7",
    "G#7",
    "A_7",
    "A#7",
    "B_7",
    "C_8",
    "C#8",
    "D_8",
    "D#8",
    "E_8",
    "F_8",
    "F#8",
    "G_8",
    "G#8",
    "A_8",
    "A#8",
    "B_8",
    "C_9",
    "C#9",
    "D_9",
    "D#9",
    "E_9",
    "F_9",
    "F#9",
    "G_9",
    "G#9",
    "A_9",
    "A#9",
    "B_9"
};

Note *song_s;
int32_t song_length_s;
int32_t song_index_s;
uint32_t vol_percent_s;
uint32_t beat_period_s;
bool direction_s;
bool is_playing_s;

void piezoPlaySongRecursion(void);

void piezoPlaySongRecursion(void)
{
    if (!is_playing_s)
    {
        return;
    }
    if (song_index_s >= song_length_s)
    {
        is_playing_s = false;
        return;
    }
    if (song_index_s < 0)
    {
        is_playing_s = false;
        return;
    }
    Note note = *song_s;
    char str[16];
    sprintf(str, "%d/%d ", song_index_s+1, song_length_s);
    if (direction_s)
    {
        song_s++;
        song_index_s++;
    }
    else
    {
        song_s--;
        song_index_s--;
    }
    
    LcdSetPosition(1, 0);
    LcdWriteStr(str);
    LcdSetPosition(1, 13);
    LcdWriteStr(note_names[note.midi]);

    PiezoPlayNote(note, vol_percent_s, beat_period_s, piezoPlaySongRecursion);
}

void InitNotes(uint32_t clock_freq)
{
    volatile uint32_t *fpu = (uint32_t *)0xE000ED88;
    *fpu |= (0xF << 20); // Enable FPU
    const float a4 = 440;
    const uint8_t midi_a4 = 69; // Nice
    for (uint8_t i = 0; i < NOTE_COUNT; i++)
    {
        float n = (i - midi_a4) / 12.0;
        float freq = a4 * powf(2, n);
        float period = clock_freq / freq;
        notes[i] = (uint32_t)period;
    }
}

void PiezoPlayNote(Note note, uint32_t vol_percent, uint32_t beat_period, void (*callback)(void))
{
    uint32_t period = notes[note.midi];
    uint32_t duty = (period * vol_percent) / 200;
    uint32_t duration = beat_period / note.divisor;
    PiezoPlayPeriodNoBlock(period, duty, duration, callback);
}

void PiezoSetSong(Note *song, uint32_t song_length, uint32_t vol_percent, uint32_t beat_period, bool direction)
{
    song_s = song;
    song_length_s = (int32_t) song_length;
    direction_s = direction;
    if (direction)
    {
        song_index_s = 0;
    }
    else
    {
        song_s += song_length - 1;
        song_index_s = song_length - 1;
    }
    is_playing_s = false;
    vol_percent_s = vol_percent;
    beat_period_s = beat_period;
}

void PauseSong(void)
{
    is_playing_s = false;
}

void ResumeSong(void)
{
    is_playing_s = true;
    piezoPlaySongRecursion();
}