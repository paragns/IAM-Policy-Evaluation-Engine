#include "PolicyLoader.h"
#include <fstream>
#include <stdexcept>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

User PolicyLoader::loadUser(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open policy file: " + filepath);
    }

    json data = json::parse(file);

    User user;
    user.username = data["username"];

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

    return user;
}
