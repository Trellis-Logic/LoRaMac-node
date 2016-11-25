/*
 * interrupt_simulate_posix.h
 *
 *  Created on: Aug 14, 2016
 *      Author: Dan
 */

#ifndef SRC_BOARDS_GENERIC_POSIX_INTERRUPT_SIMULATE_POSIX_H_
#define SRC_BOARDS_GENERIC_POSIX_INTERRUPT_SIMULATE_POSIX_H_


typedef void (*func_ptr_void)(void);
func_ptr_void interrupt_simulate_map_next_irq( func_ptr_void interrupt_callback );


#endif /* SRC_BOARDS_GENERIC_POSIX_INTERRUPT_SIMULATE_POSIX_H_ */
