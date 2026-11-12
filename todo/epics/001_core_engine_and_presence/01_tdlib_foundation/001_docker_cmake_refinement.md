# Task: Docker, CMake & Modular Project Restructuring
Implementation:
- **Restructure:** Move `main.cpp` from root to `src/main.cpp`.
- **Modular CMake Architecture (Incremental Builds):** 
    - Implement a hierarchical `CMakeLists.txt` structure.
    - Root `CMakeLists.txt`: Global settings, dependencies (TDLib), and `add_subdirectory(src)`.
    - `src/CMakeLists.txt`: Orchestrates subdirectories (`core`, `engine`, etc.).
    - Module `CMakeLists.txt` (e.g., `src/core/CMakeLists.txt`): Defines each module as a separate library target (e.g., `core_lib`) using `add_library`.
    - Use `target_link_libraries` to manage inter-module dependencies (e.g., `engine_lib` $\rightarrow$ `core_lib`).
    - Final executable links all module libraries.
- **Dockerfile:** Update to reflect the new source path and include TDLib build dependencies.
- **Docker Compose:** Update `docker-compose.yml` to ensure `data/tdlib_db/` is a persistent volume.

Verification:
- [ ] Run `docker compose build`. Ensure no errors in CMake configuration or compilation.
- [ ] Verify that changing a file in `src/core/` only triggers recompilation of `core_lib` and the final link step (incremental build check).

Cleanup:
- [ ] Remove any temporary build artifacts or hardcoded paths used during testing.
- [ ] Ensure all include paths are relative to the project root.
