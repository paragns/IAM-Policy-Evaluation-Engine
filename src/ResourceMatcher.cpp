#include "ResourceMatcher.h"

bool resourceMatches(const std::string& pattern, const std::string& resource) {
    int p = 0;  // position in pattern
    int r = 0;  // position in resource

    while (p < pattern.size() && r < resource.size()) {
        if (pattern[p] == '*') {
            // '*' matches any sequence — try matching the rest of the pattern
            // against every possible position in the remaining resource string
            while (r <= (int)resource.size()) {
                if (resourceMatches(pattern.substr(p + 1), resource.substr(r))) {
                    return true;
                }
                r++;
            }
            return false;
        }

        if (pattern[p] != resource[r]) {
            return false;  // characters don't match
        }

        p++;
        r++;
    }

    // Skip any trailing '*' in the pattern
    while (p < pattern.size() && pattern[p] == '*') {
        p++;
    }

    return p == pattern.size() && r == resource.size();
}
