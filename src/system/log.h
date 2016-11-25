/*
 * log.h
 *
 *  Created on: Jun 26, 2016
 *      Author: Dan
 */

#ifndef SRC_SYSTEM_LOG_H_
#define SRC_SYSTEM_LOG_H_

#include "board.h"

#ifndef BOARD_LOGGER
#define BOARD_LOGGER(unused1,...)
#endif

#define LOG(level,format,...)	BOARD_LOGGER(level,format , ##__VA_ARGS__)


#endif /* SRC_SYSTEM_LOG_H_ */
