# One Click Tube - YouTube Video Downloader

## Overview
**One Click Tube** is a simple and efficient YouTube video downloader built using **C++, ImGui, OpenGL, and GLFW**. It provides a lightweight GUI that allows users to paste a YouTube video link, select a save directory, and download the video with a single click.

## Features
- **Minimalist UI** using ImGui.
- **GLFW and OpenGL Integration** for rendering.
- **Custom Save Directory** option.
- **Download in Best Quality** using `yt-dlp`.
- **Close Button** to exit the application easily.

## How It Works
1. Launch the application.
2. Paste the YouTube video URL into the input field.
3. Choose a directory to save the downloaded video.
4. Click the **Download** button to start downloading.
5. The application runs a system command using `yt-dlp` to fetch and save the video.

## Installation & Requirements
### Prerequisites:
- **C++ Compiler** (GCC/Clang/MSVC)
- **CMake** (for building the project)
- **GLFW** (for window handling)
- **GLAD** (for OpenGL function loading)
- **ImGui** (for UI rendering)
- **yt-dlp** (for downloading videos) - Ensure it's installed and available in the system path.

### Steps to Build & Run:
```sh
# Clone the repository
git clone https://github.com/yourusername/OneClickTube.git
cd OneClickTube

# Build the project
mkdir build && cd build
cmake ..
make

# Run the application
./OneClickTube
```

## Future Plans & Improvements
✅ **Enhance UI:**
- Add progress bar while downloading.
- Improve styling with custom themes.
- Add drag-and-drop support for YouTube links.

✅ **Feature Additions:**
- Support for multiple downloads at once.
- Option to select video/audio format before downloading.
- Integrate a settings menu to save default preferences.

✅ **Performance Optimizations:**
- Optimize rendering to reduce resource usage.
- Improve system command execution for better stability.

## Contributions
Contributions are welcome! If you’d like to improve the project, feel free to submit a pull request or report issues in the repository.

