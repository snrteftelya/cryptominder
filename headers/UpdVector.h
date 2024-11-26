#ifndef UPDVECTOR_H
#define UPDVECTOR_H

#include <vector>
#include "Iterator.h"

template <typename T>
class UpdVector {
private:
    std::vector<T> data;

public:
    using iterator = UpdIterator<T>;
    using const_iterator = UpdIterator<const T>;

    void add(T item) {
        data.push_back(std::move(item));
    }

    void remove(iterator it) {
        data.erase(data.begin() + (it - begin()));
    }


    iterator begin() {
        return iterator(data.data());
    }

    const_iterator begin() const {
        return const_iterator(data.data());
    }

    iterator end() {
        return iterator(data.data() + data.size());
    }

    const_iterator end() const {
        return const_iterator(data.data() + data.size());
    }

    void clear() {
        data.clear();
    }

    size_t size() const {
        return data.size();
    }

    T& at(size_t index) {
        return data.at(index);
    }

    const T& at(size_t index) const {
        return data.at(index);
    }
};

template <typename Container, typename Func>
void for_each(Container &container, Func func) {
    for (auto &element : container) {
        func(element);
    }
}

#endif // UPDVECTOR_H
