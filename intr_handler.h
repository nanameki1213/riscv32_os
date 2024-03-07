#pragma once

#include "interrupt.h"

void uart(intr_type_t type, unsigned int sp);
void timer(intr_type_t type, unsigned int sp);

