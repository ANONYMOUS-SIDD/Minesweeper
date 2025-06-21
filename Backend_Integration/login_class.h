#ifndef LOGIN_CLASS_H
#define LOGIN_CLASS_H

#include <string>

class Login {
private:
    std::string email;
    std::string password;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

public:
    Login(const std::string& mail, const std::string& pass);

    std::string sendRequest();  // <-- Fixed here: added std::
};

#endif
