#pragma once

/**
 * A pointer offering no ownership over something.
 * But it is guarantee the pointer will be valid, as the thinkg using the ptr will be destroyed anyway.
 */
template<typename T>
class guaranteed_ptr {
private:
    T* _pointer = nullptr;
public:
    guaranteed_ptr(T* pointer) : _pointer(pointer) {}
    guaranteed_ptr() : _pointer(nullptr) {}
    guaranteed_ptr(const guaranteed_ptr<T>& other) : _pointer(other._pointer) {}
    guaranteed_ptr(guaranteed_ptr<T>&& other): _pointer(other._pointer) { other._ptr = nullptr; }

    bool operator()() const {
        return _pointer != nullptr;
    }
    guaranteed_ptr<T>& operator=(T* new_pointer) {
        _pointer = new_pointer;
        return *this;
    }
    guaranteed_ptr<T>& operator=(const guaranteed_ptr<T> other) {
        _pointer = other._pointer;
        return *this;
    }
    T* operator->() {
        return _pointer;
    }
    const T* operator->() const {
        return _pointer;
    }
};
