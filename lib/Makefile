all: dependencies

dependencies: benchmark libcds googletest

libcds:
	git clone https://github.com/DamirJann/libcds.git
	mkdir libcds-release
	cd libcds-release && \
    cmake -DLIBCDS_WITH_TESTS=OFF -DCMAKE_BUILD_TYPE=RELEASE ../libcds  && \
    make -j4

benchmark:
	git clone https://github.com/google/benchmark
	cd benchmark && \
	cmake -E make_directory "build" && \
	cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ../ && \
	cmake --build "build" --config Release

googletest:
	git clone https://github.com/google/googletest.git
	cd googletest && \
    cmake . && \
    make

clean:
	rm -rf