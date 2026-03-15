#pragma once
#include <string>
#include <vector>
#include "Policy.h"

struct Role {
    std::string name;
    std::vector<Policy> policies;
};
