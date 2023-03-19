from ctypes import CDLL, POINTER, c_char_p, c_float, c_int, c_void_p
from pathlib import Path


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
    "c_npeg",
)


LIB_PATH = Path(__file__).parent.joinpath("_c_npeg.so")


def load_lib() -> CDLL:
    c_npeg = CDLL(LIB_PATH)

    c_npeg.cast_void_to_img.restype = POINTER(ImgStruct)
    c_npeg.cast_void_to_img.argtypes = (c_void_p,)

    c_npeg.img_read.restype = ImgResultStruct
    c_npeg.img_read.argtypes = (c_char_p,)

    c_npeg.img_write.restype = ImgResultStruct
    c_npeg.img_write.argtypes = (POINTER(ImgStruct), c_char_p)

    c_npeg.img_copy.restype = POINTER(ImgStruct)
    c_npeg.img_copy.argtypes = (POINTER(ImgStruct),)

    c_npeg.img_free.argtypes = (POINTER(ImgStruct),)

    c_npeg.img_rotate.argtypes = (POINTER(ImgStruct), c_float, c_int)

    c_npeg.cast_img_to_pbm.restype = POINTER(PbmImgStruct)
    c_npeg.cast_img_to_pbm.argtypes = (POINTER(ImgStruct),)

    return c_npeg


c_npeg = load_lib()
