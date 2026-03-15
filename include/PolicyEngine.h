#pragma once
#include "User.h"

enum class Decision {
    Allow,
    Deny
};

class PolicyEngine {
public:
    Decision evaluate(const User& user, const std::string& action, const std::string& resource);
};
