#pragma once
#include "Vec3.h"
#include <SDL2/SDL.h>
#include <vector>
#include <set>
#include <string>
#include <sstream>

using std::vector;
using std::set;
using std::string;

class window;
class sound;

extern std::set<sound*>* global_sounds;

void audio_callback(void *userdata, Uint8 *stream, int len);

class audio_mixer {
public:
    audio_mixer() {
        wav_spec.freq = 44100;
        wav_spec.format = AUDIO_S16LSB;
        wav_spec.channels = 2;
        wav_spec.samples = 4096;
        wav_spec.callback = audio_callback;
        wav_spec.userdata = nullptr;
        audio_device = SDL_OpenAudioDevice(NULL, 0, &wav_spec, NULL, 0);
        if (audio_device == 0) {
            std::stringstream ss;
            ss << "Could not open audio device: " << SDL_GetError() << "\n";
            throw std::runtime_error(ss.str());
        }
        SDL_PauseAudioDevice(audio_device, 0);
    }

    ~audio_mixer();

    void add_sound(sound* s);

    void remove_sound(sound* s);

    SDL_AudioSpec wav_spec = {};
    Uint32 audio_device;
};

// sound stores sound assets to be played or used in positional sound class.
class sound {
public:
    sound(){}
    sound(window* win, string src, bool loop);

    inline void load(const string & src) {
        Uint8 *t_data;
        Uint32 t_length;
        if (SDL_LoadWAV(src.c_str(), &mixer->wav_spec, &t_data, &t_length) == NULL) {
            std::stringstream ss;
            ss << "Could not load WAV file: " << SDL_GetError() << "\n";
            throw std::runtime_error(ss.str());
        }

        data = vector<Uint8>(t_data, t_data + t_length);

        SDL_FreeWAV(t_data);
    }

    inline void play(float volume, float panning = 0.0f, float pitch = 1.0f) { // Plays a sound globaly
        audio_position = 0;
        this->volume = volume;
        this->panning = panning;
        this->pitch = pitch;
        mixer->add_sound(this);
    }

    inline void play(const vec3& position, float volume, float panning = 0.0f, float pitch = 1.0f) { // Plays a sound at a position
        audio_position = 0;
        this->volume = volume;
        this->panning = panning;
        this->pitch = pitch;
        mixer->add_sound(this);
    }

    inline void stop() {
        mixer->remove_sound(this);
    }

    float volume = 1.0f;

    float panning = 0.0f;

    float pitch = 1.0f;

    vector<Uint8> data;
    unsigned int audio_position = 0;
    audio_mixer* mixer = nullptr;
    bool loop;
};

class sound3d : sound {
public:
    sound3d(){}
    
};