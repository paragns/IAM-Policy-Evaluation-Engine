#pragma once
#include <string>
#include <unordered_map>
#include "Role.h"

class RoleManager {
public:
    void loadRole(const std::string& filepath);
    const Role* getRole(const std::string& roleName) const;

private:
    std::unordered_map<std::string, Role> roles;
};
