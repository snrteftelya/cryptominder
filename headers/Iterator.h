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

    bool operator==(const UpdIterator& other) const = default;

    bool operator!=(const UpdIterator& other) const {
        return !(*this == other);
    }

    friend ptrdiff_t operator-(const UpdIterator& lhs, const UpdIterator& rhs) {
        return lhs.current - rhs.current;
    }
};

#endif // ITERATOR_H
