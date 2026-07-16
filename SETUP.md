# Development Environment Setup (Windows)

## Prerequisites

This project uses the MinGW-w64 toolchain via MSYS2 for compilation on Windows. Follow these steps to set up the environment on a new machine.

## Installation Steps

### 1. Install MSYS2

Download and install MSYS2 from [msys2.org](https://www.msys2.org). When choosing an install path, use the default `C:\msys64` — the project's VS Code configuration files reference this path directly and will work without modification if you use it.

### 2. Open the correct terminal

After installation, open **MSYS2 MinGW x64** from the Start menu. This is important — do not use the "MSYS2 MSYS" or "MSYS2 UCRT64" variants, as those use different compiler environments that won't match the project's configuration.

### 3. Update the package database

Run the following command. You may be prompted to close and reopen the terminal partway through — do so, then run it again:

```bash
pacman -Syu
pacman -Su
```

### 4. Install the compiler toolchain

```bash
pacman -S mingw-w64-x86_64-toolchain
```

When prompted, install all members of the toolchain. This provides `g++`, `gcc`, `gdb`, `make`, `mingw32-make`, and related tools.

### 5. Install SDL2

```bash
pacman -S mingw-w64-x86_64-SDL2
```

### 6. Install GLM

```bash
pacman -S mingw-w64-x86_64-glm
```

### 7. Add MinGW to your Windows PATH

So that `g++` and `mingw32-make` are accessible from any terminal (including VS Code's integrated terminal):

1. Open Windows Settings and search for **"Environment Variables"**
2. Click **"Edit the system environment variables"**
3. Under **System variables**, select `Path` and click **Edit**
4. Add a new entry: `C:\msys64\mingw64\bin`
5. Click OK on all dialogs

Restart any open terminals or VS Code after this step for the change to take effect.

### 8. Install VS Code

Download from [code.visualstudio.com](https://code.visualstudio.com). Once installed, add the **C/C++** extension by Microsoft (search `ms-vscode.cpptools` in the Extensions panel).

## Building the Project

The project includes a `Makefile`. To build, open a terminal in the project root and run:

```bash
mingw32-make
```

To run after building:

```bash
./app.exe
```

## Notes

- The `.vscode` configuration files (`c_cpp_properties.json`, `launch.json`) reference `C:/msys64/mingw64/...` directly. As long as MSYS2 is installed at the default `C:\msys64` path, these require no modification on a new machine.
- GLAD is included in the project source (`src/glad.c`, `include/glad/`) and requires no separate installation.
- OpenGL (`-lopengl32`) is provided by Windows itself and requires no separate installation.
