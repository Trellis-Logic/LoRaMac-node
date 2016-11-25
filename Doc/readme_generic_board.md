1. Overview
----------------
The generic board target moves the minimum dependencies for compile and build of the LoRa radio drivers and Mac into a set of files which can easily be extended to support board specific GPIO and timer related libraries.

The project was originally created to support the Raspberry Pi target but other generic targets could be added later.

2. CMake
----------------
The cmake directory can be used with the generic target for any cmake supported gcc like compile or cross complie platforms.  See the cmake/CMakeLists.txt file for customizable parameters.  The steps to use when building using CMake are:
 1. Install [cmake](https://cmake.org) on your target.  For raspbian this is as simple as sudo apt-get install cmake
 2. Customize the cmake/CMakeLists file to specify details about your target RTC and GPIO implementations.  See specific target sections below for detailed instructions about per-project configurations.  Use `set(BOARD_RADIO "SX1276")` to specify the radio type used.
 3. Create a "build" directory in the cmake subdirectory and cd into this directory.
 4. Run `cmake ..` from this directory to generate makefiles
 5. Run `./make` to build the project file 

3. Apps Supporting Generic Board Targets
----------------
  Use 
  ```
  set(APP "appname")
  ```
  in the cmake/CMakeLists.txt file to configure the project build for one of the targets below 


   1. ttnmapper
     An aplication used to generate coverage maps, see http://ttnmapper.org/ for project overview.  Configuration and usage details are TBD.

4. Supported Target Hardware
----------------
   1. Raspberry Pi Target
	1. To configure the Raspberry Pi target, ensure CMakeLists.txt uses
	```
	set(BOARD_GPIO "RASPBERRY_PI")
	set(BOARD_RTC "USE_POSIX_TIMER")
	```
	2. Install cmake on the pi using `sudo apt-get install cmake`
	3. Install [WiringPi](http://wiringpi.com/)
	4. Define your GPIO mapping in boards/generic/Pi/gpio-pins.h.  The default pin connections are configured as shown in [this diagram](http://www.digikey.com/schemeit/project/pi-to-inair9b-connection-Q6JIJC8202QG/) for a [Modtronics InAir9b](http://modtronix.com/inair9.html).
