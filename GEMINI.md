# Project Ink: High-Integrity Terminal Pager

Ink is a minimalist, high-performance successor to GNU `less`, written in C11 with a focus on POSIX compatibility and zero external dependencies. It treats the terminal as a structured canvas, prioritizing readability through specialized layout algorithms.

## 1. Technical Specifications
- **Language**: C11 (`-std=c11`)
- **Portability**: POSIX-compliant (`_POSIX_C_SOURCE=200809L`)
- **Build System**: Makefile with strict flags (`-Wall -Wextra -pedantic`)
- **Dependencies**: None (Standard C Library & POSIX APIs only)

## 2. Architectural Design
The project is modularized into three distinct layers:
- **`src/core/`**: 
    - `document`: Line-buffered file I/O using `getline`.
    - `layout`: The layout engine responsible for dynamic 8% margins and smart word-wrapping (preventing mid-word breaks). It maintains a `raw_to_display` map for precise navigation.
- **`src/ui/`**:
    - `terminal`: POSIX `termios` raw mode management and ANSI escape sequence handling.
    - `renderer`: Aggregate `RenderBuf` implementation for zero-flicker transactional updates.
    - `input`: Non-blocking multi-byte ANSI sequence parsing for navigation keys.
- **`src/utils/`**: Robust error handling and common utility routines.

## 3. Core Features
- **Smart Word-Wrapping**: Splits text at spaces or hyphens to maintain legibility.
- **Dynamic Margins**: Automatically applies 8% side padding based on terminal width.
- **Responsive Resizing**: Full `SIGWINCH` support; layout and margins recompute instantly on terminal resize.
- **Advanced Navigation**:
    - Line-by-line (`j`, `k`)
    - Half-page (`d`, `u`)
    - Full-page (`f`, `Space`, `b`)
    - Document jumps (`g`, `G`, `<`, `>`)
- **Regex Search Engine**:
    - Forward (`/`) and Backward (`?`) search using POSIX Extended Regular Expressions.
    - Repeating searches (`n`, `N`) with direction persistence.
- **GNU Less Aesthetic**: Minimalist colon prompt with inverted `(END)` indicator at file completion.

## 4. Coding Conventions
- **Naming**: `snake_case` for functions/variables, `PascalCase` for structs.
- **Integrity**: Transactional rendering and strict signal handling for terminal state restoration.
- **Tone**: Technical documentation and commit messages follow a factual, hyperbole-free professional standard.
