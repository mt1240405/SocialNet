# Architecture

## Overview

The Social Network Backend is organized into independent modules, each responsible for a single aspect of the application. This separation minimizes coupling between components while keeping the implementation modular and maintainable.

```text
                    User Commands
                          │
                          ▼
                  SocialNetwork API
                          │
        ┌─────────────────┼─────────────────┐
        ▼                 ▼                 ▼
      Graph           AVL Trees      Command Journal
        │                 │                 │
        ▼                 ▼                 ▼
 Friendships          User Posts       Persistent State
```

The `SocialNetwork` class acts as the application service layer and coordinates all interactions between the underlying data structures.

---

# Module Responsibilities

## 1. Graph Module

The Graph module manages all relationship-based operations.

### Responsibilities

* User storage
* Username-to-ID mapping
* Friendship management
* Duplicate friendship detection
* Mutual friend computation
* Friend recommendation
* Degrees of separation
* Feed affinity computation

### Internal Representation

The friendship network is represented as an adjacency list.

Each user is assigned a unique integer ID.

Friendships are stored as edges between user IDs.

An additional hash set is maintained to detect duplicate friendships in expected **O(1)** time without affecting graph traversal performance.

---

## 2. AVL Tree Module

Every user owns an independent AVL Tree containing their posts.

Each tree node stores

* Post content
* Creation timestamp
* Like count

The AVL Tree automatically maintains balance after every insertion, ensuring logarithmic insertion complexity.

### Supported Operations

* Insert new post
* Retrieve all posts
* Retrieve recent posts
* Update likes
* Traverse posts in chronological order

Unlike an unsorted container, the AVL Tree guarantees efficient insertion while preserving post ordering.

---

## 3. SocialNetwork Module

The SocialNetwork module provides the public interface of the application.

Responsibilities include

* Command validation
* User management
* Coordinating Graph and AVL operations
* Feed generation
* Persistence handling

Business logic is intentionally isolated here so that Graph and AVL Tree remain reusable components.

---

## 4. Command Journal

Persistent storage is implemented using command replay rather than explicit object serialization.

Every state-changing operation is appended to a journal.

Example

```text
ADD_USER alice
ADD_USER bob
ADD_FRIEND alice bob
ADD_POST bob Hello World
LIKE_POST bob Hello World
```

During application startup

```text
commands.log
        │
        ▼
Replay Commands
        │
        ▼
Reconstructed State
```

This approach guarantees deterministic reconstruction while avoiding custom serialization logic.

---

# Feed Generation Pipeline

Unlike traditional timeline implementations that examine every available post, the feed generation algorithm uses a **retrieval-and-ranking** architecture.

The process consists of two stages.

```text
Viewer
   │
   ▼
Retrieve Friends
   │
   ▼
Compute Affinity
   │
   ▼
Sort Friends
   │
   ▼
Allocate Candidate Budget
   │
   ▼
Retrieve Recent Posts
   │
   ▼
Rank Candidates
   │
   ▼
Top-K Feed
```

---

## Stage 1 – Candidate Generation

The objective of this stage is to reduce the search space.

Rather than evaluating every post from every friend, only a bounded number of recent posts are retrieved.

### Affinity Function

Affinity between the requesting user and each friend is computed as

```text
affinity = mutualFriendCount + 1
```

where

* `mutualFriendCount` is the number of common friends.
* `+1` guarantees every friend contributes at least one candidate post.

The affinity values are normalized

```text
weight = affinity / Σ affinity
```

A fixed candidate budget is then distributed proportionally among friends.

Each friend contributes

* At least one post
* At Most ten posts

This prevents extremely active users from dominating the candidate pool while ensuring every friend remains eligible.

---

## Stage 2 – Candidate Ranking

After candidate generation, each retrieved post receives a relevance score.

The ranking function combines

* Engagement
* Social affinity
* Temporal freshness

The current implementation uses

```text
Score =
8 log₂(likes + 1)
+
2 √(affinity)
+
3 recency
```

where

```text
recency =
1 / (1 + ageHours)
```

### Rationale

**Likes**

Popularity exhibits diminishing returns through logarithmic scaling.

A post with 1000 likes should not be 100 times more important than a post with 10 likes.

---

**Affinity**

The square-root transformation prevents friendship density from overwhelming engagement signals while still rewarding socially closer users.

---

**Recency**

Older posts gradually lose relevance over time.

Fresh posts receive a larger score without completely excluding older high-quality content.

---

After scoring, candidate posts are sorted and the highest-ranked **K** posts are returned to the user.

This retrieval-and-ranking separation closely resembles the architecture used by large-scale recommendation systems.

# Design Decisions

## Why an Adjacency List?

The friendship graph is expected to be sparse, with each user connected to only a small fraction of the total user base.

An adjacency matrix would require **O(V²)** memory regardless of the number of friendships.

An adjacency list stores only existing friendships, reducing memory usage to **O(V + E)** while allowing efficient graph traversal.

---

## Why Integer User IDs?

Internally, every user is assigned a unique integer ID.

The graph operates exclusively on IDs rather than usernames.

Benefits include

* Constant-time vector indexing
* Lower memory overhead
* Faster graph traversal
* Simpler adjacency list representation

Usernames are converted to IDs only at the application boundary through a hash map.

---

## Why an AVL Tree?

Each user's posts must remain ordered by creation time while supporting frequent insertions.

Possible alternatives were

* Linked List
* Dynamic Array
* Binary Search Tree

An AVL Tree was selected because it guarantees

* O(log n) insertion
* Automatic balancing
* Ordered traversal
* Consistent worst-case performance

Unlike an ordinary BST, insertion order cannot degrade performance.

---

## Why One AVL Tree Per User?

Posts are naturally grouped by their author.

Maintaining an independent AVL Tree for every user provides

* Better encapsulation
* Smaller balanced trees
* Simpler retrieval of a user's timeline
* No unnecessary traversal of unrelated posts

---

## Why Command Replay Instead of Serialization?

Rather than serializing the entire object graph, every state-changing command is appended to a journal.

Example

```text
ADD_USER alice
ADD_USER bob
ADD_FRIEND alice bob
ADD_POST bob Hello
LIKE_POST bob Hello
```

At startup, the application replays the journal to reconstruct the complete application state.

Advantages

* Simple implementation
* Human-readable persistence
* Easy debugging
* Deterministic reconstruction
* No custom serialization code

---

## Why Candidate Generation?

Scanning every post in the network whenever a user refreshes their feed does not scale.

Instead, the feed generation algorithm first retrieves a bounded candidate set before ranking.

Benefits

* Lower computational cost
* Predictable execution time
* Reduced memory usage
* Better separation of retrieval and ranking

---

## Why Separate Candidate Generation and Ranking?

The recommendation pipeline intentionally separates

1. Candidate Generation
2. Candidate Ranking

Candidate generation determines **which posts should be considered**.

Ranking determines **which of those candidates should appear in the final feed**.

This modularity allows additional engagement signals (comments, shares, watch time, etc.) to be incorporated into the ranking stage without modifying the retrieval stage.

---

## Why Use Recent Posts Only?

Highly active users may create many posts within a short period.

Retrieving every post from such users would

* Increase computation
* Reduce feed diversity
* Push out posts from other friends

Limiting retrieval to the most recent posts ensures a balanced candidate pool.

---

## Why Normalize Affinity?

Affinity values are converted into proportional weights before candidate allocation.

Instead of assigning the same number of candidate posts to every friend, users with stronger social connections contribute more candidates.

This improves personalization while maintaining a bounded candidate budget.

---

## Why Logarithmic Like Scaling?

Using raw like counts would allow extremely popular posts to dominate the feed.

The scoring function therefore uses

```text
log₂(likes + 1)
```

which exhibits diminishing returns.

Example

| Likes | Contribution |
| ----: | -----------: |
|     1 |         1.00 |
|    10 |         3.46 |
|   100 |         6.66 |
|  1000 |         9.97 |

This preserves popularity as an important signal without overwhelming all other factors.

---

## Why Temporal Decay?

Recent posts are generally more relevant than older ones.

Freshness is modeled as

```text
recency = 1 / (1 + ageHours)
```

This smoothly decreases a post's contribution over time without abruptly removing older posts from consideration.

---

## Why Duplicate Friendship Detection?

Although friendships are stored using adjacency lists, an additional hash set stores normalized friendship pairs.

Benefits

* Expected O(1) duplicate detection
* No duplicate edges
* Cleaner graph traversal
* Prevents repeated recommendations caused by duplicated friendships

---

# Complexity Analysis

| Operation                    |              Time Complexity |
| ---------------------------- | ---------------------------: |
| Add User                     |                         O(1) |
| Add Friendship               |                         O(1) |
| List Friends                 |                   O(F log F) |
| Mutual Friend Recommendation |       O(Σ neighbour degrees) |
| Degrees of Separation        |                     O(V + E) |
| Insert Post                  |                     O(log P) |
| Retrieve Recent Posts        |                 O(log P + K) |
| Like / Unlike                |                         O(P) |
| Generate Feed                |         O(F log F + C log C) |
| Save Command                 |                         O(1) |
| Application Reconstruction   | O(Number of Logged Commands) |

where

* **V** = Number of users
* **E** = Number of friendships
* **F** = Number of friends of the requesting user
* **P** = Number of posts owned by a user
* **C** = Number of candidate posts

---

# Extensibility

The current architecture intentionally separates storage, graph algorithms, and business logic.

This makes the project straightforward to extend.

Possible future additions include

* Comment system
* Follow graph instead of mutual friendships
* Personalized interaction history
* Machine-learned ranking
* Distributed persistence
* Real-time notifications
* Concurrent feed generation

Because the recommendation pipeline separates candidate generation from ranking, these features can be incorporated with minimal changes to the existing implementation.

---

# Summary

The project demonstrates the practical integration of

* Graph algorithms
* Balanced search trees
* Hash-based indexing
* Breadth-First Search
* Candidate generation and ranking
* Persistent command replay
* Object-oriented software design

The resulting system models many core backend components found in modern social networking platforms while maintaining efficient asymptotic performance and clear modular separation.
