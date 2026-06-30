#ifndef SOUND_H
#define SOUND_H

#include <math.h>
#include <Windows.h>
#include <mmsystem.h>

void sound_play_tone(double freq, double duration, double volume);
void sound_play_buffer(short* samples, int sample_count);

#endif