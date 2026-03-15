#pragma once
#include <string>
#include <unordered_map>
#include "User.h"

class UserManager {
public:
    void loadUser(const std::string& filepath);
    const User* getUser(const std::string& username) const;

private:
    std::unordered_map<std::string, User> users;
};
