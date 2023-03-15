dev:
	meson compile -j 8 -C build 

release:
	meson setup --reconfigure --buildtype=release  build
	meson compile -j 8 -C build 

clean:
	rm -rf build .cache

setup:
	meson setup build

resetup:
	rm -rf build .cache 
	meson setup build
