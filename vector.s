.text
.globl m_vectors
.balign 256
m_vectors:
  j m_undefined_handler
  .balign 4
  j m_undefined_handler
  .balign 4
  j m_undefined_handler
  .balign 4
  j m_undefined_handler
  .balign 4
  j m_undefined_handler
  .balign 4
  j m_undefined_handler
  .balign 4
  j m_undefined_handler
  .balign 4
  j m_undefined_handler
  .balign 4
  j m_undefined_handler
  .balign 4
  j m_undefined_handler
  .balign 4
  j intr_serial

m_undefined_handler:
  mret

.text
.globl s_vectors
.balign 256
s_vectors:
  j s_undefined_handler
  .balign 4
  j s_undefined_handler
  .balign 4
  j s_undefined_handler
  .balign 4
  j s_undefined_handler
  .balign 4
  j s_undefined_handler
  .balign 4
  j s_undefined_handler
  .balign 4
  j s_undefined_handler
  .balign 4
  j s_undefined_handler
  .balign 4
  j s_undefined_handler
  .balign 4
  j s_undefined_handler
  .balign 4
  j intr_serial

s_undefined_handler:
  sret
