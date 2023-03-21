from types import NotImplementedType

from .bindings import MatrixStruct


# TODO: add slices for reading
# TODO: add setitem by index
# TODO: add get-item by index
# TODO: conversion into list, tuple, array.
# TODO: conversion into ndarray as optional library feature.


class Matrix:
    _matrix_struct: MatrixStruct

    def __init__(self, matrix_struct: MatrixStruct) -> None:
        self._matrix_struct = matrix_struct

    def __eq__(self, other: object) -> bool | NotImplementedType:
        if not isinstance(other, Matrix):
            return NotImplemented
        return self._matrix_struct == other._matrix_struct

    @property
    def width(self) -> int:
        return self._matrix_struct.width

    @property
    def height(self) -> int:
        return self._matrix_struct.height

    @property
    def shape(self) -> tuple[int, int]:
        return (self.width, self.height)
