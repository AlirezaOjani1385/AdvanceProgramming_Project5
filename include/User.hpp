#ifndef USER_HPP
#define USER_HPP
#include <string>

class User {
public:
    User(const std::string& name, const std::string& pass);
    std::string getUserName() const { return username; };
    bool checkPassword(const std::string& pass) const;
    virtual bool isObserver() const = 0;
protected:
    std::string username;
    std::string password;
};

#endif