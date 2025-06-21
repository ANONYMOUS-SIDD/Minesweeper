#include "score_class.h"
#include <iostream>
#include <curl/curl.h>

//CallBack Function So To Handle The Response After Hitting A Request
size_t Score::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    //Return Total Number Of Bytes Returned As Response
    size_t totalSize = size * nmemb;
    //Append The Response To Form Complete Data
    userp->append((char*)contents, totalSize);
    //Return Total Size Received As Response
    return totalSize;
}

//Score Constructor Called To Initialize The Values
Score::Score(const std::string& mail, int bestTime, int wonGames, int lostGames)
    : email(mail), best_time(bestTime), won(wonGames), lose(lostGames) {}

//sendRequest Function To Be Triggered On Button Click
std::string Score::sendRequest() {
    //Initialize Curl
    CURL* curl = curl_easy_init();

    if (curl) {
        //Initialize responseString To Store Response Message
        std::string responseString;

        //Create JSON String To Be Passed As JSON Body
        std::string jsonData = R"({"email":")" + email +
                               R"(", "best_time":)" + std::to_string(best_time) +
                               R"(, "won":)" + std::to_string(won) +
                               R"(, "lose":)" + std::to_string(lose) + "}";

        //Create Header To Indicate What Type Of Data The POST Request Is Going To Send
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        //Initialize Target URL
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:18080/playersInfo");
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

        //Erase Headers
        curl_slist_free_all(headers);
        //Free The Memory
        curl_easy_cleanup(curl);

        //Check If Connection Is Made Valid Or Not
        if (res == CURLE_OK) {
            return responseString;
        } else {
            return std::string("Request failed: ") + curl_easy_strerror(res);
        }
    }

    return "Failed to initialize CURL";
}
