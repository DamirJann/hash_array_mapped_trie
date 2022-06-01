all: dependencies

dependencies: benchmark libcds googletest

libcds:
	git clone https://github.com/DamirJann/libcds.git
	mkdir libcds-release
	cd libcds-release && \
    cmake -DLIBCDS_WITH_TESTS=OFF -DCMAKE_BUILD_TYPE=RELEASE ../libcds  && \
    make -j4 && \
    cp -rf ../libcds libcds

benchmark:
	git clone https://github.com/google/benchmark
	mkdir benchmark-release
	cd benchmark-release && \
	cmake -E make_directory "build" && \
	cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ../../benchmark && \
	cmake --build "build" --config Release && \
	cp -r ../benchmark/include include

googletest:
	git clone https://github.com/google/googletest.git
	mkdir googletest-release
	cd googletest-release && \
    cmake ../googletest && \
	make  && \
	cp -r  ../googletest ./googletest

clean:
	rm -rf