#pragma once

typedef short softvec_type_t;

typedef void (*softvec_handler_t)(softvec_type_t type, unsigned long sp);

#include "intr.h"

static softvec_handler_t softvec[SOFTVEC_TYPE_NUM];

void external_intr_enable();
void external_intr_disable();

void timer_intr_enable();
void timer_intr_disable();

void software_intr_enable();
void software_intr_disable();

void intr_enable();
void intr_disable();

int is_intr_enable();

void init_softvec();

void softvec_setintr(softvec_type_t type, softvec_handler_t handler);

void interrupt(softvec_type_t type, unsigned int sp);
