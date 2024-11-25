#ifndef UPD_VECTOR_H
#define UPD_VECTOR_H

#include <vector>
#include <memory>

template <typename T>
class UpdVector {
private:
    std::vector<std::unique_ptr<T>> data;

public:
    void add(std::unique_ptr<T> element) {
        data.push_back(std::move(element));
    }

    void remove(typename std::vector<std::unique_ptr<T>>::iterator it) {
        data.erase(it);
    }

    typename std::vector<std::unique_ptr<T>>::iterator begin() {
        return data.begin();
    }

    typename std::vector<std::unique_ptr<T>>::iterator end() {
        return data.end();
    }

    typename std::vector<std::unique_ptr<T>>::const_iterator begin() const {
        return data.begin();
    }

    typename std::vector<std::unique_ptr<T>>::const_iterator end() const {
        return data.end();
    }

    void clear() {
        data.clear();
    }

    size_t size() const {
        return data.size();
    }

    T& operator[](size_t index) {
        return *data[index];
    }

    const T& operator[](size_t index) const {
        return *data[index];
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

#endif // UPD_VECTOR_H
