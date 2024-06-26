#pragma once
#include <iostream>

template<typename T>
class RC {
public:
    RC(): refcount(1){}
    RC(T data): data(data), refcount(1){}
    inline T inc(){
        this->refcount++;
        return data;
    }
    inline void dec(){
        this->refcount--;
        if (this->refcount == 0) {
            delete this->data;
        }
    }
    T data;
    int refcount;
};

template<typename T>
constexpr void RC_collect(RC<T*>* rc) {
    rc->dec();
    if (rc->refcount == 0)
        delete rc;
}
    