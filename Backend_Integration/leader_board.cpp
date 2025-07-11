#include "leader_board.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::vector<PlayerData> LeaderBoard::fetchTopPlayers(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string responseString;
    std::vector<PlayerData> players;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            try {
                json responseJson = json::parse(responseString);
                for (const auto& item : responseJson["players"]) {
                    PlayerData player;
                    player.email = item["email"].get<std::string>();
                    player.bestTime = item["best_time"].get<int>();
                    player.won = item["won"].get<int>();
                    player.lose = item["lose"].get<int>();
                    players.push_back(player);
                }
            } catch (const std::exception& e) {
                std::cerr << "JSON Parse Error: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "CURL failed: " << curl_easy_strerror(res) << std::endl;
        }
    }

    return players;
}
