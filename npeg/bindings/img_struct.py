from enum import IntEnum
from ctypes import Structure, POINTER, c_char_p, c_int, c_uint8

from . import MatrixStruct

__all__ = (
    "ImgType",
    "ImgStruct",
)


class ImgType(IntEnum):
    UNKNOWN_MAGIC = -1
    PNG = 0
    JPG = 1
    PAM = 2
    PBM = 3
    PGM = 4
    PPM = 5
    WEBP = 6
    TIFF = 7

    def __repr__(self) -> str:
        cls_name = self.__class__.__name__
        return f"{cls_name}.{self.name}"


class ImgStruct(Structure):
    _fields_ = [
        ("channels", POINTER(MatrixStruct)),
        ("path", c_char_p),
        ("type", c_int),
        ("depth", c_uint8),
        ("channels_count", c_uint8),
    ]

    def __repr__(self) -> str:
        return (
            f"Img(channels={self.channels[0]}, depth={self.depth}, channels_count={self.channels_count},"
            f" format={ImgType(self.type)})"
        )
