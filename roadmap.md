# Project Roadmap — IAM Policy Evaluation Engine

This project implements a simplified IAM-style authorization engine in C++.
The system evaluates whether a user can perform a specific action on a resource based on defined policies.

Example authorization query:

```cpp
evaluate("demo-user", "s3:GetObject", "bucket1/file.txt")
```

Result:

```
ALLOW
```

or

```
DENY
```

The project is built incrementally across several chapters, each introducing key concepts in authorization systems.

---

# Chapter 1 — Authorization Model Design

## Goal
Define the core IAM entities and their relationships.

## Concepts to Learn
- Access control models (RBAC vs ABAC)
- Identity vs permission separation
- Policy-based authorization
- Authorization decision flow

## Implementation
Design the core data structures:

- `User`
- `Role`
- `Policy`
- `Statement`
- `PolicyEngine`

Policy structure:

```
Policy
└── Statements
    ├── Effect (Allow / Deny)
    ├── Action
    └── Resource
```

This chapter focuses only on **data modeling**, not evaluation logic.

---

# Chapter 2 — JSON Policy Parsing

## Goal
Allow policies to be defined using external JSON files.

## Concepts to Learn
- JSON parsing in C++
- Configuration-driven systems
- Serialization and deserialization

## Implementation
Use the `nlohmann/json` library to load policies.

Example policy format:

```json
{
  "Statement": [
    {
      "Effect": "Allow",
      "Action": "s3:GetObject",
      "Resource": "bucket1/*"
    }
  ]
}
```

Convert JSON policies into internal `Policy` and `Statement` objects.

---

# Chapter 3 — Policy Evaluation Engine

## Goal
Implement the core authorization decision logic.

## Concepts to Learn
- Policy evaluation algorithms
- Authorization rule precedence
- Matching requests against policy statements

Evaluation rule:

```
Explicit Deny > Allow > Default Deny
```

## Implementation
Create the main evaluation function:

```cpp
Decision evaluate(user, action, resource)
```

Evaluation steps:

1. Gather policies attached to the user
2. Find matching policy statements
3. Apply evaluation rules
4. Return authorization decision

---

# Chapter 4 — Wildcard Resource Matching

## Goal
Support flexible resource patterns in policies.

Example pattern:

```
bucket1/*
```

Matching resources:

```
bucket1/file1
bucket1/file2
```

## Concepts to Learn
- Pattern matching
- Resource hierarchy design
- String matching algorithms

## Implementation
Implement a function:

```cpp
bool resourceMatches(pattern, resource)
```

Support patterns like:

```
bucket/*
bucket/images/*
bucket/file.txt
```

---

# Chapter 5 — Users and Roles (RBAC)

## Goal
Introduce role-based access control.

Users can inherit permissions from roles.

## Concepts to Learn
- RBAC architecture
- Permission inheritance
- Policy aggregation

## Implementation
Add classes:

- `Role`
- `RoleManager`

Evaluation model:

```
User Policies
     +
Role Policies
     =
Effective Permissions
```

The policy engine should evaluate permissions from both sources.

---

# Chapter 6 — CLI Policy Simulation Tool

## Goal
Provide a command-line interface to test authorization decisions.

## Concepts to Learn
- CLI tools
- Integration testing
- System simulation

## Implementation
Example command:

```
./iam_engine evaluate demo-user s3:GetObject bucket1/file.txt
```

Output:

```
ALLOW
```

or

```
DENY
```

This tool allows quick testing of different users, actions, and resources.

---

# Final Outcome

After completing all chapters, the project will implement:

- IAM-style policy evaluation
- JSON policy configuration
- Wildcard resource matching
- Role-based access control (RBAC)
- CLI authorization simulator

## Future Extensions

Possible improvements:

- Policy conditions
- Attribute-based access control (ABAC)
- Policy versioning
- REST API interface
- Policy simulation tools

---

When you start tomorrow, **Chapter 1 will be the most important part**, because **the class architecture determines how clean the whole project will be**.

When you're ready, ask:

> "Let's design the IAM class architecture."

That will be the first step of Chapter 1.
