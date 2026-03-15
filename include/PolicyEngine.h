#pragma once
#include "User.h"
#include "RoleManager.h"

enum class Decision {
    Allow,
    Deny
};

class PolicyEngine {
public:
    PolicyEngine(const RoleManager& roleManager);
    Decision evaluate(const User& user, const std::string& action, const std::string& resource);

private:
    const RoleManager& roleManager;
};
