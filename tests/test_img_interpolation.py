import cProfile
from npeg import img_read, Interpolation

from . import IMGS_PATH


def test_interpolation():
    """ with img_read(f"{IMGS_PATH}/ascii/small_sample1.pbm") as img: """
    """     img.rotate(45, Interpolation.INTER_NONE) """
    """     img.write('re1.pbm') """
    """"""
    """ with img_read(f"{IMGS_PATH}/ascii/small_sample1.pbm") as img: """
    """     img.rotate(10, Interpolation.INTER_NONE) """
    """     img.write('re2.pbm') """
    """"""
    """ with img_read(f"{IMGS_PATH}/ascii/small_sample1.pbm") as img: """
    """     img.rotate(120, Interpolation.INTER_NONE) """
    """     img.write('re3.pbm') """

    with img_read(f"{IMGS_PATH}/ascii/small_sample1.pbm") as img:
        """ with cProfile.Profile() as pr: """
        """     img.rotate(0, Interpolation.INTER_NONE) """
        """     pr.print_stats() """
        img.write('re4_avx.pbm')
