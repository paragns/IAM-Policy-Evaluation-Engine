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

    // Wildcard: bucket1/* should match any file inside bucket1/
    printDecision("s3:GetObject", "bucket1/file.txt",
        engine.evaluate(user, "s3:GetObject", "bucket1/file.txt"));

    printDecision("s3:GetObject", "bucket1/images/photo.jpg",
        engine.evaluate(user, "s3:GetObject", "bucket1/images/photo.jpg"));

    // Deny also uses wildcard
    printDecision("s3:DeleteObject", "bucket1/file.txt",
        engine.evaluate(user, "s3:DeleteObject", "bucket1/file.txt"));

    // Different bucket — should not match bucket1/*
    printDecision("s3:GetObject", "bucket2/file.txt",
        engine.evaluate(user, "s3:GetObject", "bucket2/file.txt"));

    // No matching action
    printDecision("s3:PutObject", "bucket1/file.txt",
        engine.evaluate(user, "s3:PutObject", "bucket1/file.txt"));

    return 0;
}
