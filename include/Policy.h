#pragma once
#include <string>
#include <vector>
#include "Statement.h"

struct Policy {
    std::string name;
    std::vector<Statement> statements;
};
