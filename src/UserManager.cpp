#include "UserManager.h"
#include <fstream>
#include <stdexcept>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

void UserManager::loadUser(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open user file: " + filepath);
    }

    json data = json::parse(file);

    User user;
    user.username = data["username"];

    if (data.contains("roles")) {
        for (const auto& roleName : data["roles"]) {
            user.roles.push_back(roleName);
        }
    }

    for (const auto& policyJson : data["policies"]) {
        Policy policy;
        policy.name = policyJson["name"];

        for (const auto& stmtJson : policyJson["statements"]) {
            Statement stmt;
            stmt.action   = stmtJson["action"];
            stmt.resource = stmtJson["resource"];
            stmt.effect   = (stmtJson["effect"] == "Allow") ? Effect::Allow : Effect::Deny;
            policy.statements.push_back(stmt);
        }

        user.policies.push_back(policy);
    }

    users[user.username] = user;
}

const User* UserManager::getUser(const std::string& username) const {
    auto it = users.find(username);
    if (it == users.end()) {
        return nullptr;
    }
    return &it->second;
}
