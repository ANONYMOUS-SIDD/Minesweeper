#include "user_profile.h"
#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

UserProfile::UserProfile(const std::string& email)
    : email(email) {}

size_t UserProfile::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

UserProfileData UserProfile::fetchProfile() {
    UserProfileData userData;  // empty struct to return

    CURL* curl = curl_easy_init();
    if (curl) {
        std::string responseString;
        std::string jsonData = "{\"email\":\"" + email + "\"}";

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:18080/userProfile");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

        CURLcode res = curl_easy_perform(curl);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            try {
                json j = json::parse(responseString);

                userData.user_name = j.value("user_name", "");
                userData.email = j.value("email", "");

                // Convert int fields to string
                userData.bestTime = std::to_string(j.value("best_time", 0));
                userData.won = std::to_string(j.value("won", 0));
                userData.lose = std::to_string(j.value("lose", 0));

            } catch (const std::exception& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
        }
    } else {
        std::cerr << "Failed to initialize CURL" << std::endl;
    }

    return userData;
}
