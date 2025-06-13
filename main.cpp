#include "login_class.h"
#include "signup_class.h"
#include "score_class.h"
#include <string>
#include <iostream>

int main() {
    int choice;
//Do While Loop
    do {
        std::cout << "\n===== MENU =====\n";
        std::cout << "1. Login\n";
        std::cout << "2. Signup\n";
        std::cout << "3. Submit Score\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        // To consume the leftover newline after std::cin
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
           // Login User
            case 1: {
                std::string email, password;
                std::cout << "Enter Email: ";
                std::getline(std::cin, email);
                std::cout << "Enter Password: ";
                std::getline(std::cin, password);

                Login user(email, password);
                user.sendRequest();
                break;
            }
            // Signup User
            case 2: {
                std::string username, email, password;
                std::cout << "Enter Username: ";
                std::getline(std::cin, username);
                std::cout << "Enter Email: ";
                std::getline(std::cin, email);
                std::cout << "Enter Password: ";
                std::getline(std::cin, password);

                SignUp add(username, email, password);
                add.sendRequest();
                break;
            }
             // Submit Score
            case 3: {
                std::string email;
                int best_time, won, lose;

                std::cout << "Enter Email: ";
                std::getline(std::cin, email);
                std::cout << "Enter Best Time (int): ";
                std::cin >> best_time;
                std::cout << "Enter Number of Games Won (int): ";
                std::cin >> won;
                std::cout << "Enter Number of Games Lost (int): ";
                std::cin >> lose;

                // Consume leftover newline after std::cin
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                Score score(email, best_time, won, lose);
                score.sendRequest();
                break;
            }
            case 4:
                std::cout << "Exiting program. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 4);

    return 0;
}
