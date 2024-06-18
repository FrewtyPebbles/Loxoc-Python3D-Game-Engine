#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <stdexcept>
#include <map>
#include <format>
#include <iostream>

#define TOTAL_EVENT_FLAGS 100


class window;

typedef size_t EVENT_FLAG_T;

enum class EVENT_FLAG {

    // WINDOW MANAGEMENT
    WINDOW_MINIMIZE = (EVENT_FLAG_T)0,
    WINDOW_CLOSE = (EVENT_FLAG_T)1,
    QUIT = (EVENT_FLAG_T)2,

    // KEYS
    KEY_UP = (EVENT_FLAG_T)3,
    KEY_DOWN = (EVENT_FLAG_T)4,
    KEY_RIGHT = (EVENT_FLAG_T)5,
    KEY_LEFT = (EVENT_FLAG_T)6,
    KEY_SPACE = (EVENT_FLAG_T)7,
    KEY_a = (EVENT_FLAG_T)8,
    KEY_b = (EVENT_FLAG_T)9,
    KEY_c = (EVENT_FLAG_T)10,
    KEY_d = (EVENT_FLAG_T)11,
    KEY_e = (EVENT_FLAG_T)12,
    KEY_f = (EVENT_FLAG_T)13,
    KEY_g = (EVENT_FLAG_T)14,
    KEY_h = (EVENT_FLAG_T)15,
    KEY_i = (EVENT_FLAG_T)16,
    KEY_j = (EVENT_FLAG_T)17,
    KEY_k = (EVENT_FLAG_T)18,
    KEY_l = (EVENT_FLAG_T)19,
    KEY_m = (EVENT_FLAG_T)20,
    KEY_n = (EVENT_FLAG_T)21,
    KEY_o = (EVENT_FLAG_T)22,
    KEY_p = (EVENT_FLAG_T)23,
    KEY_q = (EVENT_FLAG_T)24,
    KEY_r = (EVENT_FLAG_T)25,
    KEY_s = (EVENT_FLAG_T)26,
    KEY_t = (EVENT_FLAG_T)27,
    KEY_u = (EVENT_FLAG_T)28,
    KEY_v = (EVENT_FLAG_T)29,
    KEY_w = (EVENT_FLAG_T)30,
    KEY_x = (EVENT_FLAG_T)31,
    KEY_y = (EVENT_FLAG_T)32,
    KEY_z = (EVENT_FLAG_T)33,
    KEY_LSHIFT = (EVENT_FLAG_T)34,
    KEY_RSHIFT = (EVENT_FLAG_T)35,
    KEY_LCTRL = (EVENT_FLAG_T)36,
    KEY_LALT = (EVENT_FLAG_T)37,
    KEY_ESCAPE = (EVENT_FLAG_T)38,
    KEY_COMMA = (EVENT_FLAG_T)39,
    KEY_PERIOD = (EVENT_FLAG_T)40,
    KEY_FORWARDSLASH = (EVENT_FLAG_T)41,
    KEY_BACKSLASH = (EVENT_FLAG_T)42,
    KEY_OPEN_BRACKET = (EVENT_FLAG_T)43,
    KEY_CLOSE_BRACKET = (EVENT_FLAG_T)44,
    KEY_SEMICOLON = (EVENT_FLAG_T)45,
    KEY_QUOTE = (EVENT_FLAG_T)46,
    KEY_ENTER = (EVENT_FLAG_T)47,
    KEY_BACKSPACE = (EVENT_FLAG_T)48,
    KEY_TAB = (EVENT_FLAG_T)49,
    KEY_BACKTICK = (EVENT_FLAG_T)50,
    KEY_DASH = (EVENT_FLAG_T)51,
    KEY_EQUALS = (EVENT_FLAG_T)52,
    KEY_1 = (EVENT_FLAG_T)53,
    KEY_2 = (EVENT_FLAG_T)54,
    KEY_3 = (EVENT_FLAG_T)55,
    KEY_4 = (EVENT_FLAG_T)56,
    KEY_5 = (EVENT_FLAG_T)57,
    KEY_6 = (EVENT_FLAG_T)58,
    KEY_7 = (EVENT_FLAG_T)59,
    KEY_8 = (EVENT_FLAG_T)60,
    KEY_9 = (EVENT_FLAG_T)61,
    KEY_0 = (EVENT_FLAG_T)62,
    KEY_RCTRL = (EVENT_FLAG_T)63,
    KEY_RALT = (EVENT_FLAG_T)64,

    // Mouse
    MOUSE_BUTTON_DOWN = (EVENT_FLAG_T)65,
    MOUSE_BUTTON_UP = (EVENT_FLAG_T)66,
    MOUSE_WHEEL = (EVENT_FLAG_T)67,
    MOUSE_MOTION = (EVENT_FLAG_T)68
};

enum class EVENT_STATE {
    NONE = 0,
    PRESSED = 1,
    RELEASED = 2
};

enum class MOUSE_EVENT_TYPE {
    BUTTON_DOWN = 0,
    BUTTON_UP = 1,
    NONE = 2
};

enum class MOUSE_BUTTON {
    LEFT = 0,
    RIGHT = 1,
    MIDDLE = 2,
    NONE = 3
};

enum class MOUSE_WHEEL_DIRECTION {
    FLIPPED = 0,
    NORMAL = 1,
    NONE = 2
};

struct mouse_wheel {
    mouse_wheel(){};
    int32_t int_x, int_y = 0;
    float x, y = 0.0f;
    MOUSE_WHEEL_DIRECTION direction = MOUSE_WHEEL_DIRECTION::NONE;
};

struct mouse_device {
    mouse_device(){};
    uint32_t id = 0; // this is like the which parameter in sdl mouse events + 1
    uint32_t timestamp = 0;
    int32_t x, y, rel_x, rel_y = 0;
    uint8_t clicks = 0;
    MOUSE_EVENT_TYPE type = MOUSE_EVENT_TYPE::NONE; 
    window* window = nullptr;
    EVENT_STATE state = EVENT_STATE::NONE;
    MOUSE_BUTTON button = MOUSE_BUTTON::NONE;
    mouse_wheel wheel = mouse_wheel();
};

class event {
public:
    event(){reset_event();}
    ~event(){}
    inline void reset_event() {
        memset(flags, (int)EVENT_STATE::NONE, sizeof(flags));
    }
    inline void set_event_flag(EVENT_FLAG _event, EVENT_STATE state) {
        flags[static_cast<EVENT_FLAG_T>(_event)] = state;
    }
    inline void remove_event_flag(EVENT_FLAG _event) {
        flags[static_cast<EVENT_FLAG_T>(_event)] = EVENT_STATE::NONE;
    }
    inline EVENT_STATE get_flag(EVENT_FLAG _event) {
        return flags[static_cast<EVENT_FLAG_T>(_event)];
    };
    inline bool check_flag(EVENT_FLAG _event) {
        return flags[static_cast<EVENT_FLAG_T>(_event)] != EVENT_STATE::NONE;
    }
    void handle_events(window* _window);
    inline mouse_device get_mouse() {
        if (!this->mice.contains(this->current_mouse_id))
            this->mice[this->current_mouse_id] = mouse_device();
        return this->mice[this->current_mouse_id];
    }
    inline mouse_device get_mouse(unsigned char id) {
        if (!this->mice.contains(id))
            this->mice[id] = mouse_device();
        return this->mice[id];
    }
    inline mouse_device* get_mouse_ptr(unsigned char id) {
        if (!this->mice.contains(id))
            this->mice[id] = mouse_device();
        return &this->mice[id];
    }
    std::map<size_t, mouse_device> mice; // ordered by device number
    unsigned char current_mouse_id;
private:    
    EVENT_STATE flags[TOTAL_EVENT_FLAGS];
};

//MACROS

#define _CONCAT(a, b) a##b
#define CONCAT(a, b) _CONCAT(a, b)

#define kb_sdl_evt(prt, state) \
    case CONCAT(SDLK_, prt): \
        this->set_event_flag(EVENT_FLAG::CONCAT(KEY_, prt), EVENT_STATE::state); \
        break;

#define kb_sdl_evt2(prt1, prt2, state) \
    case CONCAT(SDLK_, prt1): \
        this->set_event_flag(EVENT_FLAG::CONCAT(KEY_, prt2), EVENT_STATE::state); \
        break;