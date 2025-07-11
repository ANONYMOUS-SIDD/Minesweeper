#ifndef LEADER_BOARD_H
#define LEADER_BOARD_H

#include <string>
#include <vector>

struct PlayerData {
  std::string user_name;
    std::string email;
    int bestTime;
    int won;
    int lose;
};

class LeaderBoard {
public:
    static std::vector<PlayerData> fetchTopPlayers(const std::string& url="http://localhost:18080/playersInfo");
};

#endif
