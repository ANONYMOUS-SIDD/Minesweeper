// Header Files
#include <iostream>
#include <crow.h>
#include <pqxx/pqxx>
// Main Function From Where The Execution Of Program Starts
int main()
{
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
        //Initialize transaction Object To Run SQL Query
        pqxx::work txn(conn);
        //Query To Fetch The Existing User
        std::string fetch_query = "SELECT 1 FROM Users WHERE email = $1 LIMIT 1";
        //Execute Fetch Query
        pqxx::result r = txn.exec_params(fetch_query, email);
        //Check The Status After Running The Query
        if (!r.empty()) {
            return crow::response(409, "User Already Exists !");
        }
//Query To Insert New User To Database
        std::string insert_query = "INSERT INTO Users (username, email, password) VALUES ($1, $2, $3)";
        //Execute Insert Query
        txn.exec_params(insert_query, username, email, password);
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
        //Perform Password Matching
        if (password != stored_password) {
            return crow::response(404, "Invalid Email Or Password !");
        }
        //Response Under Successful Login
        return crow::response(200, "Login Successful !"); });
  // Open Port Number 18080 To Establish The Connection Between Device And Server
  // Multithread Id Enabled To Handle Multiple Request At A Single Time
  app.port(18080).multithreaded().run();
  // Successful Execution Of Code
  return 0;
}
