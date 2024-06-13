#include "Window.h"
#include <functional>
#include "Camera.h"
#include <iostream>
#include <mutex>
#include <format>
#include "Object.h"

window::~window(){
    SDL_GL_DeleteContext(this->gl_context);
    SDL_DestroyWindow(this->app_window);
    SDL_Quit();
}

window::window() {
    this->title = "Default Window Title";
    this->create_window();
}

window::window(string title, camera* cam, int width, int height, bool fullscreen) : cam(cam), title(title), width(width), height(height), current_event(event::NOTHING), fullscreen(fullscreen) {
    this->create_window();
}

void window::create_window() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error(std::format("could not initialize sdl2: {}\n", SDL_GetError()));
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
        throw std::runtime_error(std::format("could not create window: {}\n", SDL_GetError()));
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
    std::cout << std::format("Vendor:   {}\n", (char *)glGetString(GL_VENDOR));
    std::cout << std::format("Renderer: {}\n", (char *)glGetString(GL_RENDERER));
    std::cout << std::format("Version:  {}\n", (char *)glGetString(GL_VERSION));


    glEnable(GL_DEPTH_TEST);
    
    glViewport(0, 0, this->width, this->height);
    
    this->old_time = SDL_GetTicks();
    
    return;
}


void window::update(vector<object*> objects) {
    this->new_time = SDL_GetTicks();
    this->deltatime = (this->new_time - this->old_time)/1000.0f;
    glViewport(0, 0, this->width, this->height);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        /* this is where we will record sdl events */
        switch (event.type) {
            case SDL_QUIT:
                this->current_event = event::QUIT;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        this->current_event = event::KEY_UP;
                        break;

                    case SDLK_DOWN:
                        this->current_event = event::KEY_DOWN;
                        break;

                    case SDLK_LEFT:
                        this->current_event = event::KEY_LEFT;
                        break;

                    case SDLK_RIGHT:
                        this->current_event = event::KEY_RIGHT;
                        break;

                    case SDLK_SPACE:
                        this->current_event = event::KEY_SPACE;
                        break;
                    default:
                        this->current_event = event::NOTHING;
                }
                break;

            case SDL_WINDOWEVENT:

                switch (event.window.event) {

                    case SDL_WINDOWEVENT_CLOSE:   // exit game
                        this->current_event = event::WINDOW_CLOSE;
                        break;

                    default:
                        this->current_event = event::NOTHING;
                }
                break;

            default:
                this->current_event = event::NOTHING;
        }
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    for (object* ob : objects) {
        ob->render(*this->cam);
    }

    SDL_GL_SwapWindow(this->app_window);
    //SDL_Delay(1);
    this->old_time = new_time;
}