# CMake Build Instructions

This project now supports building with CMake as an alternative to the Visual Studio solution files.

## Prerequisites

- CMake 4 or higher and - preferably - Ninja (use `winget` to install)
- A C/C++ compiler (MSVC, GCC, or Clang),
- `clang-tidy` and `clang-format` tools,
- Internet connection (for downloading the Directory Opus SDK on first build)

## Building on Windows with Visual Studio

- Strongly recommend using `CMake` support,
- Strongly recommend using `Clang` complier over MSVC,
- Rely on tasks to deploy plugin to DOpus (assumes installation on drive `C:\`)
- Debugging fully supported.

## Project Structure

- **src**: Main DLL/shared library (the Directory Opus plugin)
- **external**: Dependencies

## Troubleshooting

### SDK Download Fails

If the automatic SDK download fails, you can manually:
1. Download `opus_sdk.zip` from https://cdn2.gpsoft.com.au/files/Misc/opus_sdk.zip
2. Place it in `external/opus_sdk.zip`
3. Extract it to `external/dopus_sdk/`

### Build Errors

- Ensure all submodules are properly initialized if using git
- Verify CMake version is 4 or higher: `cmake --version`

## Comparison with Visual Studio Build

The CMake build replicates the Visual Studio project configuration including:
- All source files and headers
- Include directories
- Preprocessor definitions
- Compiler flags and optimizations
- Library dependencies
- Debug/Release configurations
