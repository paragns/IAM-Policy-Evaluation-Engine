# IAM Policy Evaluation Engine (C++)

A simplified IAM-style authorization engine implemented in C++.  
The system evaluates whether a user is authorized to perform an action on a specific resource based on defined policies.

This project explores how modern cloud platforms implement **policy-based authorization systems**.

Example query:
evaluate("demo-user", "s3:GetObject", "bucket1/file.txt")

Result:
ALLOW
or
DENY

---

# Motivation

Modern infrastructure platforms rely heavily on **fine-grained authorization systems**.  
Cloud services such as AWS use IAM policies to control access to resources.

This project implements a simplified version of such a system to explore:

- Policy evaluation logic
- Role-based access control (RBAC)
- Wildcard resource matching
- Authorization engine design

---

# Features

- Policy-based authorization
- JSON policy definitions
- User and role support
- Wildcard resource matching
- Explicit allow/deny rule evaluation
- Command-line policy simulation tool

---

# Authorization Model

The engine evaluates requests of the form:
(user, action, resource)
Example:
("demo-user", "s3:GetObject", "bucket1/file.txt")
Policies define whether this request should be **allowed or denied**.
---
# Policy Format

Policies are defined using JSON.

Example:

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

# Policy components:

Field	Description
Effect	Allow or Deny
Action	Operation being performed
Resource	Resource pattern
Policy Evaluation Rules

The authorization engine follows this rule order:

Explicit Deny > Allow > Default Deny

Meaning:

If any policy explicitly denies the request → DENY

If any policy allows the request → ALLOW

Otherwise → DENY

# Project Structure
iam-policy-engine
│
├── src
│
├── include
│
├── policies
│
├── README.md
└── ROADMAP.md

