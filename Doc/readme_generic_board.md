1. Overview
----------------
The generic board target moves the minimum dependencies for compile and build of the LoRa radio drivers and Mac into a set of files which can easily be extended to support board specific GPIO and timer related libraries.

The project was originally created to support the Raspberry Pi target but other generic targets could be added later.

2. Installing Dependencies
----------------

### Install CMake

Install [cmake](https://cmake.org) on your target

```
sudo apt-get install cmake libc6
```

### Install WiringPi on the RPi

```
sudo apt-get install git-core
mkdir -p ~/raspberrypi
cd ~/raspberrypi
git clone git://git.drogon.net/wiringPi
cd wiringPi
git pull origin
./build #compile and install lib
```

3. CMake
----------------
The cmake directory can be used with the generic target for any cmake supported gcc like compile or cross complie platforms.  See the cmake/CMakeLists.txt file for customizable parameters.  The steps to use when building using CMake are:
 1. Customize the cmake/CMakeLists file to specify details about your target RTC and GPIO implementations.  See specific target sections below for detailed instructions about per-project configurations.  Use `set(BOARD_RADIO "SX1276")` to specify the radio type used.
 2. Create a "build" directory in the cmake subdirectory and cd into this directory.
    ```
    mkdir -p build
    cd build
    ```
 3. Run `cmake ..` from this directory to generate makefiles
 4. Run `./make` to build the project file


### Cross-Compile with CMake for RPi


 1. Add dependencies

    ```
    apt-get install git rsync cmake libc6-i386 lib32z1 lib32stdc++6
    ```

 2. Get the Raspberry Pi tools and toolchain

    ```
    mkdir -p ~/raspberrypi
    cd ~/raspberrypi
    git clone git://github.com/raspberrypi/tools.git
    ```

 3. Add the needes paths to the ENV variables.

    ```
    echo 'export PATH=$PATH:$HOME/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin' >>  ~/.bashrc

    source ~/.bashrc
    ```

 4. Now, verify that you can access the compiler:

    ```
    arm-linux-gnueabihf-gcc -v
    ```

 5. In your raspberrypi folder, make a folder called rootfs.

    ```
    mkdir -p ~/raspberrypi/rootfs
    cd ~/raspberrypi/
    ```

6. Connect to your Raspberry Pi board and install the needed libs. In our case, we need WiringPi.

    ```
    sudo apt-get install git-core
    git clone git://git.drogon.net/wiringPi
    cd wiringPi
    git pull origin
    ./build #compile and install the lib
    ```

 7. Now you need to copy the entire /lib and /usr directory from the Raspberry Pi to this newly created folder in our local machine. 

    ```
	rsync -rlv links -e "ssh -p 22"  pi@raspberry-pi.local:/{lib,usr} $HOME/raspberrypi/rootfs
    ```

 8. Now, we need to write a cmake config file. Open **~/raspberrypi/pi.cmake** in your favorite editor and insert the following:

    ```
	SET(CMAKE_SYSTEM_NAME Linux)
	SET(CMAKE_SYSTEM_VERSION 1)
	SET(CMAKE_C_COMPILER $ENV{HOME}/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc)
	SET(CMAKE_CXX_COMPILER $ENV{HOME}/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++)
	SET(CMAKE_FIND_ROOT_PATH $ENV{HOME}/raspberrypi/rootfs)
	SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
	SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
	SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    ```

 9. Now you should be able to compile your cmake programs simply by adding this extra flag:

    ```
	 -D CMAKE_TOOLCHAIN_FILE=$HOME/raspberrypi/pi.cmake
    ```

    **Example:**

    ```
    cmake -D CMAKE_TOOLCHAIN_FILE=$HOME/raspberrypi/pi.cmake ../ && make
    ```

 10. Demo/Example:

    ```
    cd /tmp
    mkdir -p helloworld/build
    cd helloworld

    cat << EOF > helloworld.cpp
    #include<iostream>

    int main(int argc, char *argv[]){
       std::cout << "Hello World!" << std::endl;
       return 0;
    }
    EOF

    cat << EOF > CMakeLists.txt
    cmake_minimum_required(VERSION 2.8.9)
    project (hello)
    add_executable(hello helloworld.cpp)
    EOF

    mkdir -p build
    cd build
    cmake -D CMAKE_TOOLCHAIN_FILE=$HOME/raspberrypi/pi.cmake ../
    make

    scp -P 22 CMakeHelloWorld pi@raspberry-pi.local:/tmp/
    ssh pi@raspberry-pi.local -p 22 /tmp/CMakeHelloWorld

    ```

### Cross-Compile with Make for RPi v2,3

 1. Follow the steps 1. to 7. from the **Cross-Compile with CMake for RPi** instructions.
 2. To compile your C++ code run your makefile with:

    ```
	make CXX=arm-linux-gnueabihf-g++ LD=arm-linux-gnueabihf-ld
    ```

4. Apps Supporting Generic Board Targets
----------------
  Use 
  ```
  set(APP "appname")
  ```
  in the cmake/CMakeLists.txt file to configure the project build for one of the targets below 


   1. ttnmapper
     An aplication used to generate coverage maps, see http://ttnmapper.org/ for project overview.  Configuration and usage details are TBD.

5. Supported Target Hardware
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
