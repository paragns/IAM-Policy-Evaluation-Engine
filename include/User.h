#pragma once
#include <string>
#include <vector>
#include "Policy.h"

struct User {
    std::string username;
    std::vector<Policy> policies;
};
