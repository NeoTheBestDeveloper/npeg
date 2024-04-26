from sys import platform
from pathlib import Path, WindowsPath
from ctypes import CDLL, POINTER, c_bool, c_char_p, c_float, c_int, c_void_p

from .matrix_struct import MatrixStruct, MatrixType
from .img_result_struct import ImgResultStruct, ImgResultType
from .img_struct import ImgType, ImgStruct
from .pbm_img_struct import PbmImgStruct
from .interpolation import Interpolation

__all__ = (
    "MatrixType",
    "MatrixStruct",
    "ImgResultStruct",
    "ImgResultType",
    "ImgStruct",
    "PbmImgStruct",
    "ImgType",
    "Interpolation",
    "npeg_core",
)

if platform in ("linux", "linux2"):
    LIB_PATH = Path(__file__).parent.joinpath("_npeg_core.so")
elif platform == "win32":
    LIB_PATH = WindowsPath(__file__).parent.joinpath("_npeg_core.dll").resolve().expanduser()
    print(__file__)
    print(LIB_PATH.resolve().expanduser())
    print(WindowsPath(__file__))
    print(str(LIB_PATH))
else:
    raise OSError(f"This platform '{platform}' is not supported.")


def load_lib() -> CDLL:
    lib = CDLL(str(LIB_PATH))

    lib.cast_void_to_img.restype = POINTER(ImgStruct)
    lib.cast_void_to_img.argtypes = (c_void_p,)

    lib.img_read.restype = ImgResultStruct
    lib.img_read.argtypes = (c_char_p,)

    lib.img_write.restype = ImgResultStruct
    lib.img_write.argtypes = (POINTER(ImgStruct), c_char_p)

    lib.img_copy.restype = POINTER(ImgStruct)
    lib.img_copy.argtypes = (POINTER(ImgStruct),)

    lib.img_free.argtypes = (POINTER(ImgStruct),)

    lib.img_rotate.argtypes = (POINTER(ImgStruct), c_float, c_int)

    lib.cast_img_to_pbm.restype = POINTER(PbmImgStruct)
    lib.cast_img_to_pbm.argtypes = (POINTER(ImgStruct),)

    lib.matrix_cmp.restype = c_bool
    lib.matrix_cmp.argtypes = (POINTER(MatrixStruct), POINTER(MatrixStruct))

    lib.img_interpolate.argtypes = (POINTER(ImgStruct), c_int)
    return lib


npeg_core = load_lib()
