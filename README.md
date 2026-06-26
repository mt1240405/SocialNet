# Social Network Backend

A command-line social network backend implemented in C++ using fundamental data structures and graph algorithms. The project models user relationships, content management, and personalized feed generation while emphasizing efficient algorithms and modular software design.

---

## Features

### User Management

* Create users with unique usernames
* Constant-time username lookup
* Duplicate username detection

### Friendship Network

* Undirected friendship graph
* Duplicate friendship prevention
* Alphabetically sorted friend lists
* Mutual friend recommendations
* Degrees of separation using Breadth-First Search (BFS)

### Post Management

* Timestamped user posts
* AVL Tree based storage
* Retrieve all posts or most recent posts
* Like and Unlike support

### Personalized Feed

* Graph-based candidate generation
* Affinity-based candidate allocation
* Feed ranking using:

  * Engagement (likes)
  * Social affinity
  * Temporal freshness

### Persistence

* Command journaling
* Automatic state reconstruction through command replay

---

## Data Structures

| Component                | Data Structure       |
| ------------------------ | -------------------- |
| Users                    | `unordered_map`      |
| Friendships              | Adjacency List Graph |
| Duplicate Edge Detection | `unordered_set`      |
| User Posts               | AVL Tree             |
| Feed Candidates          | `vector`             |

---

## Algorithms

* Breadth First Search
* AVL Rotations
* Graph Traversal
* Candidate Generation
* Feed Ranking
* Command Replay

---

## Complexity

| Operation             |             Complexity |
| --------------------- | ---------------------: |
| Add User              |                   O(1) |
| Add Friendship        |                   O(1) |
| List Friends          |             O(F log F) |
| Friend Recommendation | O(Σ neighbour degrees) |
| Degrees of Separation |               O(V + E) |
| Insert Post           |               O(log P) |
| Like / Unlike         |                   O(P) |
| Feed Generation       |   O(F log F + C log C) |

---

## Build

```bash
g++ -std=c++17 *.cpp
```

---

## Project Structure

```text
Graph/
AVL Tree/
Social Network/
Command Journal/
```

---

## Documentation

See **ARCHITECTURE.md** for implementation details and design decisions.
