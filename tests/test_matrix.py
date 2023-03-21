from npeg import img_read
from . import IMGS_PATH


def test_matrix_reading():
    with img_read(f"{IMGS_PATH}/ascii/small_sample1.pbm") as img:
        assert img.channels[0].height == 427
        assert img.channels[0].width == 640
        assert img.channels[0].shape == (640, 427)


def test_matrix_eq():
    with img_read(f"{IMGS_PATH}/ascii/small_sample1.pbm") as img1:
        with img_read(f"{IMGS_PATH}/ascii/small_sample1.pbm") as img2:
            assert img1.channels[0] == img2.channels[0]
