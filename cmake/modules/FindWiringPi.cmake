find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
find_path(WIRINGPI_INCLUDE_DIRS NAMES wiringPi.h)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(wiringPi "Could not find WiringPi.  Either install it using instructions at http://wiringpi.com/download-and-install/ or specify a different BOARD_GPIO" WIRINGPI_LIBRARIES WIRINGPI_INCLUDE_DIRS)
