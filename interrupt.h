#pragma once

typedef short intr_type_t;

// typedef void (*softvec_handler_t)(softvec_type_t type, unsigned int )

void external_intr_enable();
void external_intr_disable();

void timer_intr_enable();
void timer_intr_disable();

void software_intr_enable();
void software_intr_disable();

void intr_enable();
void intr_disable();

int is_intr_enable();

void interrupt(intr_type_t type, unsigned int sp);