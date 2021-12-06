import os
import sys


def line_must_be_gone(line: str) -> bool:
    if line == "":
        return False

    beginnings = ["SET(CMAKE_C_COMPILER",
                  "SET(CMAKE_CXX_COMPILER",
                  "SET(CMAKE_C_FLAGS",
                  "SET(CMAKE_CXX_FLAGS",
                  "SET(CMAKE_SYSTEM_NAME",
                  "SET(CMAKE_C_COMPILER_WORKS",
                  "SET(CMAKE_CXX_COMPILER_WORKS",
                  "SET(CMAKE_C_STANDARD",
                  "SET(CMAKE_CXX_STANDARD",
                  'SET(CMAKE_CONFIGURATION_TYPES "ESP32CAM"']

    for part in beginnings:
        if line.upper().startswith(part.upper()):
            return True

    return False


def change_lines(in_lines: list[str]) -> list[str]:
    out_lines = []

    for line in in_lines:
        if line_must_be_gone(line):
            out_lines.append("#" + line)
        else:
            out_lines.append(line)

    return out_lines


def prepare_cmake_file(filename: str):
    print(f"Manipulating file '{filename}'")
    with open(filename, "r") as file_h:
        in_lines = file_h.readlines()

    out_lines = change_lines(in_lines)

    with open(filename, "w") as file_h:
        file_h.writelines(out_lines)


def main():
    print("Initializing CMake files for test execution (CLion)")
    try:
        prepare_cmake_file("CMakeListsPrivate.txt")
        prepare_cmake_file("CMakeLists.txt")
    except FileNotFoundError:
        print("Error initiating one or more CMake Files. Maybe you need to (re-)init the PlatformIO project for CLion.")
        sys.exit(1)
    print("Initialization successful.")
    print("")

    cmake_dirs = [f for f in os.listdir('.') if os.path.isdir(f) and f.startswith("cmake-build")]

    print("You might have to delete the following directories if any errors occur:")
    for directory in cmake_dirs:
        print("  " + directory)


if __name__ == "__main__":
    main()
