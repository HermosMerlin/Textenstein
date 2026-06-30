#include "sound.h"
#include "config.h"

static short make_sine_sample(double t, double freq, double volume) {
    double v = sin(2.0 * PI * freq * t);
    return (short)(v * volume * 32767.0);
}

void sound_play_tone(double freq, double duration, double volume) {
    int sample_count = (int)(SAMPLE_RATE * duration);
    short samples[SAMPLE_RATE];

    for (int i = 0; i < sample_count; i++) {
        double t = (double)i / SAMPLE_RATE;
        samples[i] = make_sine_sample(t, freq, volume);
    }

    sound_play_buffer(samples, sample_count);
}

void sound_play_buffer(short* samples, int sample_count) {
    HWAVEOUT device;

    WAVEFORMATEX format = {0};
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 1;
    format.nSamplesPerSec = SAMPLE_RATE;
    format.wBitsPerSample = 16;
    format.nBlockAlign = format.nChannels * format.wBitsPerSample / 8;
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;

    if (waveOutOpen(&device, WAVE_MAPPER, &format, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
        return;
    }

    WAVEHDR header = {0};
    header.lpData = (LPSTR)samples;
    header.dwBufferLength = sample_count * sizeof(short);

    waveOutPrepareHeader(device, &header, sizeof(header));
    waveOutWrite(device, &header, sizeof(header));

    while (!(header.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }

    waveOutUnprepareHeader(device, &header, sizeof(header));
    waveOutClose(device);
}