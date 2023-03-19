from enum import Enum

__all__ = ("Interpolation",)


class Interpolation(Enum):
    INTER_NONE = 0
    INTER_LINIAR = 1
    INTER_AVG = 2
    """ INTER_LANCZOS = 2 """
    """ INTER_CUBIC = 3 """
