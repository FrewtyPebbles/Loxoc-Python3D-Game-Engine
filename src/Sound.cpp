#include "Sound.h"
#include "Window.h"

static set<sound*> * global_sounds = new set<sound*>();

void apply_pan(Uint8* stream, Uint32 len, float pan) {
    // clamp pan
    pan = (pan < -1.0f) ? -1.0f : (pan > 1.0f) ? 1.0f : pan;

    // Calculate the volume scale for each channel
    float left_volume = (pan < 0.0f) ? 1.0f : 1.0f - pan;  // Reduce right volume as pan moves left
    float right_volume = (pan > 0.0f) ? 1.0f : 1.0f + pan; // Reduce left volume as pan moves right

    // Ensure volumes are in the correct range
    left_volume = SDL_MIX_MAXVOLUME * left_volume;
    right_volume = SDL_MIX_MAXVOLUME * right_volume;

    // len is the length in bytes, convert to 16-bit samples for stereo audio
    Sint16* samples = (Sint16*)stream;
    Uint32 sample_count = len / sizeof(Sint16);

    for (Uint32 i = 0; i < sample_count; i += 2) {
        Sint16 left_sample = samples[i];   // Left channel sample
        Sint16 right_sample = samples[i+1]; // Right channel sample

        // Apply panning by scaling each channel independently
        samples[i]   = static_cast<Sint16>(left_sample * (left_volume / SDL_MIX_MAXVOLUME));
        samples[i+1] = static_cast<Sint16>(right_sample * (right_volume / SDL_MIX_MAXVOLUME));
    }
}

void audio_callback(void *userdata, Uint8 *stream, int len) {
    SDL_memset(stream, 0, len);  // Initialize the output buffer to silence

    // Temporary buffer to accumulate audio samples
    Uint8* mix_buffer = new Uint8[len];
    SDL_memset(mix_buffer, 0, len);  // Clear the mixing buffer

    int active_sounds = 0;

    // Count how many sounds are currently playing
    for (auto it = global_sounds->begin(); it != global_sounds->end(); ++it) {
        sound* audio = *it;
        if (audio->audio_position < audio->data.size()) {
            active_sounds++;
        }
    }

    // If no sounds, return early
    if (active_sounds == 0) {
        delete[] mix_buffer;
        return;
    }

    // Mix all active audio sources into the mix_buffer
    for (auto it = global_sounds->begin(); it != global_sounds->end(); ) {
        sound* audio = *it;
        if (audio->audio_position < audio->data.size()) {
            // Determine how many bytes we can mix
            Uint32 bytes_to_mix = (audio->data.size() - audio->audio_position);
            if (bytes_to_mix > (Uint32)len) {
                bytes_to_mix = len;
            }

            // Mix the audio into the temporary mix_buffer
            SDL_MixAudioFormat(mix_buffer, &audio->data[audio->audio_position], AUDIO_S16LSB, bytes_to_mix, SDL_MIX_MAXVOLUME * audio->volume);

            apply_pan(mix_buffer, bytes_to_mix, audio->panning);

            // Update the audio position
            audio->audio_position += bytes_to_mix;

            if (audio->audio_position >= audio->data.size() && audio->loop) {
                audio->audio_position = 0;  // Reset position if looping
            } else if (audio->audio_position >= audio->data.size()) {
                // Remove the sound if finished playing
                it = global_sounds->erase(it);
                continue;
            }
        }

        ++it;
    }

    // Copy the mixed result from mix_buffer into the stream buffer
    SDL_memcpy(stream, mix_buffer, len);

    // Clean up
    delete[] mix_buffer;
}

sound::sound(window* win, string src, bool loop): mixer(win->sound_mixer), loop(loop){
    load(src);
}

audio_mixer::~audio_mixer() {
    SDL_CloseAudioDevice(audio_device);
    delete global_sounds;
}

void audio_mixer::add_sound(sound* s) {
    global_sounds->insert(s);
}

void audio_mixer::remove_sound(sound* s) {
    global_sounds->erase(s);
}