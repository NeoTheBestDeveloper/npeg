from ctypes import c_float, c_int
from pathlib import Path

from .bindings import (
    Interpolation,
    c_npeg,
    MatrixStruct,
    ImgType,
    ImgStruct,
)


class Img:
    """Base class for img."""

    _img_struct: ImgStruct
    channels: MatrixStruct

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        c_npeg.img_free(self._img_struct)

    @property
    def max_colors(self) -> int:
        """Return max colors count (2**depth)"""
        return 2**self._img_struct.contents.depth

    @property
    def depth(self) -> int:
        return self._img_struct.contents.depth

    @property
    def channels_count(self) -> int:
        return self._img_struct.contents.channels_count

    @property
    def type(self) -> ImgType:
        return ImgType(self._img_struct.contents.type)

    @property
    def width(self) -> int:
        return self._img_struct.contents.channels[0].width

    @property
    def height(self) -> int:
        return self._img_struct.contents.channels[0].height

    def __init__(self, img_struct: ImgStruct) -> None:
        self._img_struct = img_struct

    def __str__(self) -> str:
        return (
            f"Img(max_colors={self.max_colors}, channels_count={self.channels_count}, type={self.type},"
            f" size={self.width}x{self.height})"
        )

    def write(self, path: str | Path) -> None:
        c_npeg.img_write(self._img_struct, Path(path).expanduser().as_posix().encode())

    def rotate(self, degrees: float, inter: Interpolation = Interpolation.INTER_NONE) -> None:
        c_npeg.img_rotate(self._img_struct, c_float(degrees), c_int(inter.value))
