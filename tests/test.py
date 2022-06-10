#!/usr/bin/python

from subprocess import run

ASSETS_DIR = "tests/assets"
OUT_DIR = "tests/out"

alghoritms = ('rotate90', 'rotate180', 'rotate270', 'gaussian_filter',
              'box_filter', 'median_filter', 'unsharp_filter',
              'horizontal_flip', 'vertical_flip')


def test(input_path: str, alghortim: str) -> None:
    result = run([
        "build/npeg", '-i', f'{ASSETS_DIR}/{input_path}', '-o',
        f'{OUT_DIR}/{input_path}_{alghortim}.out', '-a', alghortim
    ],
                 capture_output=True,
                 text=True)
    if result.returncode == 0:
        print(f"[SUCCESS] {alghortim}")
    else:
        print(f'[FAILED] {alghortim}', end='')
        if result.returncode == -11:
            print(' (segmentation fault)')
        else:
            print(f' ({result.stderr})')


print("### Start test pbm format ###")
print("###     RAW encoding      ###\n")

for alghortim in alghoritms:
    test('sample_640x426.pbm', alghortim)

print("\n###    ASCII encoding     ###\n")

for alghortim in alghoritms:
    test('fool.ascii.pbm', alghortim)

print("\n### Start test pgm format ###")
print("###     RAW encoding      ###\n")

for alghortim in alghoritms:
    test('sample_640×426.pgm', alghortim)

print("\n###    ASCII encoding     ###\n")

for alghortim in alghoritms:
    test('mona_lisa.ascii.pgm', alghortim)
