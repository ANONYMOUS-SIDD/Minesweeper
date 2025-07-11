#include "score_class.h"
#include <iostream>
#include <curl/curl.h>

// Callback to handle HTTP response
size_t Score::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

// Constructor without user_name
Score::Score(const std::string& mail, int bestTime, int wonGames, int lostGames)
    : email(mail), best_time(bestTime), won(wonGames), lose(lostGames) {}

// Constructor with user_name
Score::Score(const std::string& user_name, const std::string& mail, int bestTime, int wonGames, int lostGames)
    : user_name(user_name), email(mail), best_time(bestTime), won(wonGames), lose(lostGames) {}

std::string Score::sendRequest() {
    CURL* curl = curl_easy_init();
    if (curl) {
        std::string responseString;

        std::string jsonData = R"({"email":")" + email +
                               R"(", "best_time":)" + std::to_string(best_time) +
                               R"(, "won":)" + std::to_string(won) +
                               R"(, "lose":)" + std::to_string(lose) + "}";

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:18080/playersInfo");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

        CURLcode res = curl_easy_perform(curl);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            return responseString;
        } else {
            return std::string("Request failed: ") + curl_easy_strerror(res);
        }
    }

    return "Failed to initialize CURL";
}

std::string Score::initRequest() {
    CURL* curl = curl_easy_init();
    if (curl) {
        std::string responseString;

        std::string jsonData = R"({"user_name":")" + user_name +
                               R"(", "email":")" + email +
                               R"(", "best_time":)" + std::to_string(best_time) +
                               R"(, "won":)" + std::to_string(won) +
                               R"(, "lose":)" + std::to_string(lose) + "}";

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:18080/playersInfo");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

        CURLcode res = curl_easy_perform(curl);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            return responseString;
        } else {
            return std::string("Request failed: ") + curl_easy_strerror(res);
        }
    }

    return "Failed to initialize CURL";
}
