import importlib.metadata

NPEG_CORE_VERSION = "0.0.1"


def get_npeg_version():
    __version__ = importlib.metadata.version("npeg")
    print(__version__)


def get_npeg_core_version():
    print(NPEG_CORE_VERSION)
