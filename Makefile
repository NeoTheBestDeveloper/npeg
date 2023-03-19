dev:
	meson compile -j 8 -C build 

setup:
	meson setup build

setup_release:
	meson setup --buildtype=release release_build -Db_sanitize=none

resetup_release:
	rm -rf release_build .cache 
	meson setup --buildtype=release release_build -Db_sanitize=none

release:
	meson compile -j 8 -C release_build
	cp ./release_build/libnpeg.so ./npeg-py/npeg/bindings/_c_npeg.so
	python3 -m build ./npeg-py

clean:
	rm -rf build .cache release_build


resetup:
	rm -rf build .cache 
	meson setup build

