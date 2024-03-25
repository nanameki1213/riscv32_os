#pragma once

#include "interrupt.h"

void uart(softvec_type_t type, unsigned int sp);
void timer(softvec_type_t type, unsigned int sp);

