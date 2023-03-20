from ctypes import Structure, c_bool

from . import ImgStruct

__all__ = ("PbmImgStruct",)


class PbmImgStruct(Structure):
    _fields_ = [
        ("img", ImgStruct),
        ("is_ascii", c_bool),
    ]

    def __str__(self) -> str:
        return f"Img(img={self.img} is_ascii={self.is_ascii})"
