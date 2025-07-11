#ifndef SCORE_CLASS_H
#define SCORE_CLASS_H

#include <string>

class Score {
private:
    std::string user_name;
    std::string email;
    int best_time;
    int won;
    int lose;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

public:
    // Constructor with email only (for sendRequest)
    Score(const std::string& mail, int bestTime, int wonCount, int loseCount);

    // Constructor with user_name (for initRequest)
    Score(const std::string& user_name, const std::string& mail, int bestTime, int wonCount, int loseCount);

    // Functions
    std::string sendRequest();
    std::string initRequest();
};

#endif // SCORE_CLASS_H
