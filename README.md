# Valkara_JobSystemProtoType

**Valkara_JobSystemProtoType** is a multi-phase C++ job system prototype, designed for high-performance parallel task execution. The goal is to build a modern, efficient job system from the ground up—starting with a thread pool and evolving toward advanced concepts like lock-free queues, fibers, and job dependencies.

This project is being developed incrementally in clearly defined phases, each focused on specific subsystems and design concerns relevant to modern game engine backends and real-time applications.

---

## 🚧 Current Status

**Phase 1 – In Progress**  
Currently implementing the foundational job system using a custom thread pool and thread-safe task queue.

---

## 📌 Project Phases & Roadmap

### ✅ Phase 1: Core Infrastructure
- [x] Thread pool
- [x] Thread-safe queue or data structure
- [x] Worker threads pulling tasks with graceful shutdown
- [ ] Basic job system built on top of the thread pool

---

### 🔜 Phase 2: Job System Enhancements
- [ ] Job sorting (e.g., priority-based execution)
- [ ] Basic profiling & job timing
- [ ] Workload balancing / job stealing

---

### 🔮 Phase 3: Tagging, Dependencies, and Thread Affinity
- [ ] Job dependencies
- [ ] Job tags/types (e.g., AI, Rendering, Audio)
- [ ] Thread specifiers (restrict jobs to certain threads)

---

### 🧪 Phase 4: Advanced Systems Research
- [ ] Lock-free queue implementations
- [ ] Research into fiber-based job switching (inspired by Naughty Dog’s GDC talk: *"Parallelizing the Naughty Dog Engine Using Fibers"*)

---

## 🛠 Tech Stack

- **Language:** C++17
- **Platform:** Windows
- **Build System:** Visual Studio 2022
- **License:** TODO

---

## 📚 Goals

- Build a lean, modern job system suitable for game engine or simulation work.
- Understand and prototype common and advanced job scheduling features.
- Prepare groundwork for future integration into more complex systems (e.g., ECS, task-based AI, rendering pipelines).

---

## ⚠️ Disclaimer

This is an active prototype and research project. The code is evolving quickly and may change significantly between commits. Contributions, suggestions, and critiques are welcome.

---

## 📎 Related Resources

- 🔗 [Unity: Job System + ECS (GDC Vault)](https://gdcvault.com/play/1024839/Job-System-Entity-Component-System)
- 🔗 [Parallelizing the Naughty Dog Engine Using Fibers – GDC Talk](https://www.youtube.com/watch?v=JWFIR7z6xmc)
- 🔗 [GCAP 2016 – Parallel Game Engine Design, Brooke Hodgman](https://www.youtube.com/watch?v=w8f_fz7ZtKA)
- 🔗 [Intel Threading Building Blocks](https://www.threadingbuildingblocks.org/)
- 🔗 [CppCon – Task/Job System Design Talks](https://www.youtube.com/results?search_query=cppcon+job+system)

