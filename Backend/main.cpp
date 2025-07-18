// Header Files
#include <iostream>
#include <crow.h>
#include <pqxx/pqxx>
#include <sodium.h>   // Added for password hashing

// Main Function From Where The Execution Of Program Starts
int main()
{
  // Initialize libsodium (required for password hashing)
  if (sodium_init() < 0) {
      std::cerr << "Failed To Initialize Libsodium!" << std::endl;
      return 1;
  }

  // Initialize Crow WebServer
  crow::SimpleApp app;
  // Store All The Necessary Data In conn_query Variable Which Is Required To Establish The Connection Between The Server And PostgreSQL Database
  const std::string conn_query = "dbname=Minesweeper user=postgres password=siddhant123 host=localhost";
  // Establish The Connection Between Server And Database
  pqxx::connection conn(conn_query);
  // Check The Connection Of Server With Database
  if (!conn.is_open())
  {
    std::cerr << "Failed To Connect To Database !" << std::endl;
    return 1;
  }
  // Create Table If Not Exist
  {
    pqxx::work txn(conn);
    std::string table_query = R"(CREATE TABLE IF NOT EXISTS Users (
        id SERIAL NOT NULL,
            username TEXT NOT NULL,
            email TEXT PRIMARY KEY,
            password TEXT NOT NULL))";

    txn.exec(
        table_query);
    txn.commit();
        {
        pqxx::work txn(conn);
        txn.exec(R"(CREATE TABLE IF NOT EXISTS PlayerStats (
          user_name TEXT,
            email TEXT PRIMARY KEY REFERENCES Users(email),
            best_time INT,
            won INT,
            lose INT
        ))");
        txn.commit();
    }

  }
  // Register Route To Add User To The Database
  CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::POST)([&conn](const crow::request &req)
                                                               {
      //Store The JSON Body Into body Variable
      //load Function Converts JSON String Into JSON Object
        auto body = crow::json::load(req.body);
        //Check The Body Status
        if (!body) {
            return crow::response(400, "Failed To Parse JSON Data !");
        }
        //Store The User Data In Individual Variable
        std::string username = body["username"].s();
        std::string email = body["email"].s();
        std::string password = body["password"].s();

        // Hash the password before storing
        char hashed_password[crypto_pwhash_STRBYTES];
        if (crypto_pwhash_str(
            hashed_password,
            password.c_str(),
            password.length(),
            crypto_pwhash_OPSLIMIT_INTERACTIVE,
            crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
          return crow::response(500, "Password Hashing Failed !");
        }

        //Initialize transaction Object To Run SQL Query
        pqxx::work txn(conn);
        //Query To Fetch The Existing User
        std::string fetch_query = "SELECT 1 FROM Users WHERE email = $1 LIMIT 1";
          pqxx::result r = txn.exec_params(fetch_query, email);
        //Check The Status After Running The Query
        if (!r.empty()) {
            return crow::response(409, "Email Already In Use !");
        }
            std::string fetch_query_username = "SELECT 1 FROM Users WHERE username = $1 LIMIT 1";
        //Execute Fetch Query
                pqxx::result ruser = txn.exec_params(fetch_query_username, username);
                   if (!ruser.empty()) {
            return crow::response(409, "User Name Already In Use!");
        }


        //Query To Insert New User To Database (store hashed password)
        std::string insert_query = "INSERT INTO Users (username, email, password) VALUES ($1, $2, $3)";
        //Execute Insert Query
        txn.exec_params(insert_query, username, email, std::string(hashed_password));
        //Make Changes Visible
        txn.commit();
        //Response The User With Success Message
        return crow::response(201, "User Registered Successfully !"); });
  // Login Route To Identify The Existing User
  CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)([&conn](const crow::request &req)
                                                            {
      //Store The JSON Body Into body Variable
      //load Function Converts JSON String Into JSON Object
        auto body = crow::json::load(req.body);
        //Check If The Body Is Empty Or Not
        if (!body) {
            return crow::response(400, "Failed To Parse JSON Data !");
        }
        //Store User Login Data Into Individual Variable
        std::string email = body["email"].s();
        std::string password = body["password"].s();
        //Initialize Transaction Object To Run SQL Query
        pqxx::work txn(conn);
        //Fetch Query To Match Email With Password
        std::string fetch_query = "SELECT password FROM Users WHERE email = $1";
        //Execute Fetch Query
        pqxx::result r = txn.exec_params(fetch_query, email);
        //Check If Password And Email Match Or Not
        if (r.empty()) {
            return crow::response(404, "Invalid Email Or Password !");
        }
        //Store The User's Password Obtained From Database Into stored_password Variable
        //as<std::string>() Used For Type Casting
        std::string stored_password = r[0][0].as<std::string>();
        //Perform Password Matching (verify hash)
        if (crypto_pwhash_str_verify(stored_password.c_str(), password.c_str(), password.length()) != 0) {
            return crow::response(404, "Invalid Email Or Password !");
        }
        //Response Under Successful Login
        return crow::response(200, "Login Successful !"); });

        //Player Status Route
    //Route To Handle POST Request On /playersInfo Endpoint
CROW_ROUTE(app, "/playersInfo").methods(crow::HTTPMethod::POST)(
    [&conn](const crow::request& req) {
        // Parse the JSON body from the request
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Failed to parse JSON data");
        }
        // Extract fields





        std::string email = body["email"].s();
        int best_time = body.has("best_time") ? body["best_time"].i() : 0;
        int won = body.has("won") ? body["won"].i() : 0;
        int lose = body.has("lose") ? body["lose"].i() : 0;

        try {
            // Start transaction
            pqxx::work txn(conn);

            // Check if player already exists
            std::string check_query = "SELECT user_name, best_time, won, lose FROM PlayerStats WHERE email = $1";
            pqxx::result r = txn.exec_params(check_query, email);

            if (!r.empty()) {
                // Extract previous stats (correct indexes)
                int prev_best = r[0][1].is_null() ? 0 : r[0][1].as<int>();
                int prev_won = r[0][2].is_null() ? 0 : r[0][2].as<int>();
                int prev_lose = r[0][3].is_null() ? 0 : r[0][3].as<int>();

                // Update best time if better or no previous time
                int updated_best_time = (best_time != 0 && (best_time < prev_best || prev_best == 0))
                                          ? best_time
                                          : prev_best;

                // Update existing record
                std::string update_query = "UPDATE PlayerStats SET best_time = $2, won = $3, lose = $4 WHERE email = $1";
                txn.exec_params(update_query, email, updated_best_time, prev_won + won, prev_lose + lose);

                txn.commit();
                return crow::response(200, "Player stats updated successfully!");
            } else {
                // Insert new player record
                if(body.has("user_name")){
                   std::string user_name = body["user_name"].s();
                std::string insert_query = "INSERT INTO PlayerStats (user_name, email, best_time, won, lose) VALUES ($1, $2, $3, $4, $5)";
                txn.exec_params(insert_query, user_name, email, best_time, won, lose);

                txn.commit();
                return crow::response(201, "Player stats created successfully!");

                }
                   return crow::response(201, "Failed !");

            }
        } catch (const std::exception& e) {
            return crow::response(500, std::string("Database error: ") + e.what());
        }
    });

CROW_ROUTE(app, "/playersInfo").methods(crow::HTTPMethod::GET)([&conn]() {
    try {
        pqxx::work txn(conn);

        // Query top 5 players by best_time ascending (lowest best_time first)
pqxx::result r = txn.exec("SELECT * FROM PlayerStats WHERE best_time != 0 ORDER BY best_time ASC LIMIT 5");


        crow::json::wvalue::list player_list;

        for (const auto& row : r) {
            crow::json::wvalue player;
             player["user_name"] = row["user_name"].c_str();
            player["email"] = row["email"].c_str();
            player["best_time"] = row["best_time"].is_null() ? 0 : row["best_time"].as<int>();
            player["won"] = row["won"].is_null() ? 0 : row["won"].as<int>();
            player["lose"] = row["lose"].is_null() ? 0 : row["lose"].as<int>();

            player_list.emplace_back(std::move(player));
        }

        crow::json::wvalue result;
        result["players"] = std::move(player_list);

        return crow::response(200, result);
    } catch (const std::exception& e) {
        return crow::response(500, std::string("Server Error: ") + e.what());
    }
});

CROW_ROUTE(app, "/userProfile").methods(crow::HTTPMethod::POST)([&conn](const crow::request &req) {
    try {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Failed To Parse JSON Data !");
        }

        std::string email = body["email"].s();

        pqxx::work txn(conn);

        pqxx::result r = txn.exec_params(
            "SELECT user_name, email, best_time, won, lose FROM PlayerStats WHERE email = $1",
            email
        );

        if (r.empty()) {
            return crow::response(404, "User not found");
        }

        const auto& row = r[0];

        crow::json::wvalue player;

        try {
            player["user_name"] = row.at("user_name").c_str();
            player["email"] = row.at("email").c_str();
            player["best_time"] = row["best_time"].is_null() ? 0 : row["best_time"].as<int>();
            player["won"] = row["won"].is_null() ? 0 : row["won"].as<int>();
            player["lose"] = row["lose"].is_null() ? 0 : row["lose"].as<int>();
        } catch (const std::exception& inner) {
            return crow::response(500, std::string("JSON build error: ") + inner.what());
        }

        return crow::response(200, player);

    } catch (const std::exception &e) {
        return crow::response(500, std::string("Server Error: ") + e.what());
    }
});













  // Open Port Number 18080 To Establish The Connection Between Device And Server
  // Multithread Id Enabled To Handle Multiple Request At A Single Time
  app.port(18080).multithreaded().run();
  // Successful Execution Of Code
  return 0;
}
