# Clarity - Image Processing Pseudo-Application

Clarity is an image processing application developed using the OpenCV library. It supports various image processing functionalities, including Gaussian blur, grayscale conversion, and brightness adjustment. The project integrates OpenCV C++ API and is designed for Linux environments.

---

## Setup and Installation

### Prerequisites
Ensure the following tools are installed on your system:
- **C++ Compiler** (e.g., `g++`)
- **Git** (for cloning repositories)
- **CMake** (for building OpenCV)
- **Make** (to compile the build system)

### Steps to Install OpenCV on Linux

1. **Install Required Packages**:
   Use the following command to install essential dependencies:
   ```bash
   sudo apt update && sudo apt install -y build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
   ```

2. **Clone the OpenCV Repository**:
   ```bash
   git clone https://github.com/opencv/opencv.git
   ```

3. **Build and Install OpenCV**:

   - Navigate to the OpenCV directory:
     ```bash
     cd opencv
     ```
   - Create a build directory:
     ```bash
     mkdir build && cd build
     ```
   - Generate the build configuration:
     ```bash
     cmake ..
     ```
   - Compile the library:
     ```bash
     make -j$(nproc)
     ```
   - Install OpenCV:
     ```bash
     sudo make install
     ```

4. **Verify Installation**:
   Test if OpenCV is installed correctly by checking the version:
   ```bash
   pkg-config --modversion opencv4
   ```

---

## Running the Application

### Compiling the Code
To compile the source code, use the following command:
```bash
g++ -o clarity main.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc
```

### Running the Application
Run the compiled application with an image path as input:
```bash
./clarity <image_path>
