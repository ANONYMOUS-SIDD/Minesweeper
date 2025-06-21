//SignUp Class Source File
#include "signup_class.h"
#include <iostream>
#include <curl/curl.h>
#include <string>

//CallBack Function To Handle Server Response After Sending The Request
size_t SignUp::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    //Calculate Total Number Of Bytes Received In Response
    size_t totalSize = size * nmemb;
    //Append Response Data To The Response String
    userp->append((char*)contents, totalSize);
    //Return The Total Number Of Bytes Received
    return totalSize;
}

//Constructor To Initialize Username, Email, And Password
SignUp::SignUp(const std::string& uname, const std::string& mail, const std::string& pword)
    : username(uname), email(mail), password(pword) {}

//Function To Send SignUp Request To The Server
std::string SignUp::sendRequest() {
    //Initialize CURL
    CURL* curl = curl_easy_init();

    if (curl) {
        //Variable To Store Response From The Server
        std::string responseString;

        //Create JSON String To Be Sent As Request Body
        std::string jsonData = R"({"username":")" + username + R"(", "email":")" + email + R"(", "password":")" + password + R"("})";

        //Create Header To Specify JSON Content Type
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        //Set Target URL For SignUp Endpoint
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:18080/register");
        //Attach Headers
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        //Attach JSON Request Body
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        //Set CallBack Function To Handle Response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        //Set Variable To Store Response Data
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

        //Execute The Request
        CURLcode res = curl_easy_perform(curl);

        //Free The Header Memory
        curl_slist_free_all(headers);
        //Clean Up CURL Resources
        curl_easy_cleanup(curl);

        //Check If Request Was Successful
        if (res == CURLE_OK) {
           // std::cout << "Response: " << responseString << std::endl;
           return responseString;
        } else {
           // std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
           return std::string("curl_easy_perform() failed: ") + curl_easy_strerror(res);
        }
    }
    // Return this if curl initialization failed
    return "Failed to initialize CURL";
}
