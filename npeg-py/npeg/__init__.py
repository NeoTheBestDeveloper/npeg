from pathlib import Path
from typing import NoReturn

from .bindings import ImgResultType, ImgType, c_npeg
from .exceptions import ImgTypeIsNotSupportedYet, UnknownImgTypeError
from .pbm_img import PbmImg


def img_open(path: str | Path) -> PbmImg | NoReturn:
    img_result = c_npeg.img_read(Path(path).expanduser().as_posix().encode())
    img = c_npeg.cast_void_to_img(img_result.data)

    if img_result.type == ImgResultType.FILE_DOES_NOT_EXISTS:
        raise FileNotFoundError(f"No such file {path}")
    elif img_result.type == ImgResultType.DO_NOT_HAVE_READ_PERMISSIONS:
        raise PermissionError(f"Permission denied {path}")
    elif img_result.type == ImgResultType.IMG_TYPE_IS_NOT_SUPPORTED_YET:
        raise ImgTypeIsNotSupportedYet()
    elif img_result.type == ImgResultType.UNKNOWN_IMG_TYPE:
        raise UnknownImgTypeError()

    if img.contents.type == ImgType.PBM.value:
        return PbmImg(img)


__all__ = [
    "img_open",
]
