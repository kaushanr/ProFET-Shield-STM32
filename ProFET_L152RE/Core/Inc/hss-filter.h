/*
 * hss-filter.h
 *
 *  Created on: Mar 20, 2023
 *      Author: kgaya
 */

#include "stdint.h"

#ifndef INC_HSS_FILTER_H_
#define INC_HSS_FILTER_H_

typedef struct {
	float tau;   /**< Time constant */
	float y;        /**< Output value */
	float y_last;   /**< Last output value */
	float x;
} ExponentialFilterState;

void exponentialFilter_init(ExponentialFilterState* state);
void exponentialFilter_init_1(ExponentialFilterState* state, float y);
void exponentialFilter_init_2(ExponentialFilterState* state, float y, uint16_t t);
void exponentialFilter_set_filter(ExponentialFilterState* state, float y, uint16_t t);
void exponentialFilter_set_tau(ExponentialFilterState* state, uint16_t t);
void exponentialFilter_set_newVal(ExponentialFilterState* state, float y);
void exponentialFilter_input(ExponentialFilterState* state, float x);
float exponentialFilter_output(ExponentialFilterState* state);


#endif /* INC_HSS_FILTER_H_ */
