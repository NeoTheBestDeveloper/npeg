import pytest

from npeg import img_read
from . import IMGS_PATH


def test_write_to_unwritable_file():
    with img_read(f"{IMGS_PATH}/ascii/small_sample1.pbm") as img:
        with pytest.raises(PermissionError):
            img.write(f"{IMGS_PATH}/unwritable_file")


def test_write_with_same_img_type():
    with img_read(f"{IMGS_PATH}/ascii/small_sample1.pbm") as src:
        src.write("res.pbm")

        with img_read("res.pbm") as dst:
            assert dst.channels_count == src.channels_count
            assert dst.height == src.height
            assert dst.width == src.width
            assert dst.max_colors == src.max_colors
            assert dst.depth == src.depth
            assert dst.type == src.type
            assert dst.shape == src.shape