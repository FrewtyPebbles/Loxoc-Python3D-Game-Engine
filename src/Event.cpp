#include "Event.h"

const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

void event::handle_events(window* _window) {
    SDL_Event event;
    mouse_device* mouse;
    // reset nessicary events
    this->set_event_flag(EVENT_FLAG::MOUSE_BUTTON_UP, EVENT_STATE::NONE);
    this->set_event_flag(EVENT_FLAG::MOUSE_BUTTON_DOWN, EVENT_STATE::NONE);
    this->set_event_flag(EVENT_FLAG::MOUSE_MOTION, EVENT_STATE::NONE);
    this->set_event_flag(EVENT_FLAG::MOUSE_WHEEL, EVENT_STATE::NONE);

    while (SDL_PollEvent(&event)) {
        /* this is where we will record sdl events */
        switch (event.type) {
            case SDL_QUIT:
                this->set_event_flag(EVENT_FLAG::QUIT, EVENT_STATE::PRESSED);
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    this->set_event_flag(EVENT_FLAG::MOUSE_BUTTON_DOWN, EVENT_STATE::PRESSED);
                } else {
                    this->set_event_flag(EVENT_FLAG::MOUSE_BUTTON_UP, EVENT_STATE::RELEASED);
                }
                mouse = this->get_mouse_ptr(event.button.which + 1); // add + 1 to avoid -1 index
                this->current_mouse_id = mouse->id = event.button.which + 1;
            
                mouse->clicks = event.button.clicks;
                mouse->x = event.button.x;
                mouse->y = event.button.y;
                mouse->timestamp = event.button.timestamp;
                mouse->window = _window;
                // button
                switch (event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        mouse->button = MOUSE_BUTTON::LEFT;
                        break;

                    case SDL_BUTTON_RIGHT:
                        mouse->button = MOUSE_BUTTON::RIGHT;
                        break;

                    case SDL_BUTTON_MIDDLE:
                        mouse->button = MOUSE_BUTTON::MIDDLE;
                        break;
                }

                // state
                switch (event.button.state)
                {
                    case SDL_PRESSED:
                        mouse->state = EVENT_STATE::PRESSED;
                        break;

                    case SDL_RELEASED:
                        mouse->state = EVENT_STATE::RELEASED;
                        break;
                }

                // type
                switch (event.button.type)
                {
                    case SDL_MOUSEBUTTONDOWN:
                        mouse->type = MOUSE_EVENT_TYPE::BUTTON_DOWN;
                        break;

                    case SDL_MOUSEBUTTONUP:
                        mouse->type = MOUSE_EVENT_TYPE::BUTTON_UP;
                        break;
                }
                break;
            case SDL_MOUSEWHEEL:
                this->set_event_flag(EVENT_FLAG::MOUSE_WHEEL, EVENT_STATE::PRESSED);

                mouse = this->get_mouse_ptr(event.wheel.which + 1); // add + 1 to avoid -1 index
                this->current_mouse_id = mouse->id = event.wheel.which + 1;

                mouse->wheel.int_x = event.wheel.x;
                mouse->wheel.int_y = event.wheel.y;
                mouse->wheel.x = event.wheel.preciseX;
                mouse->wheel.y = event.wheel.preciseY;
                mouse->x = event.wheel.mouseX;
                mouse->y = event.wheel.mouseY;
                mouse->window = _window;
                
                // MWheel direction
                switch (event.wheel.direction)
                {
                    case SDL_MOUSEWHEEL_NORMAL:
                        mouse->wheel.direction = MOUSE_WHEEL_DIRECTION::NORMAL;
                        break;

                    case SDL_MOUSEWHEEL_FLIPPED:
                        mouse->wheel.direction = MOUSE_WHEEL_DIRECTION::FLIPPED;
                        break;
                }
                break;
            case SDL_MOUSEMOTION: // move mouse
                this->set_event_flag(EVENT_FLAG::MOUSE_MOTION, EVENT_STATE::PRESSED);
                
                mouse = this->get_mouse_ptr(event.motion.which + 1); // add + 1 to avoid -1 index
                this->current_mouse_id = mouse->id = event.motion.which + 1;

                mouse->x = event.motion.x;
                mouse->y = event.motion.y;
                mouse->rel_x = event.motion.xrel;
                mouse->rel_y = event.motion.yrel;
                mouse->timestamp = event.motion.timestamp;
                mouse->window = _window;
                
                break;

            case SDL_KEYUP:
            case SDL_KEYDOWN:
                kb_state(UP, UP);
                kb_state(DOWN, DOWN);
                kb_state(LEFT, LEFT);
                kb_state(RIGHT, RIGHT);
                kb_state(SPACE, SPACE);
                kb_state(LSHIFT, LSHIFT);
                kb_state(RSHIFT, RSHIFT);
                kb_state(LCTRL, LCTRL);
                kb_state(RCTRL, RCTRL);
                kb_state(LALT, LALT);
                kb_state(RALT, RALT);
                kb_state(a, A);
                kb_state(b, B);
                kb_state(c, C);
                kb_state(d, D);
                kb_state(e, E);
                kb_state(f, F);
                kb_state(g, G);
                kb_state(h, H);
                kb_state(i, I);
                kb_state(j, J);
                kb_state(k, K);
                kb_state(l, L);
                kb_state(m, M);
                kb_state(n, N);
                kb_state(o, O);
                kb_state(p, P);
                kb_state(q, Q);
                kb_state(r, R);
                kb_state(s, S);
                kb_state(t, T);
                kb_state(u, U);
                kb_state(v, V);
                kb_state(w, W);
                kb_state(x, X);
                kb_state(y, Y);
                kb_state(z, Z);
                kb_state(0, 0);
                kb_state(1, 1);
                kb_state(2, 2);
                kb_state(3, 3);
                kb_state(4, 4);
                kb_state(5, 5);
                kb_state(6, 6);
                kb_state(7, 7);
                kb_state(8, 8);
                kb_state(9, 9);
                kb_state(PERIOD, PERIOD);
                kb_state(COMMA, COMMA);
                kb_state(QUOTE, APOSTROPHE);
                kb_state(SEMICOLON, SEMICOLON);
                kb_state(OPEN_BRACKET, LEFTBRACKET);
                kb_state(CLOSE_BRACKET, RIGHTBRACKET);
                kb_state(BACKTICK, GRAVE);
                kb_state(DASH, MINUS);
                kb_state(EQUALS, EQUALS);
                kb_state(BACKSLASH, BACKSLASH);
                kb_state(FORWARDSLASH, SLASH);
                kb_state(ESCAPE, ESCAPE);
                
                break;

            case SDL_WINDOWEVENT:

                switch (event.window.event) {

                    case SDL_WINDOWEVENT_CLOSE:   // exit game
                        this->set_event_flag(EVENT_FLAG::WINDOW_CLOSE, EVENT_STATE::PRESSED);
                        break;
                }
                break;
        }
    }
}