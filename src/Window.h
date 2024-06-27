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
#include <chrono>

#define SDLBOOL(b) b ? SDL_TRUE : SDL_FALSE

using std::string;
using std::vector;
class camera;
class object;
class object2d;

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
    double deltatime;
    long long time_ns, time;

    inline void lock_mouse(bool lock) {
        SDL_SetRelativeMouseMode(SDLBOOL(lock));
    }

    void add_object(object* obj);
    void remove_object(object* obj);
    void add_object_list(vector<object*> objs);
    void remove_object_list(vector<object*> objs);

    void add_object2d(object2d* obj);
    void remove_object2d(object2d* obj);
    void add_object2d_list(vector<object2d*> objs);
    void remove_object2d_list(vector<object2d*> objs);
private:
    void create_window();
    SDL_Window* app_window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    SDL_GLContext gl_context = NULL;
    std::chrono::steady_clock::time_point old_time, new_time, starttime;
    std::set<object*> render_list;
    std::set<object2d*> render_list2d;
};