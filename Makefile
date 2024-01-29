.PHONY: eigen json fplus frugal denoising

all: eigen json fplus frugal denoising

eigen:
	rm -rf build && mkdir -p build install && cd build && cmake -DCMAKE_INSTALL_PREFIX=../install ../eigen && make install
#	cd install/include && ln -s eigen3/Eigen .

json:
	rm -rf build && mkdir -p build install && cd build && cmake -DCMAKE_INSTALL_PREFIX=../install ../json && make install

fplus:
	rm -rf build && mkdir -p build install && cd build && cmake -DCMAKE_INSTALL_PREFIX=../install ../FunctionalPlus && make install

frugal:
	rm -rf build && mkdir -p build install && cd build && cmake -DCMAKE_INSTALL_PREFIX=../install ../frugally-deep && make install

clean:
	rm -rf build install
	make -C denoising/code clean

denoising:
	make -C denoising/code all

