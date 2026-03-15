#pragma once
#include <string>

enum class Effect {
    Allow,
    Deny
};

struct Statement {
    Effect effect;
    std::string action;
    std::string resource;
};
