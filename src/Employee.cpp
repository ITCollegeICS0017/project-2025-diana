#include "Employee.h"

// base
Employee::Employee(const std::string& name) : mName(name) {}
std::string Employee::name() const { return mName; }

// Clerk
Clerk::Clerk(const std::string& name) : Employee(name) {}
std::string Clerk::role() const { return "Clerk"; }

// Administrator
Administrator::Administrator(const std::string& name) : Employee(name) {}
std::string Administrator::role() const { return "Administrator"; }