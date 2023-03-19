from ctypes import cdll

__all__ = ("cnpeg",)

c_npeg = cdll.LoadLibrary("../build/libnpeg.so")
