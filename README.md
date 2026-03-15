# IAM Policy Evaluation Engine (C++)

A simplified IAM-style authorization engine implemented in C++.
The system evaluates whether a user is authorized to perform an action on a specific resource based on defined policies.

This project explores how modern cloud platforms implement **policy-based authorization systems**.

---

## Motivation

Modern infrastructure platforms rely heavily on **fine-grained authorization systems**.
Cloud services such as AWS use IAM policies to control access to resources.

This project implements a simplified version of such a system to explore:

- Policy evaluation logic
- Role-based access control (RBAC)
- Wildcard resource matching
- Authorization engine design

---

## Features

- Policy-based authorization
- JSON policy definitions
- User and role support
- Wildcard resource matching (`bucket1/*`)
- Explicit deny always overrides allow
- Default deny when no policy matches
- Command-line policy simulation tool

---

## Authorization Model

The engine evaluates requests of the form:

```
(user, action, resource)
```

Example:

```
("demo-user", "s3:GetObject", "bucket1/file.txt")
```

**Evaluation rule precedence:**

```
Explicit Deny  >  Allow  >  Default Deny
```

- If any policy explicitly denies the request → **DENY**
- If any policy allows the request → **ALLOW**
- If no policy matches → **DENY** (default)

---

## Project Structure

```
IAM-Policy-Evaluation-Engine/
├── include/
│   ├── Statement.h         ← single policy rule (effect, action, resource)
│   ├── Policy.h            ← named group of statements
│   ├── User.h              ← username, own policies, assigned role names
│   ├── Role.h              ← role name and its policies
│   ├── UserManager.h       ← loads and stores all users
│   ├── RoleManager.h       ← loads and stores all roles
│   ├── PolicyLoader.h      ← deserializes JSON into C++ objects
│   ├── PolicyEngine.h      ← core evaluation algorithm
│   └── ResourceMatcher.h   ← wildcard pattern matching
├── src/
│   ├── main.cpp            ← CLI entry point
│   ├── PolicyLoader.cpp
│   ├── PolicyEngine.cpp
│   ├── ResourceMatcher.cpp
│   ├── RoleManager.cpp
│   └── UserManager.cpp
├── policies/
│   ├── demo-user.json      ← example user policy
│   └── roles/
│       └── s3-reader.json  ← example role policy
├── third_party/
│   └── nlohmann/
│       └── json.hpp        ← JSON parsing library
└── CMakeLists.txt
```

---

## Policy Format

### User policy (`policies/demo-user.json`)

```json
{
  "username": "demo-user",
  "roles": ["s3-reader"],
  "policies": [
    {
      "name": "DenyDeletePolicy",
      "statements": [
        {
          "effect": "Deny",
          "action": "s3:DeleteObject",
          "resource": "bucket1/*"
        }
      ]
    }
  ]
}
```

### Role policy (`policies/roles/s3-reader.json`)

```json
{
  "name": "s3-reader",
  "policies": [
    {
      "name": "S3ReadPolicy",
      "statements": [
        {
          "effect": "Allow",
          "action": "s3:GetObject",
          "resource": "bucket1/*"
        }
      ]
    }
  ]
}
```

### Policy fields

| Field | Description |
|---|---|
| `effect` | `Allow` or `Deny` |
| `action` | Operation being performed (e.g. `s3:GetObject`) |
| `resource` | Resource pattern, supports wildcards (e.g. `bucket1/*`) |

---

## Build

**Using WSL / Linux:**

```bash
g++ -std=c++17 -Iinclude -Ithird_party \
    src/main.cpp \
    src/PolicyLoader.cpp \
    src/PolicyEngine.cpp \
    src/ResourceMatcher.cpp \
    src/RoleManager.cpp \
    src/UserManager.cpp \
    -o iam_engine
```

**Using CMake:**

```bash
cmake -S . -B build
cmake --build build
```

---

## Usage

```bash
./iam_engine evaluate <username> <action> <resource>
```

The engine automatically scans the `policies/` folder at startup and registers all users and roles.

---

## Examples

```bash
# User inherits s3:GetObject from the s3-reader role
./iam_engine evaluate demo-user s3:GetObject bucket1/file.txt
→ ALLOW

# User has an explicit Deny for s3:DeleteObject
./iam_engine evaluate demo-user s3:DeleteObject bucket1/file.txt
→ DENY

# No policy covers s3:PutObject — default deny
./iam_engine evaluate demo-user s3:PutObject bucket1/file.txt
→ DENY

# Wildcard bucket1/* does not match bucket2/
./iam_engine evaluate demo-user s3:GetObject bucket2/file.txt
→ DENY

# Wrong number of arguments — prints usage
./iam_engine
→ Usage: iam_engine evaluate <username> <action> <resource>
```

---

## Adding new users and roles

**New user** — create `policies/<username>.json`:

```json
{
  "username": "new-user",
  "roles": ["s3-reader"],
  "policies": []
}
```

**New role** — create `policies/roles/<rolename>.json`:

```json
{
  "name": "s3-writer",
  "policies": [
    {
      "name": "S3WritePolicy",
      "statements": [
        {
          "effect": "Allow",
          "action": "s3:PutObject",
          "resource": "bucket1/*"
        }
      ]
    }
  ]
}
```

No code changes needed — just drop the file in the right folder and run.
