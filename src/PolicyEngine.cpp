#include "PolicyEngine.h"
#include "ResourceMatcher.h"

Decision PolicyEngine::evaluate(const User& user, const std::string& action, const std::string& resource) {
    bool allowed = false;

    for (const auto& policy : user.policies) {
        for (const auto& stmt : policy.statements) {
            if (stmt.action == action && resourceMatches(stmt.resource, resource)) {
                if (stmt.effect == Effect::Deny) {
                    return Decision::Deny;  // Explicit Deny — stop immediately
                }
                if (stmt.effect == Effect::Allow) {
                    allowed = true;         // Mark allowed, but keep checking for Denies
                }
            }
        }
    }

    return allowed ? Decision::Allow : Decision::Deny;  // Default Deny if nothing matched
}
