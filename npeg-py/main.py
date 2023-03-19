from img import image_open, PostProcess

img = image_open("../assets/ascii_pbm/big_sample1.pbm")
img.rotate(10, PostProcess.NONE_PROCESS)
img.save("python.pbm")
