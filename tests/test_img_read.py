from os import chmod, remove

import pytest

from npeg import img_read
from npeg.exceptions import UnknownImgTypeError

IMGS_PATH = "tests/imgs"


@pytest.fixture
def unreadable_file():
    path = f"{IMGS_PATH}/unreadable_file"
    with open(path, "x") as file:
        chmod(path, 0000)
        yield path
        file.close()
        remove(path)


def test_successful_img_read():
    img = img_read(f"{IMGS_PATH}/ascii/small_sample1.pbm")
    img.close()


def test_file_does_not_exists():
    with pytest.raises(FileNotFoundError):
        img_read("some_wrong_path.img")


def test_img_type_is_not_supported_yet():
    with pytest.raises(NotImplementedError):
        img_read(f"{IMGS_PATH}/raw/small_sample1.png")


def test_unknowm_img_type():
    with pytest.raises(UnknownImgTypeError):
        img_read(f"{IMGS_PATH}/some.img")


def test_permission_error(unreadable_file: str):
    with pytest.raises(PermissionError):
        img_read(unreadable_file)
