#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#include <string>
#include <thread>
#include "glad/gl.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <vector>


using std::string;
using std::vector;
class camera;
class object;

enum class event {
    NOTHING,

    // WINDOW MANAGEMENT
    WINDOW_CLOSE,
    QUIT,

    // KEYS
    KEY_UP,
    KEY_DOWN,
    KEY_RIGHT,
    KEY_LEFT,
    KEY_SPACE
};

class window {
public:
    window();
    window(string title, camera* cam, int width, int height);
    ~window();
    camera* cam;
    string title;
    int width, height;
    event current_event;
    bool fullscreen = false;
    void update(vector<object*> objects);
    float deltatime;
private:
    void create_window();
    SDL_Window* app_window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    SDL_GLContext gl_context = NULL;
    uint32_t old_time, new_time;
};