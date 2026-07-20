#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP
#include <exception>

class BadRequestException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Bad Request";
    }
};

class PermissionDeniedException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Permission Denied";
    }
};

class NotFoundException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Not Found";
    }
};

class EmptyException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Empty";
    }
};

class LevelMismatchException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Level Mismatch";
    }
};

#endif