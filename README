# C Programming Projects

A collection of practical C projects based on [GeeksForGeeks C Projects](https://www.geeksforgeeks.org/c/c-projects/), designed for learning and practicing C programming fundamentals.
Some of the projects were not able to be converted hence 2 bonus projects Reference Counting and Reference Counting inspired by Boot Dev. 

## Features

- **C17 Standard**: All projects follow the C17 specification
- **Structured Code**: Consistent organization across all projects
- **Windows-Optimized**: Written for MSVC with secure functions (`_s` variants)
- **Build Automation**: Batch scripts for compilation and cleanup

## Code Structure

Each project follows this standardized structure:

```c
// Includes
// Constants & Macros  
// Struct Definitions
// Global Variables
// Function Declarations
// Driver Code (main)
// Function Definitions
```

## Prerequisites

### Windows
- **Windows Operating System**
- **MSVC** (Microsoft Visual C++ Compiler) installed and added to PATH
  - Available through Visual Studio or Build Tools for Visual Studio
  - Verify installation: `cl` in Command Prompt should display compiler version

### Linux/macOS
- **GCC** or **Clang** compiler
- Requires minor code modifications (see Platform Compatibility below)

## Setup

### Platform Compatibility

**For Linux/macOS users**, replace MSVC-specific secure functions:
- `fopen_s()` → `fopen()`
- `strcpy_s()` → `strcpy()`
- `strcat_s()` → `strcat()`
- `scanf_s()` → `scanf()`
- `sprintf_s()` → `sprintf()`

## Build Tools

The project includes batch file utilities located in `bat_files/`:

### 1. Generate C/C++ Properties
- **Location**: Root directory  
- **File**: `0_compile_auto.bat`  
- **Purpose**: Generates `c_cpp_properties.json` for IDE configuration

### 2. Compile Scripts
- **Location**: Must be run from `src/` directory  
- **Files**: 
  - `0_compile_auto.bat` - Compiles C17 projects
  - `4_generate_c_json.bat` - Compiles C++23 projects (for conversion practice)
- **Usage**: Navigate to `src/` directory, then run the desired batch file

### 3. Cleanup Script
- **Location**: Must be run from `src/` directory  
- **File**: `1_remove.bat`  
- **Purpose**: Removes build artifacts (`.exe`, `.ilk`, `.pdb`, `.obj`)
- **Usage**: Navigate to `src/` directory, then run `1_remove.bat`

**Important**: Compilation and cleanup scripts must be executed from within the `src/` directory to work properly.

## Projects Included

The `src/` directory contains various projects including:

**Management Systems:**
- Bank Management System
- Bus Reservation System
- Employee Management System
- Hospital Management System
- School Management System
- Library Management System
- Telecom Billing System
- Online Voting System

**Games:**
- 2048
- Dino Game
- Hangman
- Pacman
- Quiz Game
- Snake Game
- Snake and Ladder
- Tic-Tac-Toe
- Rock Paper Scissors

**Utility Programs:**
- File Handling Demonstrations
- Calculator
- Coordinate System Examples
- Cricket Score Board
- Calendar
- Syntax Checker
- Lexical Analyzer
- Number System Conversion
- Refcounting GC
- Mark and Sweep GC
- Multidimensional Arrays
- Portability Check
- Simple Calculator
- Built-in Functions Demo
- Project Layout Demo

## Learning Path

1. **Start Simple**: Begin with utility programs (calculators, syntax checkers)
2. **File I/O**: Progress to management systems with file operations
3. **Complex Logic**: Tackle games for algorithm practice
4. **Challenge**: Convert C projects to C++23 as an advanced exercise

## Quick Start

1. Ensure MSVC is installed and accessible via PATH
2. Clone or download this repository
3. Navigate to the `src/` directory
4. Place and run `0_compile_auto.bat` in the `src/` directory to compile a project
5. Execute the generated `.exe` file
6. If you want to generate a `.vscode` folder with a setup for C17, run the `4_generate_c_json.bat` file which will tell MSVC that you are using C17.

## Contributing

Feel free to fork, modify, and enhance these projects for your learning journey.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
