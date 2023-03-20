from os import system

CC = "clang"


def build_dev_lib() -> None:
    """Build dev core library and copy shared object file into python npeg catalog."""
    system(f"CC={CC} meson setup --buildtype=debug debug_build")
    system(f"CC={CC} meson compile -j 8 -C debug_build")
    system("cp debug_build/libnpeg.so npeg/bindings/_c_npeg.so")


def build_release_lib() -> None:
    """Build release core library and copy shared object file into python npeg catalog."""
    system(f"CC={CC} meson setup --buildtype=release release_build -Db_sanitize=none")
    system(f"CC={CC} meson compile -j 8 -C release_build")
    system("strip release_build/libnpeg.so")
    system("cp release_build/libnpeg.so npeg/bindings/_c_npeg.so")


def build_pkg() -> None:
    """Build python package."""
    build_release_lib()
    system("poetry build")


def clean() -> None:
    system("rm -rf release_build build_dev")
