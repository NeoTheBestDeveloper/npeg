from enum import IntEnum
from ctypes import Structure, c_int, c_void_p

__all__ = (
    "ImgResultType",
    "ImgResultStruct",
)


class ImgResultType(IntEnum):
    IMG_OK = 0
    FILE_DOES_NOT_EXISTS = 1
    DO_NOT_HAVE_WRITE_PERMISSIONS = 2
    DO_NOT_HAVE_READ_PERMISSIONS = 3
    UNKNOWN_IMG_TYPE = 4
    IMG_TYPE_IS_NOT_SUPPORTED_YET = 5

    def __repr__(self) -> str:
        cls_name = self.__class__.__name__
        return f"{cls_name}.{self.name}"


class ImgResultStruct(Structure):
    _fields_ = [
        ("data", c_void_p),
        ("type", c_int),
    ]
