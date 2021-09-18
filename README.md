# Fractoid

Fractoid is a free software to generate beautiful fractal images. Fractoid consists of a dependency-free library and a
command line interface.

## Installation

Before the standalone executable, the library must be compiled. Navigate to the project folder and run `cmake .`
followed by `make fractoid`. Now that the library has been compiled you can compile the executable by running `make`
in the project folder. Both binaries will be compiled into the `bin` folder.

## Usage

### Library

To create a fractal object, simply call the class constructor with the required parameters.
```c++
Mandelbrot fractal(80, 10);
```
After initialization of the fractal object, we need to create an `Algorithm` object, which will hold a specified method
of generation. For demonstration purposes, we can use smoothed periodic coloring with an example seed of 34.
```c++
Algorithm alg = Algorithm::periodic(true, 34);
```
For the fractal to have a black filling, we need to specify the fill color. If you don't specify the fill color, the
color will be determined based on the number of iteration before bailout inside the fractal.
```c++
alg.fill(0, 0, 0);
```
Now it's time to paint the fractal inside the `Image` object. We need to specify the center, zoom, algorithm and
resolution.
```c++
Image image = fractal.paint(-0.75, 0, 1, alg, 1920, 1080);
```
We can then save the image into a `.png` file.
```c++
image.save("fractal.png");
```
If you followed these steps carefully, the following image will be generated.

![example1.png](./img/example1.png)

### CLI

The usage of the CLI is pretty straightforward. To see all the available options simply run the `fractoid-cli`
executable without any arguments. To replicate the image above, run the following command.
```shell
./fractoid-cli mandelbrot -l -0.75 0 -s 34 -f 0 0 0 --smooth
```