from ctypes import (
    POINTER,
    Structure,
    c_float,
    c_uint16,
    c_uint8,
    c_int,
    c_char_p,
    c_bool,
)
from pathlib import Path
from enum import Enum

from img_format import ImageFormat
from matrix import Matrix
from clib import c_npeg


class ImgStructure(Structure):
    _fields_ = [
        ("channels", POINTER(Matrix)),
        ("max_colors", c_uint16),
        ("channels_count", c_uint8),
        ("format", c_int),
    ]

    def __str__(self) -> str:
        return f"Img(channels={self.channels[0]}, max_colors={self.max_colors}, channels_count={self.channels_count}, format={ImageFormat(self.format)})"


class PbmImgStructure(Structure):
    _fields_ = [
        ("img", ImgStructure),
        ("is_ascii", c_bool),
    ]

    def __str__(self) -> str:
        return f"Img(img={self.img} is_ascii={self.is_ascii})"


class PostProcess(Enum):
    NONE_PROCESS = 0
    BILINEAR_INTER = 1
    LANCZOS_INTER = 2
    AVG_INTER = 3
    UP_DOWN_SCALE = 4


c_img_open = c_npeg.img_open
c_img_open.restype = POINTER(ImgStructure)
c_img_open.argtypes = [c_char_p]


c_img_save = c_npeg.img_save
c_img_save.argtypes = [POINTER(ImgStructure), c_char_p]

c_img_rotate = c_npeg.img_rotate
c_img_rotate.argtypes = [POINTER(ImgStructure), c_float, c_int]

c_img_free = c_npeg.img_free
c_img_free.argtypes = [POINTER(ImgStructure)]

c_cast_img_to_pbm = c_npeg.cast_img_to_pbm
c_cast_img_to_pbm.argtypes = [POINTER(ImgStructure)]
c_cast_img_to_pbm.restype = POINTER(PbmImgStructure)


class Img:
    _img_struct: ImgStructure
    channels: Matrix
    max_colors: int
    channels_count: int
    image_format: ImageFormat

    @property
    def max_colors(self) -> int:
        return self.img_struct.contents.max_colors

    @property
    def channels_count(self) -> int:
        return self.img_struct.contents.channels_count

    @property
    def image_format(self) -> ImageFormat:
        return ImageFormat(self.img_struct.contents.format)

    @property
    def width(self) -> int:
        return self.img_struct.contents.channels[0].width

    @property
    def height(self) -> int:
        return self.img_struct.contents.channels[0].height

    def __init__(self, img_struct: ImgStructure) -> None:
        self.img_struct = img_struct

    def __del__(self) -> None:
        c_img_free(self.img_struct)

    def __str__(self) -> str:
        return f"Img(max_colors={self.max_colors}, channels_count={self.channels_count}, format={self.image_format}, size={self.width}x{self.height})"

    def save(self, path: str | Path) -> None:
        c_img_save(self.img_struct, path.encode())

    def rotate(self, degrees: float, post_process: PostProcess) -> None:
        c_img_rotate(self.img_struct, c_float(degrees), c_int(post_process.value))


class PbmImg(Img):
    is_ascii: bool
    _children_img_struct: POINTER(PbmImgStructure)

    @property
    def is_ascii(self) -> bool:
        return self.children_img_struct.contents.is_ascii

    def __init__(self, img_struct: ImgStructure) -> None:
        self.img_struct = img_struct
        super().__init__(img_struct)
        self.children_img_struct = c_cast_img_to_pbm(img_struct)

    def __str__(self) -> str:
        return f"PbmImg(max_colors={self.max_colors}, channels_count={self.channels_count}, format={self.image_format}, size={self.width}x{self.height}, is_ascii={self.is_ascii})"


def image_open(path: str | Path) -> Img:
    img_struct = c_img_open(path.encode())

    if img_struct.contents.format == ImageFormat.PBM.value:
        return PbmImg(img_struct)
