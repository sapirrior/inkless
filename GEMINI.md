# Project Ink: High-Integrity Terminal Pager

Ink is a minimalist, high-performance successor to GNU `less`, written in C11 with a focus on POSIX compatibility and zero external dependencies. It treats the terminal as a structured canvas, prioritizing readability through specialized layout algorithms.

## 1. Technical Specifications
- **Language**: C11 (`-std=c11`)
- **Portability**: POSIX-compliant (`_POSIX_C_SOURCE=200809L`)
- **Build System**: Makefile with strict flags (`-Wall -Wextra -pedantic`)
- **Dependencies**: None (Standard C Library & POSIX APIs only)

## 2. Architectural Design
Ink follows a Go-inspired **Domain-Driven Design (DDD)** where each feature set is encapsulated in an autonomous module.
- **Single Header Rule**: Each module directory (e.g., `src/document/`) exposes its public API through exactly ONE unified header file (`document.h`). Internal logic is split across multiple `.c` files within the same directory.
- **AppState Centralization**: A global `AppState` struct (`src/app/state.h`) anchors all domain-specific data, enabling decoupled interaction between modules.
- **Command Pattern**: User actions are strictly isolated into individual files under `src/commands/` (e.g., `nav/up.c`, `search/execute.c`), orchestrated by a central dispatcher.

### Core Modules:
- `src/app/`: Orchestration and event loop.
- `src/document/`: File I/O and text buffering.
- `src/layout/`: Smart wrapping and coordinate mapping.
- `src/terminal/`: POSIX raw mode and ANSI sequence management.
- `src/view/`: Screen rendering and prompt handling.
- `src/input/`: Keystroke parsing.
- `src/utils/`: Generic helpers and the POSIX regex search engine.
- `src/commands/`: Isolated action handlers categorized by domain.
## 3. Core Features
- **Smart Word-Wrapping**: Splits text at spaces or hyphens to maintain legibility.
- **Dynamic Margins**: Automatically applies 8% side padding based on terminal width.
- **Responsive Resizing**: Full `SIGWINCH` support; layout and margins recompute instantly on terminal resize.
- **Advanced Navigation**:
    - Line-by-line (`j`, `k`)
    - Half-page (`d`, `u`)
    - Full-page (`f`, `Space`, `b`)
    - Document jumps (`g`, `G`, `<`, `>`)
- **Interactive Help**: Access a structured, monochrome command reference by pressing `h`. Returns to document via `Esc`.
- **Regex Search Engine**:
    - Forward (`/`) and Backward (`?`) search using POSIX Extended Regular Expressions.
    - Repeating searches (`n`, `N`) with direction persistence.
    - **Visual Highlighting**: Real-time ANSI-inverted highlighting of all matches.
    - **Wraparound Search**: Automatic document loop-back when no further matches exist.
    - **Clear Search**: Press `Esc` to instantly remove all active highlighting.
- **GNU Less Aesthetic**: Minimalist colon prompt with inverted `(END)` indicator at file completion.

## 4. Getting Started
### Build Instructions
To compile Ink, ensure you have a C compiler (GCC/Clang) and Make installed.
```bash
make          # Compiles the binary to build/ink
make clean    # Removes the build directory
```

### Usage
```bash
./build/ink <filename>
```

## 5. Development Workflow
- **Version Control**: Use Git for all changes. The remote repository is hosted at `https://github.com/sapirrior/ink`.
- **Commit Messages**: Strictly follow the [Conventional Commits](https://www.conventionalcommits.org/) specification (e.g., `feat:`, `fix:`, `docs:`, `refactor:`).
- **PR Process**: All major features should be developed on feature branches and merged via pull requests.

## 6. Coding Conventions
- **Naming**: `snake_case` for functions/variables, `PascalCase` for structs.
- **Hardened Integrity**:
    - **Defensive I/O**: Strict `realloc` validation and CRLF handling during file load.
    - **Memory Safety**: OOB prevention in layout mapping and dynamic RenderBuf sizing to eliminate overflow risks.
    - **Robust Navigation**: Clamped scroll and view logic ensures stability during extreme terminal resizing.
- **Tone**: Technical documentation and commit messages follow a factual, hyperbole-free professional standard.
