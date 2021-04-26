import os
import sys


def main() -> bool:
    if not os.path.isdir("build"):
        os.system("mkdir build")
    success = os.system("cd build && cmake .. -DCMAKE_C_COMPILER=clang && make && ./static_tests") == 0
    return success


if __name__ == "__main__":
    test_status = main()

    if test_status:
        sys.exit(0)
    sys.exit(1)
