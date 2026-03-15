#include <iostream>
#include "UserManager.h"
#include "RoleManager.h"
#include "PolicyEngine.h"

void printDecision(const std::string& action, const std::string& resource, Decision decision) {
    std::cout << "evaluate(\"" << action << "\", \"" << resource << "\") → "
              << (decision == Decision::Allow ? "ALLOW" : "DENY") << "\n";
}

int main() {
    // Phase 1 — register all roles and users
    RoleManager roleManager;
    roleManager.loadRole("policies/roles/s3-reader.json");

    UserManager userManager;
    userManager.loadUser("policies/demo-user.json");

    PolicyEngine engine(roleManager);

    // Phase 2 — process the query
    const User* user = userManager.getUser("demo-user");
    if (!user) {
        std::cerr << "User not found\n";
        return 1;
    }

    // ALLOW — inherited from s3-reader role
    printDecision("s3:GetObject", "bucket1/file.txt",
        engine.evaluate(*user, "s3:GetObject", "bucket1/file.txt"));

    // DENY — explicitly denied in user's own policy
    printDecision("s3:DeleteObject", "bucket1/file.txt",
        engine.evaluate(*user, "s3:DeleteObject", "bucket1/file.txt"));

    // DENY — no policy covers this action
    printDecision("s3:PutObject", "bucket1/file.txt",
        engine.evaluate(*user, "s3:PutObject", "bucket1/file.txt"));

    return 0;
}
