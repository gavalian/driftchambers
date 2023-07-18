# Denoising Software for CLAS12 Drift Chambers

## installing

Clone the distribution with submodules and compile it using included Makefile

```
prompt> git clone --recurse-submodules https://github.com/gavalian/driftchambers.git
prompt> cd driftchambers
prompt> make
```

## usage

The code is compiled into one exacutable located in install/bin. The usage
of the program can be seen by typing:

```
prompt> ./install/bin/denoise2.exe -h

Available parameters:

  -h	--help
   
   This parameter is optional. The default value is ''.

  -i	--input	(required)
   input file name

  -o	--output
   output denoised file name
   This parameter is optional. The default value is 'output.h5'.

  -n	--network
   neural network file name
   This parameter is optional. The default value is 'network/cnn_autoenc_0f_112.json'.

  -t	--threads
   number of threads to run
   This parameter is optional. The default value is '8'.

  -f	--frames
   number of events in each frame
   This parameter is optional. The default value is '16'.

  -l	--level
   cut off level for background hits
   This parameter is optional. The default value is '0.050000'.

  -e	--events
   maximum number of events to process
   This parameter is optional. The default value is '-1'.
   
```

The program will run multithreaded (default is set to 8 threads), the level
option is used for debugging purposes and is not needed for regular running.
The requiret parameters are the input file and the network file (usually JSON)
used by the neural network. The default networks are included with the package and
located in directory denoising/code/network. default network is cnn_autoenc_0f_112.json.
Example usage:

```
prompt> ./install/bin/denoise2.exe -i clas_005977.hipo \
           -o clas_005977_denoised.hipo \
	   -n denoising/code/network/cnn_autoenc_0f_112.json
```

The input file must contain the bank DC::tdc for the denoiser to do it's job.
