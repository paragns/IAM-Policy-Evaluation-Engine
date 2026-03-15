#include "PolicyEngine.h"
#include "ResourceMatcher.h"

PolicyEngine::PolicyEngine(const RoleManager& roleManager) : roleManager(roleManager) {}

static bool checkPolicies(const std::vector<Policy>& policies, const std::string& action, const std::string& resource, bool& allowed) {
    for (const auto& policy : policies) {
        for (const auto& stmt : policy.statements) {
            if (stmt.action == action && resourceMatches(stmt.resource, resource)) {
                if (stmt.effect == Effect::Deny) {
                    return true;  // signal an explicit Deny was found
                }
                if (stmt.effect == Effect::Allow) {
                    allowed = true;
                }
            }
        }
    }
    return false;
}

Decision PolicyEngine::evaluate(const User& user, const std::string& action, const std::string& resource) {
    bool allowed = false;

    // Check user's own policies
    if (checkPolicies(user.policies, action, resource, allowed)) {
        return Decision::Deny;
    }

    // Check policies inherited from roles
    for (const auto& roleName : user.roles) {
        const Role* role = roleManager.getRole(roleName);
        if (role && checkPolicies(role->policies, action, resource, allowed)) {
            return Decision::Deny;
        }
    }

    return allowed ? Decision::Allow : Decision::Deny;
}
