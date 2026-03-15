#include <iostream>
#include "PolicyLoader.h"
#include "PolicyEngine.h"

void printDecision(const std::string& action, const std::string& resource, Decision decision) {
    std::cout << "evaluate(\"" << action << "\", \"" << resource << "\") → "
              << (decision == Decision::Allow ? "ALLOW" : "DENY") << "\n";
}

int main() {
    PolicyLoader loader;
    User user = loader.loadUser("policies/demo-user.json");

    PolicyEngine engine;

    // Should be ALLOW — matches the Allow statement
    printDecision("s3:GetObject", "bucket1/file.txt",
        engine.evaluate(user, "s3:GetObject", "bucket1/file.txt"));

    // Should be DENY — matches the Deny statement
    printDecision("s3:DeleteObject", "bucket1/file.txt",
        engine.evaluate(user, "s3:DeleteObject", "bucket1/file.txt"));

    // Should be DENY — no matching statement at all (default deny)
    printDecision("s3:PutObject", "bucket1/file.txt",
        engine.evaluate(user, "s3:PutObject", "bucket1/file.txt"));

    return 0;
}
