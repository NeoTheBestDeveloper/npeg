from ctypes import c_float, c_int
from pathlib import Path
from typing import NoReturn

from .matrix import Matrix
from .bindings import Interpolation, npeg_core, ImgType, ImgStruct, ImgResultStruct, ImgResultType

__all__ = ("BaseImg",)


class BaseImg:
    """Base class for img."""

    _img_struct: ImgStruct
    channels: list[Matrix]

    def __init__(self, img_struct: ImgStruct) -> None:
        self._img_struct = img_struct
        self.channels = [Matrix(img_struct.contents.channels[i]) for i in range(img_struct.contents.channels_count)]

    def __repr__(self) -> str:
        return (
            f"Img(max_colors={self.max_colors}, channels_count={self.channels_count}, type={self.type},"
            f" size={self.width}x{self.height})"
        )

    def __enter__(self):
        return self

    def __exit__(self, *_) -> None:
        npeg_core.img_free(self._img_struct)

    @property
    def max_colors(self) -> int:
        """Return max colors count=2^depth."""

        return 2**self._img_struct.contents.depth

    @property
    def depth(self) -> int:
        return self._img_struct.contents.depth

    @property
    def channels_count(self) -> int:
        return self._img_struct.contents.channels_count

    @property
    def type(self) -> ImgType:
        return ImgType(self._img_struct.contents.type)

    @property
    def width(self) -> int:
        return self._img_struct.contents.channels[0].width

    @property
    def height(self) -> int:
        return self._img_struct.contents.channels[0].height

    @property
    def shape(self) -> tuple[int, int]:
        """Return img shape like '(width, height)'."""

        return (self.width, self.height)

    def write(self, path: str | Path) -> None | NoReturn:
        img_result: ImgResultStruct = npeg_core.img_write(self._img_struct, Path(path).expanduser().as_posix().encode())
        if img_result.type == ImgResultType.DO_NOT_HAVE_WRITE_PERMISSIONS:
            raise PermissionError
        return None

    def rotate(self, degrees: float, inter: Interpolation | None = None) -> None:
        """
        Rotate img around the center, parts which will be out of range will not be at the img.
        Void places will be fillen with white color. Also work with negative degrees.

        Example (rotated by 65 degrees):

        |-------------------|                |-------------------|
        |g g g . . g g g g g|                |. . . . . g g g . .|
        |g g g . . g . . . g|                |. . . g g g g . . .|
        |g g g . . g . . . g|                |. g g g g g . . . .|
        |g g g . . g . . . g|         #      |g . g g g . . . g g|
        |g g g . . g . . . g|         ##     |g g g . . . g g . g|
        |g g g . . g . . . g|    ########    |. . . . . g . . . .|
        |g g g . . g . . . g|         ##     |. . . g . . . . . g|
        |g g g . . g . . . g|         #      |. . g g . . . g g .|
        |g g g . . g . . . g|                |. . . g . . g g . .|
        |g g g . . g g g g g|                |. . . . g g . . . .|
        |-------------------|                |-------------------|
        """

        npeg_core.img_rotate(
            self._img_struct, c_float(degrees), c_int(Interpolation.INTER_NONE if inter is None else inter)
        )

    def close(self) -> None:
        """Free allocated memory for image. Run automatically with python context manager."""

        npeg_core.img_free(self._img_struct)
