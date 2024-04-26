from enum import IntEnum
from ctypes import Structure, POINTER, c_char_p, c_int, c_uint8

from . import MatrixStruct

__all__ = (
    "ImgType",
    "ImgStruct",
)


class ImgType(IntEnum):
    UNKNOWN_MAGIC = 0
    PNG = 1
    JPG = 2
    PAM = 3
    PBM = 4
    PGM = 5
    PPM = 6
    WEBP = 7
    TIFF = 8

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
