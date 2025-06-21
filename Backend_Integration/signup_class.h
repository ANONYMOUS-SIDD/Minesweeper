//SignUp Class Header File
#ifndef SIGNUP_CLASS_H
#define SIGNUP_CLASS_H

#include <string>

//SignUp Class Definition Starts Here
class SignUp {
private:
    //Private Variables To Store Username, Email, And Password
    std::string username;
    std::string email;
    std::string password;

    //Static CallBack Function To Handle The Response From The Server
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

public:
    //Constructor To Initialize Username, Email, And Password
    SignUp(const std::string& uname, const std::string& mail, const std::string& pword);

    //Function To Send POST Request To The Server
    std::string sendRequest();  // <-- Fixed here: added std::
};

#endif
