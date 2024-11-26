#ifndef UPDVECTOR_H
#define UPDVECTOR_H
#include <vector>
#include <algorithm>
#include <stdexcept>

template <typename T>
class UpdVector {
private:
    std::vector<T> data;

public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    void add(T item) {
        data.push_back(std::move(item));
    }

    void remove(iterator it) {
        data.erase(it);
    }

    iterator begin() {
        return data.begin();
    }

    const_iterator begin() const {
        return data.begin();
    }

    iterator end() {
        return data.end();
    }

    const_iterator end() const {
        return data.end();
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

template <typename T>
bool contains(const UpdVector<T>& container, const T& value) {
    std::ranges::for_each(container, [&](const auto& elem) {
        if (elem == value) {
            return true;
        }
    });
    return false;
}

#endif // UPDVECTOR_H
