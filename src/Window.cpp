#include "Window.h"
#include <functional>
#include "Camera.h"
#include <iostream>
#include <mutex>
#include "Object3d.h"
#include <sstream>
#include "Object2d.h"
#include "Octree.h"
#include "Mesh.h"

window::~window(){
    SDL_GL_DeleteContext(this->gl_context);
    SDL_DestroyWindow(this->app_window);
    SDL_Quit();
}

window::window() {
    this->title = "Default Window Title";
    this->create_window();
}

window::window(string title, camera* cam, int width, int height, bool fullscreen, vec3 * ambient_light, skybox* sky_box) : cam(cam), title(title), width(width), height(height), current_event(event()), fullscreen(fullscreen), ambient_light(ambient_light), sky_box(sky_box) {
    this->create_window();
}

void window::create_window() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::stringstream ss;
        ss << "could not initialize sdl2: " << SDL_GetError() << "\n";
        throw std::runtime_error(ss.str());
    }
    atexit(SDL_Quit);

    SDL_GL_SetSwapInterval(0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
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
    
    
    glViewport(0, 0, this->width, this->height);
    this->old_time = this->starttime = std::chrono::steady_clock::now();
    return;
} 



void window::update() {
    
    this->current_event.handle_events(this);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    this->cam->recalculate_pv();

    for (object3d* ob : render_list) {
        ob->render(*this->cam, this);
    }
    
    glDepthMask(GL_FALSE);// TODO Make this per sprite based on wether the sprite is marked as translucent
    for (object2d* ob : render_list2d) {
        ob->render(*this->cam);
    }
    glDepthMask(GL_TRUE);// TODO Make this per sprite based on wether the sprite is marked as translucent

    // text (TODO: make sprites and text part of the same set so they can be layered. (probably via a variant))
    for (text* ob : render_list_text) {
        ob->render(*this->cam);
    }

    if (sky_box)
        sky_box->render(*this->cam);

    SDL_GL_SwapWindow(this->app_window);
    this->new_time = std::chrono::steady_clock::now();
    this->time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(this->starttime - this->old_time).count();
    this->time = std::chrono::duration_cast<std::chrono::seconds>(this->starttime - this->old_time).count();
    this->deltatime = std::chrono::duration_cast<std::chrono::nanoseconds>(this->new_time - this->old_time).count()/1000000000.0;// dt in seconds
    this->old_time = this->new_time;
} 

void window::add_object(object3d* obj) {
    this->render_list.insert(obj);
}

void window::remove_object(object3d* obj) {
    this->render_list.erase(obj);
}

void window::add_object_list(vector<object3d*> objs) {
    for (object3d * obj : objs) {
        this->render_list.insert(obj);
    }
}

void window::remove_object_list(vector<object3d*> objs) {
    for (object3d * obj : objs) {
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

// Point lights

void window::add_point_light(point_light* obj) {
    this->render_list_point_lights.insert(obj);
}

void window::remove_point_light(point_light* obj) {
    this->render_list_point_lights.erase(obj);
}

void window::add_point_light_list(vector<point_light*> objs) {
    for (point_light * obj : objs) {
        this->render_list_point_lights.insert(obj);
    }
}

void window::remove_point_light_list(vector<point_light*> objs) {
    for (point_light * obj : objs) {
        this->render_list_point_lights.erase(obj);
    }
}

// Directional lights

void window::add_directional_light(directional_light* obj) {
    this->render_list_directional_lights.insert(obj);
}

void window::remove_directional_light(directional_light* obj) {
    this->render_list_directional_lights.erase(obj);
}

void window::add_directional_light_list(vector<directional_light*> objs) {
    for (directional_light * obj : objs) {
        this->render_list_directional_lights.insert(obj);
    }
}

void window::remove_directional_light_list(vector<directional_light*> objs) {
    for (directional_light * obj : objs) {
        this->render_list_directional_lights.erase(obj);
    }
}

// Spot lights

void window::add_spot_light(spot_light* obj) {
    this->render_list_spot_lights.insert(obj);
}

void window::remove_spot_light(spot_light* obj) {
    this->render_list_spot_lights.erase(obj);
}

void window::add_spot_light_list(vector<spot_light*> objs) {
    for (spot_light * obj : objs) {
        this->render_list_spot_lights.insert(obj);
    }
}

void window::remove_spot_light_list(vector<spot_light*> objs) {
    for (spot_light * obj : objs) {
        this->render_list_spot_lights.erase(obj);
    }
}

// text

void window::add_text(text* obj) {
    this->render_list_text.insert(obj);
}

void window::remove_text(text* obj) {
    this->render_list_text.erase(obj);
}

void window::add_text_list(vector<text*> objs) {
    for (text * obj : objs) {
        this->render_list_text.insert(obj);
    }
}

void window::remove_text_list(vector<text*> objs) {
    for (text * obj : objs) {
        this->render_list_text.erase(obj);
    }
}