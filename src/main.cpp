#include <iostream>
#include "PolicyLoader.h"

int main() {
    PolicyLoader loader;
    User user = loader.loadUser("policies/demo-user.json");

    std::cout << "User: " << user.username << "\n";

    for (const auto& policy : user.policies) {
        std::cout << "Policy: " << policy.name << "\n";
        for (const auto& stmt : policy.statements) {
            std::cout << "  Statement:\n";
            std::cout << "    Effect:   " << (stmt.effect == Effect::Allow ? "Allow" : "Deny") << "\n";
            std::cout << "    Action:   " << stmt.action << "\n";
            std::cout << "    Resource: " << stmt.resource << "\n";
        }
    }

    return 0;
}
