#include <iostream>
#include "User.h"
#include "PolicyEngine.h"

int main() {
    // Build a statement: Allow s3:GetObject on bucket1/file.txt
    Statement stmt;
    stmt.effect = Effect::Allow;
    stmt.action = "s3:GetObject";
    stmt.resource = "bucket1/file.txt";

    // Wrap it in a policy
    Policy policy;
    policy.name = "S3ReadPolicy";
    policy.statements.push_back(stmt);

    // Attach the policy to a user
    User user;
    user.username = "demo-user";
    user.policies.push_back(policy);

    // Print out the model to verify it was built correctly
    std::cout << "User: " << user.username << "\n";
    std::cout << "Policy: " << user.policies[0].name << "\n";
    std::cout << "Statement:\n";
    std::cout << "  Effect:   " << (user.policies[0].statements[0].effect == Effect::Allow ? "Allow" : "Deny") << "\n";
    std::cout << "  Action:   " << user.policies[0].statements[0].action << "\n";
    std::cout << "  Resource: " << user.policies[0].statements[0].resource << "\n";

    return 0;
}
