/*
 * hss-filter.c
 *
 *  Created on: Mar 20, 2023
 *      Author: kgaya
 */

#include "hss-filter.h"

uint16_t pow2(uint16_t p)
{
    return 1<< p;
}

void exponentialFilter_init(ExponentialFilterState* state)
{
	state->x = 0.0f;
	state->y = 0.0f;
	state->y_last = 0.0f;
	state->tau = 1;
}

void exponentialFilter_init_1(ExponentialFilterState* state, float y)
{
    state->y = y;
	state->y_last = y;
	state->x = y;
}

void exponentialFilter_init_2(ExponentialFilterState* state, float y, uint16_t t)
{
    state->y = y;
	state->y_last = y;
	state->x = y;
	state->tau = pow2(t);
}

void exponentialFilter_set_filter(ExponentialFilterState* state, float y, uint16_t t)
{
    state->y = y;
	state->y_last = y;
	state->x = y;
	state->tau = pow2(t);
}

void exponentialFilter_set_tau(ExponentialFilterState* state, uint16_t t)
{
	state->tau = pow2(t);
}

void exponentialFilter_set_newVal(ExponentialFilterState* state, float y)
{
    state->y = state->y_last = state->x = y;
}

void exponentialFilter_input(ExponentialFilterState* state, float x)
{
	state->y_last = state->y;
	state->x = x;
	state->y = state->y_last + ((state->x - state->y_last)/(float)state->tau);
}

float exponentialFilter_output(ExponentialFilterState* state)
{
    return state->y;
}


