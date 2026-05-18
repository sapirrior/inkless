# Inkless

Inkless is a terminal-based text pager written in C11 for POSIX systems. It focuses on minimalist design, zero external dependencies, and stable layout management.

## Technical Specifications

- **Language**: C11 (`-std=c11`)
- **Portability**: POSIX-compliant (`_POSIX_C_SOURCE=200809L`)
- **Dependencies**: Standard C Library only
- **Build System**: Makefile

## Core Features

- **Word-Wrapping**: Splits lines at whitespace or hyphens.
- **Fixed Margins**: Applies a constant 8% side-margin to the viewport.
- **Regex Search**: Forward and backward search using POSIX Extended Regular Expressions.
- **Line Numbering**: Optional line number display (`:N`) within the margin.
- **Navigation**: Supports line-by-line, page-by-page, and direct line jumping (`:<number>`).
- **Multi-File**: Support for viewing multiple files via `:n` and `:p`.

## Build Instructions

To compile the binary:

```bash
make
```

The executable will be located at `build/inkl`.

## Usage

```bash
./build/inkl <filename> [filename2 ...]
```

## Commands

| Key | Action |
|-----|--------|
| `j` / `k` | Scroll down / up (one line) |
| `d` / `u` | Scroll down / up (half page) |
| `f` / `Space` | Page down |
| `b` | Page up |
| `g` / `G` | Jump to start / end of file |
| `/pattern` | Search forward |
| `?pattern` | Search backward |
| `n` / `N` | Next / previous search match |
| `:N` | Toggle line numbers |
| `:<num>` | Jump to line `<num>` |
| `:n` / `:p` | Next / previous file |
| `h` | Show help menu |
| `q` | Quit |

## License

MIT License - Copyright (c) 2026 Nolan Stark
