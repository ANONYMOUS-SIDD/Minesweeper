#ifndef USER_PROFILE_H
#define USER_PROFILE_H

#include <string>

// Correct struct name to match usage
struct UserProfileData {
    std::string user_name;
    std::string email;
    std::string bestTime;
    std::string won;
    std::string lose;
};

class UserProfile {
public:
    // Constructor with email
    UserProfile(const std::string& email);

    // Fetch profile data using HTTP POST
    UserProfileData fetchProfile();

private:
    std::string email;

    // CURL write callback
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
};

#endif
