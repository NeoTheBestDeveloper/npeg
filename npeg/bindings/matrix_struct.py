from enum import IntEnum
from ctypes import Structure, c_int, c_int64, c_void_p

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
