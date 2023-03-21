from enum import IntEnum
from types import NotImplementedType
from ctypes import Structure, c_int, c_int64, c_uint16, c_uint8, c_void_p, POINTER, cast
from typing_extensions import Self

__all__ = (
    "MatrixType",
    "MatrixStruct",
)


class MatrixType(IntEnum):
    U8_MATRIX = 0
    U16_MATRIX = 1

    def __repr__(self) -> str:
        cls_name = self.__class__.__name__
        return f"{cls_name}.{self.name}"


class MatrixStruct(Structure):
    _fields_ = [
        ("height", c_int64),
        ("width", c_int64),
        ("data", c_void_p),
        ("matrix_type", c_int),
    ]

    def __repr__(self) -> str:
        return f"Matrix(height={self.height}, width={self.width}, matrix_type={MatrixType(self.matrix_type)})"

    def _eq_u8(self, other: Self) -> bool:
        self_data = cast(self.data, POINTER(c_uint8))
        other_data = cast(other.data, POINTER(c_uint8))

        for i in range(self.height * self.width):
            if self_data[i] != other_data[i]:
                return False
        return True

    def _eq_u16(self, other: Self) -> bool:
        self_data = cast(self.data, POINTER(c_uint16))
        other_data = cast(other.data, POINTER(c_uint16))

        for i in range(self.height * self.width):
            if self_data[i] != other_data[i]:
                return False
        return True

    # TODO: rewrite on C.
    def __eq__(self, other: object) -> bool | NotImplementedType:
        if not isinstance(other, MatrixStruct):
            return NotImplemented

        if self.matrix_type != other.matrix_type:
            return False

        if self.matrix_type == MatrixType.U8_MATRIX:
            return self._eq_u8(other)
        return self._eq_u16(other)
