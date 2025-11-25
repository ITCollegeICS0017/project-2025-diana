
#pragma once
#include <vector>

template<typename T>
class IRepository {
public:
    virtual void add(const T&) = 0;
    virtual std::vector<T> listAll() const = 0;
    virtual void save() = 0;
    virtual void load() = 0;
    virtual ~IRepository() = default;
};
