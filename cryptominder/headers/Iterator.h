#ifndef ITERATOR_H
#define ITERATOR_H

#include <vector>

template <typename T>
class Iterator {
public:
    explicit Iterator(typename std::vector<T>::iterator it) : it(it), is_const(false) {}

    explicit Iterator(typename std::vector<T>::const_iterator it) : it_const(it), is_const(true) {}

    T& operator*() { return is_const ? const_cast<T&>(*it_const) : *it; }
    const T& operator*() const { return *it_const; }

    T* operator->() { return is_const ? const_cast<T*>(&(*it_const)) : &(*it); }
    const T* operator->() const { return &(*it_const); }

    Iterator& operator++() {
        if (is_const) {
            ++it_const;
        } else {
            ++it;
        }
        return *this;
    }

    Iterator operator++(int) {
        Iterator temp = *this;
        if (is_const) {
            ++it_const;
        } else {
            ++it;
        }
        return temp;
    }

    bool operator==(const Iterator& other) const {
        return is_const ? (it_const == other.it_const) : (it == other.it);
    }

    bool operator!=(const Iterator& other) const {
        return is_const ? (it_const != other.it_const) : (it != other.it);
    }

private:
    typename std::vector<T>::iterator it;
    typename std::vector<T>::const_iterator it_const;
    bool is_const = false;
};

#endif // ITERATOR_H
