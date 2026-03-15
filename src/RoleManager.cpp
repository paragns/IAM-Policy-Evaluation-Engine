#include "RoleManager.h"
#include <fstream>
#include <stdexcept>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

void RoleManager::loadRole(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open role file: " + filepath);
    }

    json data = json::parse(file);

    Role role;
    role.name = data["name"];

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

        role.policies.push_back(policy);
    }

    roles[role.name] = role;
}

const Role* RoleManager::getRole(const std::string& roleName) const {
    auto it = roles.find(roleName);
    if (it == roles.end()) {
        return nullptr;
    }
    return &it->second;
}
