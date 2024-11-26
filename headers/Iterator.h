#ifndef ITERATOR_H
#define ITERATOR_H
#include <cstddef>

template <typename T>
class UpdIterator {
private:
    T* current;

public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    explicit UpdIterator(pointer ptr) : current(ptr) {}

    reference operator*() const {
        return *current;
    }

    pointer operator->() {
        return current;
    }

    UpdIterator& operator++() {
        ++current;
        return *this;
    }

    UpdIterator operator++(int) {
        UpdIterator temp = *this;
        ++current;
        return temp;
    }

    bool operator==(const UpdIterator& other) const {
        return current == other.current;
    }

    bool operator!=(const UpdIterator& other) const {
        return current != other.current;
    }

    ptrdiff_t operator-(const UpdIterator& other) const {
        return current - other.current;
    }
};

#endif // ITERATOR_H
