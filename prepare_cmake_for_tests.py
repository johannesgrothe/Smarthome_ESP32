
def line_must_be_gone(line: str) -> bool:
    check_line = line.upper()
    return check_line.startswith("SET(CMAKE_C_COMPILER") or \
           check_line.startswith("SET(CMAKE_CXX_COMPILER") or \
           check_line.startswith("SET(CMAKE_C_FLAGS") or \
           check_line.startswith("SET(CMAKE_CXX_FLAGS") or \
           check_line.startswith("SET(CMAKE_SYSTEM_NAME") or \
           check_line.startswith("SET(CMAKE_C_COMPILER_WORKS") or \
           check_line.startswith("SET(CMAKE_CXX_COMPILER_WORKS") or \
           check_line.startswith("SET(CMAKE_C_STANDARD") or \
           check_line.startswith("SET(CMAKE_CXX_STANDARD")


def change_lines(in_lines: list[str]) -> list[str]:
    out_lines = []

    for line in in_lines:
        if line_must_be_gone(line):
            out_lines.append("#" + line)
        else:
            out_lines.append(line)

    return out_lines


def prepare_cmake_lists_private():
    with open("CMakeListsPrivate.txt", "r") as file_h:
        in_lines = file_h.readlines()

    out_lines = change_lines(in_lines)

    with open("CMakeListsPrivate.txt", "w") as file_h:
        file_h.writelines(out_lines)


def prepare_cmake_lists():
    with open("CMakeLists.txt", "r") as file_h:
        in_lines = file_h.readlines()

    out_lines = change_lines(in_lines)

    with open("CMakeLists.txt", "w") as file_h:
        file_h.writelines(out_lines)


def main():
    prepare_cmake_lists()
    prepare_cmake_lists_private()


if __name__ == "__main__":
    main()
