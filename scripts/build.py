from os import system

CC = "clang"


def build_dev_lib() -> None:
    """Build dev core library and copy shared object file into python npeg catalog."""
    system(f"CC={CC} meson setup --buildtype=debug debug_build")
    system(f"CC={CC} meson compile -j 8 -C debug_build")
    system("cp debug_build/libnpeg-core.so npeg/bindings/_npeg_core.so")

    system(f"CC={CC} meson setup --cross-file=x86_64-w64-mingw32.txt --buildtype=debug debug_win_build")
    system(f"CC={CC} meson compile -j 8 -C debug_win_build")
    system("cp debug_win_build/libnpeg-core.dll npeg/bindings/_npeg_core.dll")


def build_release_lib() -> None:
    """Build release core library and copy shared object file into python npeg catalog."""
    system(f"CC={CC} meson setup --buildtype=release release_build -Db_sanitize=none")
    system(f"CC={CC} meson compile -j 8 -C release_build")
    system("strip release_build/libnpeg-core.so")
    system("cp release_build/libnpeg-core.so npeg/bindings/_npeg_core.so")

    system(
        f"CC={CC} meson setup --cross-file=x86_64-w64-mingw32.txt"
        "--buildtype=release release_win_build -Db_sanitize=none"
    )
    system(f"CC={CC} meson compile -j 8 -C release_win_build")
    system("/usr/bin/x86_64-w64-mingw32-strip release_win_build/libnpeg-core.dll")
    system("cp release_win_build/libnpeg-core.dll npeg/bindings/_npeg_core.dll")


def build_pkg() -> None:
    """Build python package."""
    build_release_lib()
    system("poetry build")


def clean() -> None:
    system("rm -rf release_build debug_build")
