.text
.globl m_vectors
.balign 256
m_vectors:
  j intr_software
  .balign 4
  j intr_software
  .balign 4
  j m_undefined_handler
  .balign 4
  j intr_software 
  .balign 4
  j m_undefined_handler
  .balign 4
  j intr_timer
  .balign 4
  j m_undefined_handler
  .balign 4
  j intr_timer
  .balign 4
  j m_undefined_handler
  .balign 4
  j intr_external
  .balign 4
  j m_undefined_handler
  .balign 4
  j intr_external

m_undefined_handler:
  mret

.text
.globl s_vectors
.balign 256
s_vectors:
  j intr_software
  .balign 4
  j intr_software
  .balign 4
  j s_undefined_handler
  .balign 4
  j intr_software
  .balign 4
  j intr_timer
  .balign 4
  j intr_timer
  .balign 4
  j s_undefined_handler
  .balign 4
  j intr_timer
  .balign 4
  j intr_external
  .balign 4
  j intr_external
  .balign 4
  j s_undefined_handler
  .balign 4
  j intr_external

s_undefined_handler:
  sret
