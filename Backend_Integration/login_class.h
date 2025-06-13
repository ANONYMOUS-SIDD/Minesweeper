//Login Class Header File
#ifndef LOGIN_CLASS_H
#define LOGIN_CLASS_H

#include <string>

//Login Class Definition Starts Here
class Login {
private:
    //Private Variables To Store Email And Password
    std::string email;
    std::string password;

    //Static CallBack Function To Handle The Response From The Server
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

public:
    //Constructor To Initialize Email And Password
    Login(const std::string& mail, const std::string& pass);

    //Function To Send POST Request To The Server
    void sendRequest();
};

#endif
