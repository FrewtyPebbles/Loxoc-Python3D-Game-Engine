#pragma once
#include <iostream>

template<typename T>
class RC {
public:
    RC(): refcount(1){}
    RC(T data): data(data), refcount(1){}
    // ~RC() {
    //     delete this->data;
    // }
    inline T inc(){
        this->refcount++;
        return data;
    }
    inline void dec(){
        this->refcount--;
        if (this->refcount == 0) {
            delete this->data;
        } else if (this->refcount < 0) {
            std::cout << "There has been a fatal reference counting bug, please open an issue on the github.  REFCOUNT " << this->refcount << "\n";
        }
    }
    T data;
    int refcount;
};

template<typename T>
constexpr void RC_collect(RC<T*>* rc) {
    rc->dec();
    if (rc->refcount == 0) {
        delete rc;
    }
}
    