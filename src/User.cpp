#include "User.hpp"

using namespace std;

User::User(const std::string& name, const std::string& pass) {
    username = name;
    password = pass;
}

bool User::checkPassword(const std::string &pass) const {
    return (password == pass);
}