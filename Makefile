define call_cmake
        echo '==============='
        echo CMAKE $(1)
        echo '==============='
	cmake -B $(1)/build -S $(1) -DCMAKE_INSTALL_PREFIX=install -DCMAKE_PREFIX_PATH=$(CURDIR)/install
	cmake --build $(1)/build
	cmake --install $(1)/build
endef

.PHONY: eigen json fplus frugal denoising

all: eigen json fplus frugal denoising

eigen:
	@$(call call_cmake,$@)
	cd install/include && ln -sf eigen3/Eigen .

json:
	@$(call call_cmake,$@)

fplus:
	@$(call call_cmake,FunctionalPlus)

frugal:
	@$(call call_cmake,frugally-deep)

clean:
	rm -rf eigen/build
	rm -rf json/build
	rm -rf FunctionalPlus/build
	rm -rf frugally-deep/build
	rm -rf install
	make -C denoising/code clean

denoising:
	make -C denoising/code all

