//Score Class Header File
#ifndef SCORE_CLASS_H
#define SCORE_CLASS_H

#include <string>

//Score Class Definition Starts Here
class Score {
private:
    //Private Variables To Store Player's Email And Game Statistics
    std::string email;
    int best_time; //Stores The Best Time Achieved
    int won;       //Stores The Total Number Of Games Won
    int lose;      //Stores The Total Number Of Games Lost

    //Static CallBack Function To Handle The Response From The Server
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

public:
    //Constructor To Initialize Email, Best Time, Won Count, And Lose Count
    Score(const std::string& mail, int bestTime, int wonCount, int loseCount);

    //Function To Send POST Request To The Server
    std::string sendRequest();
};

#endif // SCORE_CLASS_H
