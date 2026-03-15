#pragma once
#include <string>
#include "User.h"

class PolicyLoader {
public:
    User loadUser(const std::string& filepath);
};
