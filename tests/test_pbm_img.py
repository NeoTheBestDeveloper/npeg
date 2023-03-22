import pytest

from npeg import img_read
from npeg.bindings.img_struct import ImgType
from npeg.img import PbmImg
from . import IMGS_PATH


@pytest.fixture
def pbm_ascii_img():
    with img_read(f"{IMGS_PATH}/ascii/small_sample1.pbm") as img:
        yield img


def test_type_recognition(pbm_ascii_img: PbmImg):
    assert pbm_ascii_img.type == ImgType.PBM


def test_read_size(pbm_ascii_img: PbmImg):
    assert pbm_ascii_img.width == 640
    assert pbm_ascii_img.height == 427
    assert pbm_ascii_img.shape == (640, 427)


def test_read_depth(pbm_ascii_img: PbmImg):
    assert pbm_ascii_img.depth == 1
    assert pbm_ascii_img.max_colors == 2


def test_read_channels_count(pbm_ascii_img: PbmImg):
    assert pbm_ascii_img.channels_count == 1


def test_is_ascii(pbm_ascii_img: PbmImg):
    assert pbm_ascii_img.is_ascii is True
