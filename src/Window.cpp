#include "Window.h"
#include <functional>
#include "Camera.h"
#include <iostream>
#include <mutex>
#include "Object.h"
#include <sstream>
#include "Object2d.h"

window::~window(){
    SDL_GL_DeleteContext(this->gl_context);
    SDL_DestroyWindow(this->app_window);
    SDL_Quit();
}

window::window() {
    this->title = "Default Window Title";
    this->create_window();
}

window::window(string title, camera* cam, int width, int height, bool fullscreen) : cam(cam), title(title), width(width), height(height), current_event(event()), fullscreen(fullscreen) {
    this->create_window();
}

void window::create_window() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::stringstream ss;
        ss << "could not initialize sdl2: " << SDL_GetError() << "\n";
        throw std::runtime_error(ss.str());
    }
    atexit(SDL_Quit);


    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    if (this->fullscreen) {
        this->app_window = SDL_CreateWindow(
            this->title.c_str(), 
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
            0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
        );
    } else {
        this->app_window = SDL_CreateWindow(
            this->title.c_str(),
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            this->width, this->height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
        );
    }
    if (this->app_window == NULL) {
        std::stringstream ss;
        ss << "could not create window: " << SDL_GetError() << "\n";
        throw std::runtime_error(ss.str());
    }

    this->gl_context = SDL_GL_CreateContext(this->app_window);
    if (this->gl_context == NULL) {
        throw std::runtime_error("Failed to create OpenGL context");
    }
    

    std::cout << "OpenGL loaded:\n";
    
    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        throw std::runtime_error("Failed to initialize GLAD");
    }
    std::cout << "Vendor:   " << (char *)glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << (char *)glGetString(GL_RENDERER) << "\n";
    std::cout << "Version:  " << (char *)glGetString(GL_VERSION) << "\n";


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    
    glViewport(0, 0, this->width, this->height);
    
    this->old_time = SDL_GetTicks();
    glViewport(0, 0, this->width, this->height);
    
    return;
}



void window::update() {
    this->new_time = SDL_GetTicks();
    this->deltatime = (this->new_time - this->old_time)/1000.0f;// dt in seconds
    this->current_event.handle_events(this);


    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    for (object* ob : render_list) {
        this->cam->recalculate_pv();
        ob->render(*this->cam);
    }

    for (object2d* ob : render_list2d) {
        ob->render(*this->cam);
    }
    
    SDL_GL_SwapWindow(this->app_window);
    this->old_time = new_time;
}

void window::add_object(object* obj) {
    this->render_list.insert(obj);
}

void window::remove_object(object* obj) {
    this->render_list.erase(obj);
}

void window::add_object_list(vector<object*> objs) {
    for (object * obj : objs) {
        this->render_list.insert(obj);
    }
}

void window::remove_object_list(vector<object*> objs) {
    for (object * obj : objs) {
        this->render_list.erase(obj);
    }
}

void window::add_object2d(object2d* obj) {
    this->render_list2d.insert(obj);
}

void window::remove_object2d(object2d* obj) {
    this->render_list2d.erase(obj);
}

void window::add_object2d_list(vector<object2d*> objs) {
    for (object2d * obj : objs) {
        this->render_list2d.insert(obj);
    }
}

void window::remove_object2d_list(vector<object2d*> objs) {
    for (object2d * obj : objs) {
        this->render_list2d.erase(obj);
    }
}