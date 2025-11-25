#pragma once
#include <string>

class Employee {
public:
    explicit Employee(const std::string& name);
    virtual ~Employee() = default;
    virtual std::string role() const = 0;
    std::string name() const;
protected:
    std::string mName;
};

class Clerk : public Employee {
public:
    explicit Clerk(const std::string& name);
    std::string role() const override;
};

class Administrator : public Employee {
public:
    explicit Administrator(const std::string& name);
    std::string role() const override;
};
