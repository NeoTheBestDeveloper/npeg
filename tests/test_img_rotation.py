import cProfile

from npeg import img_read
from . import IMGS_PATH


def test_pbm_rotation():
    with cProfile.Profile() as pr:
        with img_read("assets/ascii_pbm/big_sample1.pbm") as img:
            img.rotate(45)
            img.write("res.pbm")
            pr.print_stats(1)
