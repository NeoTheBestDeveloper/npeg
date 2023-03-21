from pathlib import Path
from typing import NoReturn


from .pbm_img import PbmImg
from .exceptions import UnknownImgTypeError
from .bindings import ImgResultType, ImgType, npeg_core, Interpolation, ImgStruct, ImgResultStruct

__all__ = (
    "img_read",
    "Interpolation",
)


def _cast_img(img_struct: ImgStruct) -> PbmImg | NoReturn:
    """Depending on img type return corresponding img subclass."""
    match img_struct.contents.type:
        case ImgType.PBM:
            return PbmImg(img_struct)
        case _:
            raise UnknownImgTypeError()


def img_read(path: str | Path) -> PbmImg | NoReturn:
    """
    Open and reading image, return one of img subclasses:
        - PbmImg (class for .pbm format, work in ascii and binary modes).
    """

    path = Path(path).expanduser().as_posix()
    img_result: ImgResultStruct = npeg_core.img_read(path.encode())
    img: ImgStruct = npeg_core.cast_void_to_img(img_result.data)

    match img_result.type:
        case ImgResultType.FILE_DOES_NOT_EXISTS:
            raise FileNotFoundError(f"No such file '{path}'.")

        case ImgResultType.DO_NOT_HAVE_READ_PERMISSIONS:
            raise PermissionError(f"Permission denied '{path}'.")

        case ImgResultType.IMG_TYPE_IS_NOT_SUPPORTED_YET:
            raise NotImplementedError("Image type for recognized but havr not been implemented yet.")

        case ImgResultType.UNKNOWN_IMG_TYPE:
            raise UnknownImgTypeError(f"Cannot recognize img type for file '{path}'.")

        case _:
            return _cast_img(img)
