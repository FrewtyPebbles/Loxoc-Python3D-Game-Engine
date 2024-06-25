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
#include "Event.h"
#include <set>

#define SDLBOOL(b) b ? SDL_TRUE : SDL_FALSE

using std::string;
using std::vector;
class camera;
class object;

class window {
public:
    window();
    window(string title, camera* cam, int width, int height, bool fullscreen);
    ~window();
    camera* cam;
    string title;
    int width, height;
    event current_event;
    bool fullscreen = false;
    void update();
    float deltatime;
    inline void lock_mouse(bool lock) {
        SDL_SetRelativeMouseMode(SDLBOOL(lock));
    }

    void add_object(object* obj);
    void remove_object(object* obj);
    void add_object_list(vector<object*> objs);
    void remove_object_list(vector<object*> objs);
private:
    void create_window();
    SDL_Window* app_window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    SDL_GLContext gl_context = NULL;
    uint32_t old_time, new_time;
    std::set<object*> render_list;
};