#include "Event.h"

void event::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        /* this is where we will record sdl events */
        switch (event.type) {
            case SDL_QUIT:
                this->set_event_flag(EVENT_FLAG::QUIT, EVENT_STATE::PRESSED);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    kb_sdl_evt(UP, PRESSED);
                    kb_sdl_evt(DOWN, PRESSED);
                    kb_sdl_evt(LEFT, PRESSED);
                    kb_sdl_evt(RIGHT, PRESSED);
                    kb_sdl_evt(SPACE, PRESSED);
                    kb_sdl_evt(LSHIFT, PRESSED);
                    kb_sdl_evt(RSHIFT, PRESSED);
                    kb_sdl_evt(LCTRL, PRESSED);
                    kb_sdl_evt(RCTRL, PRESSED);
                    kb_sdl_evt(LALT, PRESSED);
                    kb_sdl_evt(RALT, PRESSED);
                    kb_sdl_evt(a, PRESSED);
                    kb_sdl_evt(b, PRESSED);
                    kb_sdl_evt(c, PRESSED);
                    kb_sdl_evt(d, PRESSED);
                    kb_sdl_evt(e, PRESSED);
                    kb_sdl_evt(f, PRESSED);
                    kb_sdl_evt(g, PRESSED);
                    kb_sdl_evt(h, PRESSED);
                    kb_sdl_evt(i, PRESSED);
                    kb_sdl_evt(j, PRESSED);
                    kb_sdl_evt(k, PRESSED);
                    kb_sdl_evt(l, PRESSED);
                    kb_sdl_evt(m, PRESSED);
                    kb_sdl_evt(n, PRESSED);
                    kb_sdl_evt(o, PRESSED);
                    kb_sdl_evt(p, PRESSED);
                    kb_sdl_evt(q, PRESSED);
                    kb_sdl_evt(r, PRESSED);
                    kb_sdl_evt(s, PRESSED);
                    kb_sdl_evt(t, PRESSED);
                    kb_sdl_evt(u, PRESSED);
                    kb_sdl_evt(v, PRESSED);
                    kb_sdl_evt(w, PRESSED);
                    kb_sdl_evt(x, PRESSED);
                    kb_sdl_evt(y, PRESSED);
                    kb_sdl_evt(z, PRESSED);
                    kb_sdl_evt(0, PRESSED);
                    kb_sdl_evt(1, PRESSED);
                    kb_sdl_evt(2, PRESSED);
                    kb_sdl_evt(3, PRESSED);
                    kb_sdl_evt(4, PRESSED);
                    kb_sdl_evt(5, PRESSED);
                    kb_sdl_evt(6, PRESSED);
                    kb_sdl_evt(7, PRESSED);
                    kb_sdl_evt(8, PRESSED);
                    kb_sdl_evt(9, PRESSED);
                    kb_sdl_evt(PERIOD, PRESSED);
                    kb_sdl_evt(COMMA, PRESSED);
                    kb_sdl_evt(QUOTE, PRESSED);
                    kb_sdl_evt(SEMICOLON, PRESSED);
                    kb_sdl_evt2(LEFTBRACKET, OPEN_BRACKET, PRESSED);
                    kb_sdl_evt2(RIGHTBRACKET, CLOSE_BRACKET, PRESSED);
                    kb_sdl_evt2(BACKQUOTE, BACKTICK, PRESSED);
                    kb_sdl_evt2(MINUS, DASH, PRESSED);
                    kb_sdl_evt(EQUALS, PRESSED);
                    kb_sdl_evt(BACKSLASH, PRESSED);
                    kb_sdl_evt2(SLASH, FORWARDSLASH, PRESSED);
                    kb_sdl_evt(ESCAPE, PRESSED);
                }
                break;
            
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    kb_sdl_evt(UP, RELEASED);
                    kb_sdl_evt(DOWN, RELEASED);
                    kb_sdl_evt(LEFT, RELEASED);
                    kb_sdl_evt(RIGHT, RELEASED);
                    kb_sdl_evt(SPACE, RELEASED);
                    kb_sdl_evt(LSHIFT, RELEASED);
                    kb_sdl_evt(RSHIFT, RELEASED);
                    kb_sdl_evt(LCTRL, RELEASED);
                    kb_sdl_evt(RCTRL, RELEASED);
                    kb_sdl_evt(LALT, RELEASED);
                    kb_sdl_evt(RALT, RELEASED);
                    kb_sdl_evt(a, RELEASED);
                    kb_sdl_evt(b, RELEASED);
                    kb_sdl_evt(c, RELEASED);
                    kb_sdl_evt(d, RELEASED);
                    kb_sdl_evt(e, RELEASED);
                    kb_sdl_evt(f, RELEASED);
                    kb_sdl_evt(g, RELEASED);
                    kb_sdl_evt(h, RELEASED);
                    kb_sdl_evt(i, RELEASED);
                    kb_sdl_evt(j, RELEASED);
                    kb_sdl_evt(k, RELEASED);
                    kb_sdl_evt(l, RELEASED);
                    kb_sdl_evt(m, RELEASED);
                    kb_sdl_evt(n, RELEASED);
                    kb_sdl_evt(o, RELEASED);
                    kb_sdl_evt(p, RELEASED);
                    kb_sdl_evt(q, RELEASED);
                    kb_sdl_evt(r, RELEASED);
                    kb_sdl_evt(s, RELEASED);
                    kb_sdl_evt(t, RELEASED);
                    kb_sdl_evt(u, RELEASED);
                    kb_sdl_evt(v, RELEASED);
                    kb_sdl_evt(w, RELEASED);
                    kb_sdl_evt(x, RELEASED);
                    kb_sdl_evt(y, RELEASED);
                    kb_sdl_evt(z, RELEASED);
                    kb_sdl_evt(0, RELEASED);
                    kb_sdl_evt(1, RELEASED);
                    kb_sdl_evt(2, RELEASED);
                    kb_sdl_evt(3, RELEASED);
                    kb_sdl_evt(4, RELEASED);
                    kb_sdl_evt(5, RELEASED);
                    kb_sdl_evt(6, RELEASED);
                    kb_sdl_evt(7, RELEASED);
                    kb_sdl_evt(8, RELEASED);
                    kb_sdl_evt(9, RELEASED);
                    kb_sdl_evt(PERIOD, RELEASED);
                    kb_sdl_evt(COMMA, RELEASED);
                    kb_sdl_evt(QUOTE, RELEASED);
                    kb_sdl_evt(SEMICOLON, RELEASED);
                    kb_sdl_evt2(LEFTBRACKET, OPEN_BRACKET, RELEASED);
                    kb_sdl_evt2(RIGHTBRACKET, CLOSE_BRACKET, RELEASED);
                    kb_sdl_evt2(BACKQUOTE, BACKTICK, RELEASED);
                    kb_sdl_evt2(MINUS, DASH, RELEASED);
                    kb_sdl_evt(EQUALS, RELEASED);
                    kb_sdl_evt(BACKSLASH, RELEASED);
                    kb_sdl_evt2(SLASH, FORWARDSLASH, RELEASED);
                    kb_sdl_evt(ESCAPE, RELEASED);
                }
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