from npeg import img_read
from . import IMGS_PATH


def test_pbm_rotation():
    with img_read(f"{IMGS_PATH}/ascii/small_sample1_rotated.pbm") as standart:
        with img_read(f"{IMGS_PATH}/ascii/small_sample1.pbm") as img:
            img.rotate(45)
            assert img.channels[0] == standart.channels[0]
