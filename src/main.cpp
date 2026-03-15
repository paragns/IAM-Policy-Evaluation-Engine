#include <iostream>
#include <filesystem>
#include "UserManager.h"
#include "RoleManager.h"
#include "PolicyEngine.h"

namespace fs = std::filesystem;

void loadAll(UserManager& userManager, RoleManager& roleManager) {
    // Load all roles first
    for (const auto& entry : fs::directory_iterator("policies/roles")) {
        if (entry.path().extension() == ".json") {
            roleManager.loadRole(entry.path().string());
        }
    }

    // Load all users
    for (const auto& entry : fs::directory_iterator("policies")) {
        if (fs::is_regular_file(entry) && entry.path().extension() == ".json") {
            userManager.loadUser(entry.path().string());
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5 || std::string(argv[1]) != "evaluate") {
        std::cerr << "Usage: iam_engine evaluate <username> <action> <resource>\n";
        return 1;
    }

    std::string username = argv[2];
    std::string action   = argv[3];
    std::string resource = argv[4];

    // Phase 1 — load all users and roles from policies/ folder
    UserManager userManager;
    RoleManager roleManager;
    loadAll(userManager, roleManager);

    // Phase 2 — evaluate the query
    const User* user = userManager.getUser(username);
    if (!user) {
        std::cerr << "User not found: " << username << "\n";
        return 1;
    }

    PolicyEngine engine(roleManager);
    Decision decision = engine.evaluate(*user, action, resource);

    std::cout << (decision == Decision::Allow ? "ALLOW" : "DENY") << "\n";

    return 0;
}
