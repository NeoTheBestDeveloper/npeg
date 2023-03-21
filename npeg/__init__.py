from pathlib import Path
from typing import NoReturn

from .bindings import ImgResultType, ImgType, c_npeg, Interpolation
from .exceptions import ImgTypeIsNotSupportedYet, UnknownImgTypeError
from .pbm_img import PbmImg


def img_read(path: str | Path) -> PbmImg | NoReturn:
    """
    Open and reading image, return one of img subclasses:
        - PbmImg (class for .pbm format, work in ascii and binary modes).
    """
    img_result = c_npeg.img_read(Path(path).expanduser().as_posix().encode())
    img = c_npeg.cast_void_to_img(img_result.data)

    if img_result.type == ImgResultType.FILE_DOES_NOT_EXISTS.value:
        raise FileNotFoundError(f"No such file {path}")
    if img_result.type == ImgResultType.DO_NOT_HAVE_READ_PERMISSIONS.value:
        raise PermissionError(f"Permission denied {path}")
    if img_result.type == ImgResultType.IMG_TYPE_IS_NOT_SUPPORTED_YET.value:
        raise ImgTypeIsNotSupportedYet()
    if img_result.type == ImgResultType.UNKNOWN_IMG_TYPE.value:
        raise UnknownImgTypeError()

    if img.contents.type == ImgType.PBM.value:
        return PbmImg(img)
    raise UnknownImgTypeError()


__all__ = [
    "img_read",
    "Interpolation",
]
