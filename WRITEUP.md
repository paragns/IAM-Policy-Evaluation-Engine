# IAM Policy Evaluation Engine — Project Write-up

## What is the project?

I built a simplified **IAM (Identity and Access Management) Policy Evaluation Engine** in C++ — the same kind of system that powers AWS IAM under the hood. The engine answers one question:

> Given a user, an action, and a resource — is the user authorized or not?

For example:

```bash
./iam_engine evaluate demo-user s3:GetObject bucket1/file.txt
→ ALLOW
```

This is purely an **authorization** engine — authentication is out of scope. The system assumes the user's identity is already known and trusted.

---

## Core Concepts

**Authorization rule precedence — the golden rule:**

```
Explicit Deny  >  Allow  >  Default Deny
```

- If any policy explicitly denies — always DENY, no exceptions
- If at least one policy allows and nothing denies — ALLOW
- If nothing matches — DENY by default (never grant access by accident)

**RBAC — Role Based Access Control:**
Users can be assigned roles. A role is a bundle of policies shared across many users. This avoids duplicating permissions — define once, assign to many.

---

## Implementation Plan

I built the project incrementally across 6 chapters, each introducing one concept:

| Chapter | What was built                                     |
| ------- | -------------------------------------------------- |
| 1       | Data model — `Statement`, `Policy`, `User`, `Role` |
| 2       | JSON policy loader using `nlohmann/json`           |
| 3       | Core evaluation engine — ALLOW/DENY logic          |
| 4       | Wildcard resource matching (`bucket1/*`)           |
| 5       | RBAC — `RoleManager` and `UserManager`             |
| 6       | CLI tool — terminal input, auto policy loading     |

---

## Architecture

```
policies/
├── demo-user.json          ← user's own policies + assigned role names
└── roles/
    └── s3-reader.json      ← role's policies

UserManager                 ← loads and stores all users in a map
RoleManager                 ← loads and stores all roles in a map
PolicyLoader                ← deserializes JSON into C++ objects
ResourceMatcher             ← wildcard pattern matching (bucket1/*)
PolicyEngine                ← runs the evaluation algorithm
```

**Data model:**

```
User
├── username
├── roles: ["s3-reader"]       ← role names, looked up in RoleManager
└── policies: [Policy]
         └── Policy
              └── statements: [Statement]
                       ├── effect: Allow | Deny
                       ├── action: "s3:GetObject"
                       └── resource: "bucket1/*"
```

---

## How it works at runtime

**Phase 1 — Startup:**
The program scans the `policies/` folder and registers every JSON file:

```
policies/roles/*.json  →  RoleManager  (map of role name → Role)
policies/*.json        →  UserManager  (map of username → User)
```

**Phase 2 — Evaluate:**

```
CLI input: demo-user s3:GetObject bucket1/file.txt
    │
    ├── UserManager.getUser("demo-user")    → User object
    │
    ├── checkPolicies(user.own policies)
    │       Deny? → return DENY immediately
    │       Allow? → mark allowed = true
    │
    ├── for each role in user.roles:
    │       RoleManager.getRole("s3-reader") → Role object
    │       checkPolicies(role.policies)
    │               Deny? → return DENY immediately
    │               Allow? → mark allowed = true
    │
    └── allowed = true → ALLOW
        allowed = false → DENY (default)
```

---

## Key Design Decisions

**Why `struct` for data, `class` for behavior?**
`Statement`, `Policy`, `User`, `Role` are pure data containers — all fields public, no logic. `struct` is the C++ convention for this. `PolicyEngine`, `RoleManager`, `UserManager` have behavior and encapsulation so they use `class`.

**Why separate `ResourceMatcher`?**
Pattern matching has nothing to do with authorization logic. Keeping it separate means it can be tested independently and reused without touching the engine.

**Why `RoleManager` and `UserManager` instead of storing full objects in User?**
If 100 users are all assigned `s3-reader`, storing the full role object in each user duplicates data 100 times. The managers are a single source of truth — users just hold the role name as a string key.

**Why header-only `nlohmann/json`?**
C++ has no built-in JSON support. `nlohmann/json` is a single `.hpp` file — no separate compilation, no build configuration. Drop it in and include it.

**Why `std::filesystem` in Chapter 6?**
Hardcoding file paths doesn't scale. Scanning the directory automatically means adding a new user or role is just dropping a JSON file in the right folder — no code changes needed.

---

## What I learned

- Domain modeling in C++ — designing clean data structures before writing logic
- Separation of concerns — each class has one job, changes in one don't ripple through others
- How real IAM systems work — the evaluation algorithm, default deny, explicit deny precedence
- RBAC architecture — permission inheritance through roles
- Third party library integration in C++ — header-only libraries
- Build systems — CMake and compiling multi-file C++ projects
