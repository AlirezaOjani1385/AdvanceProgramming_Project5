#include "Observer.hpp"

Observer::Observer(const std::string &name, const std::string &pass)
    :User(name, pass) {}

bool Observer::isObserver() const {
    return true;
}