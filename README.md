# TRUtor

A C++ tutoring platform console app for managing interactions between students and tutors.

## Installation

**macOS:**
```bash
brew install cmake
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get install cmake build-essential
```

**Windows:**
- Download CMake from https://cmake.org/download/
- Download Visual Studio or MinGW for C++ compiler

## Compilation Instructions

#### 1. Clone the Repository

```bash
git clone https://github.com/kongkham-lk/TRUtor.git
cd TRUtor
```

#### 2. Create Build Directory

```bash
mkdir build
cd build
```

#### 3. Generate Build Files with CMake

```bash
cmake ..
```

#### 4. Compile and Run the Project

You have two options to build the project: using CMake (recommended) or a single g++ command (quick build).

**Option A — CMake (recommended)**

<ins>*Compile the Project:*</ins>

```bash
# macOS / Linux
cmake --build .

# Or use make directly (if available)
make
```

<ins>*Run the Application:*</ins>

```bash
# If you used CMake (default target name `TRUtor`)
./TRUtor

# Windows (from CMake build directory)
./Debug/TRUtor.exe
# or
./Release/TRUtor.exe
```

**Option B — Single-command build (Bash/zsh)**

<ins>*Compile the Project:*</ins>

```bash
g++ -std=c++11 $(find . -name "*.cpp" ! -path "*/CMakeFiles/*" ! -path "*/cmake-build-*/*") -o main
```
This finds all `.cpp` files recursively and compiles them into a single executable named `main`. It excludes generated CMake files and common build directories.

<ins>*Run the Application:*</ins>

```bash
# If you used the single-command g++ build (executable `main`)
./main
```

## Features Implemented

| Structure | Description |
|---------|-------------|
| `Model Classes` | Organized data models representing core entities (User, Student, Tutor) with clear separation of concerns and object-oriented design |
| `Service Classes` | Business logic and service layer handling operations like user management, session booking, and messaging functionality |
| `IUser Interface` | Base class with role-based hierarchy, storing user credentials (email, password), profile information (name, ID), and creation timestamps |

| Feature | Description |
|---------|-------------|
| `User Class` | Organized data models representing core entities (User, Student, Tutor) with clear separation of concerns and object-oriented design |
| `Student Class` | Derived from User class representing student users with student-specific properties and role identifier (role = 0) |
| `Tutor Class` | Derived from User class representing tutor users with tutor-specific properties and role identifier (role = 1) |
| `Role Identification` | System to distinguish between students (role 0) and tutors (role 1), enabling role-based access control and differentiated functionality |
| `User Profile` | Capture and manage user information including name, email, password, and account creation timestamps |
| `User Authentication` | Email and password-based authentication system for user verification and security |
| `Login Session` | Session tracking for authenticated users, maintaining login state and user context during application runtime |
| `Course Booking` | Manage and track tutoring sessions/appointments booked between students and tutors, including session scheduling and availability |
| `Message System` | Inter-user messaging system enabling students and tutors to communicate, send queries, and exchange information |
