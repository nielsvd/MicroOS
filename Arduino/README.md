# Handling the Arduino with CMake

Do the following:
```
mkdir build && cd build
cmake ..
```
Check the output - if there are no errors you have three possibilities:
- only generating an executable of the defined files, stored in `bin/`, by running
```
make
```
- additionally generating a `.hex`-file of the executable, also stored in `bin/`, by running
```
make hex
```
- additionally flashing the hex-file to a connected Arduino by running
```
make flash
```
