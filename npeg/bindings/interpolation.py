from enum import IntEnum

__all__ = ("Interpolation",)


# TODO: write tests.
class Interpolation(IntEnum):
    """WRANING: it may be buggy now."""

    INTER_NONE = 0
    INTER_LINIAR = 1
    INTER_AVG = 2
    # INTER_LANCZOS = 2
    # INTER_CUBIC = 3

    def __repr__(self) -> str:
        cls_name = self.__class__.__name__
        return f"{cls_name}.{self.name}"
