# Interesting Animation OpenGL

A modular OpenGL animation project with a clean structure designed for team collaboration.

## Project Structure

```
interesting_animation_opengl/
├── Makefile                # Build configuration
├── README.md               # This file
├── .gitignore              # Git ignore rules
├── include/                # Header files
│   └── window/
│       └── Window.hpp      # Window management class
└── src/                    # Source files
    ├── main.cpp            # Application entry point
    └── window/
        └── Window.cpp      # Window implementation
```

## Dependencies

- **OpenGL** (3.3+)
- **GLFW3** - Window and input management
- **GLEW** - OpenGL extension loading
- **Make** - Build system
- **GCC/G++** - C++ compiler

## Building

### Linux

#### Install Dependencies

**Arch Linux / Manjaro:**
```bash
sudo pacman -S glfw glew base-devel libx11
```

**Ubuntu / Debian:**
```bash
sudo apt-get update
sudo apt-get install libglfw3-dev libglew-dev build-essential libx11-dev
```

**Fedora:**
```bash
sudo dnf install glfw-devel glew-devel gcc-c++ libX11-devel
```

#### Build and Run

```bash
# Build and run in one command
make run

# Or build separately
make

# Then run
./build/InterestingAnimationOpenGL

# Clean build files
make clean
```

## Features

- Modular architecture with clear separation of concerns
- Window management abstraction
- Easy to extend with new modules
- Cross-platform support (Linux, macOS, Windows)

## Adding New Modules

To add a new module:

1. Create header in `include/yourmodule/YourClass.hpp`
2. Create implementation in `src/yourmodule/YourClass.cpp`
3. The Makefile automatically detects new `.cpp` files in the `src/` directory

## License

[Add your license here]

