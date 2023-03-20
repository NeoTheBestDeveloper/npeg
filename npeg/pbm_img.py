from .bindings import PbmImgStruct, c_npeg, ImgStruct
from .img import Img


class PbmImg(Img):
    _children_img_struct: PbmImgStruct

    @property
    def is_ascii(self) -> bool:
        return self._children_img_struct.contents.is_ascii

    def __init__(self, img_struct: ImgStruct) -> None:
        super().__init__(img_struct)
        self._children_img_struct = c_npeg.cast_img_to_pbm(img_struct)

    def __str__(self) -> str:
        return (
            f"PbmImg(max_colors={self.max_colors}, channels_count={self.channels_count}, format={self.type},"
            f" size={self.width}x{self.height}, is_ascii={self.is_ascii})"
        )
