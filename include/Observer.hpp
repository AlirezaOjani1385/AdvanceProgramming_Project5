#ifndef OBSERVER_HPP
#define OBSERVER_HPP
#include "User.hpp"

class Observer : public User {
public:
    Observer(const std::string& name, const std::string& pass);
    bool isObserver() const override;
};

#endif