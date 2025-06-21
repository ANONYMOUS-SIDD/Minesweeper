// Login Class Header File
#include "login_class.h"
#include <curl/curl.h>
#include <iostream>
#include <string>

//Login Constructor Called To Initialize The Value
Login::Login(const std::string& mail, const std::string& pass)
    : email(mail), password(pass) {}

//CallBack Function So To Handle The Response After Hitting A Request
size_t Login::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
  //Return Total Number Of Bytes Returned As Response
  //size Stores The Byte Consumed By Each Component Returend
  //nmemb Stores The Total Number Of Components Returned
    size_t totalSize = size * nmemb;
    //The Data Is Returned Multiple Times So The Data Is Appended To Form Complete contents
    userp->append((char*)contents, totalSize);
    //Return Total Size Received As Response
    return totalSize;
}

//sendRequest Function To Be Triggered On Button Click
std::string Login::sendRequest() {
  //Initialize Curl
    CURL* curl = curl_easy_init();
    if (curl) {
      //Initialize responseString To Store Response Message
        std::string responseString;
        //Create JSON String To Be Passes As JSON Body
        std::string jsonData = "{\"email\":\"" + email + "\", \"password\":\"" + password + "\"}";
        //Create Header To Indicate What Type Of Data The POST Request Is Going To Send
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        //Initialize Target URL
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:18080/login");
        //Include Header
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        //Include JSON Body
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        //Include CallBack Function For Response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        //Store The Response In responseString
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
        //Finally Execute The Query
        CURLcode res = curl_easy_perform(curl);

        //Erase Headers and Free The Memory BEFORE returning
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        //Check If Connection Is Made Valid Or Not
        if (res == CURLE_OK) {
            //std::cout << "Response: " << responseString << std::endl;
            return responseString;
        } else {
            //std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return std::string("curl_easy_perform() failed: ") + curl_easy_strerror(res);
        }
    }
    // Return this if curl initialization failed
    return "Failed to initialize CURL";
}
