# Npeg it is a small and blazingly fast python library for image processing.

# Warning: project is unstable now.

## Supported formats

- PNG (In the future)
- JPG (In the future)
- WEBP (In the future)
- TIFF (In the future)
- PBM
- PGM (In the future)
- PPM (In the future)
- PPA (In the future)

## Examples

```python 
from npeg import img_read, Interpolation

# img rotation.
with img_read('cats.pbm') as img:
    img.rotate(10)
    img.rotate(-10, Interpolation.INTER_LINEAR)
    img.save('rotated_cat.pbm')

# Or you can read file manually.

img = img_read('dogs.pbm')
img.save('dods_cipy.bpm') # Kind of img copying.
img.close()
```

## Installation

```bash
$ pip install npeg
```

Or manually

Dependencies:
- poetry 
- meson
```bash
$ git clone https://github.com/neothebestdeveloper/npeg
$ poetry run build_pkg
$ pip install "dist/npeg-0.0.1-py3-none-any.whl"
```
