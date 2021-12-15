#ifndef MT_KERNEL_H
#define MT_KERNEL_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint-gcc.h>

#define PANIC() while (1) {}
#define SAVED_STATE_SIZE 33

typedef void (*MT_TaskFn)();
struct MT_TaskDefinition {
  MT_TaskFn task;
  uint8_t stackSize;
};

struct MT_KernelTask {
  struct MT_TaskDefinition* task;
  uint16_t stackStart;
  uint16_t sp;
  uint16_t pc;
};

#define MT_NUM_TASKS 16
struct MT_Kernel {
  struct MT_KernelTask tasks[MT_NUM_TASKS];
  uint8_t tasksRegistered;
  uint8_t activeTask;
  uint16_t nextStackAddress;
};

extern uint8_t MT_tmp_reg;
extern uint8_t MT_pc_high;
extern uint8_t MT_pc_low;
extern uint8_t MT_sp_high;
extern uint8_t MT_sp_low;

extern struct MT_Kernel MT_kernel;

void MT_kernel_init();
void MT_kernel_start();
void MT_register_task(struct MT_TaskDefinition* task);

#define MT_SAVE_TASK_CTX()                                                                    \
  __asm__ __volatile__ (                                                                        \
      /* The next two bytes on the stack hold the the current PC */                             \
    "sts    MT_tmp_reg, __tmp_reg__         \n\t"                                               \
    "pop    __tmp_reg__                     \n\t"                                               \
    "sts    MT_pc_high, __tmp_reg__         \n\t"                                               \
    "pop    __tmp_reg__                     \n\t"                                               \
    "sts    MT_pc_low, __tmp_reg__          \n\t"                                               \
    "lds    __tmp_reg__, MT_tmp_reg         \n\t"                                               \
    /* */                                                                                       \
    "push   __tmp_reg__                     \n\t" /* Need to save the scratch register */       \
    "in     __tmp_reg__, __SREG__           \n\t" /* Read out the SREG and place in scratch */  \
    "push   __tmp_reg__                     \n\t" /* Save SREG */                               \
    "push   __zero_reg__                    \n\t" /* Save Zero Reg (r1) */                      \
    "clr    __zero_reg__                    \n\t" /* Clear Zero Reg (r1) */                     \
    "push   r2                              \n\t" /* Save r2 */                                 \
    "push   r3                              \n\t" /* Save r3 */                                 \
    "push   r4                              \n\t" /* Save r4 */                                 \
    "push   r5                              \n\t" /* Save r5 */                                 \
    "push   r6                              \n\t" /* Save r6 */                                 \
    "push   r7                              \n\t" /* Save r7 */                                 \
    "push   r8                              \n\t" /* Save r8 */                                 \
    "push   r9                              \n\t" /* Save r9 */                                 \
    "push   r10                             \n\t" /* Save r10 */                                \
    "push   r11                             \n\t" /* Save r11 */                                \
    "push   r12                             \n\t" /* Save r12 */                                \
    "push   r13                             \n\t" /* Save r13 */                                \
    "push   r14                             \n\t" /* Save r14 */                                \
    "push   r15                             \n\t" /* Save r15 */                                \
    "push   r16                             \n\t" /* Save r16 */                                \
    "push   r17                             \n\t" /* Save r17 */                                \
    "push   r18                             \n\t" /* Save r18 */                                \
    "push   r19                             \n\t" /* Save r19 */                                \
    "push   r20                             \n\t" /* Save r20 */                                \
    "push   r21                             \n\t" /* Save r21 */                                \
    "push   r22                             \n\t" /* Save r22 */                                \
    "push   r23                             \n\t" /* Save r23 */                                \
    "push   r24                             \n\t" /* Save r24 */                                \
    "push   r25                             \n\t" /* Save r25 */                                \
    "push   r26                             \n\t" /* Save r26 */                                \
    "push   r27                             \n\t" /* Save r27 */                                \
    "push   r28                             \n\t" /* Save r28 */                                \
    "push   r29                             \n\t" /* Save r29 */                                \
    "push   r30                             \n\t" /* Save r30 */                                \
    "push   r31                             \n\t" /* Save r31 */                                \
    "in     __tmp_reg__, __SP_L__           \n\t" /* Store the current SP to shared global */   \
    "sts    MT_sp_low, __tmp_reg__          \n\t" /* Store the current SP to shared global */   \
    "in     __tmp_reg__, __SP_H__           \n\t" /* Store the current SP to shared global */   \
    "sts    MT_sp_high, __tmp_reg__         \n\t" /* Store the current SP to shared global */   \
  );

#define MT_RESTORE_TASK_CTX()                     \
  __asm__ __volatile__ (                            \
    /* Restore this tasks SP */                     \
    "lds __tmp_reg__, MT_sp_low             \n\t"   \
    "out __SP_L__, __tmp_reg__              \n\t"   \
    "lds __tmp_reg__, MT_sp_high            \n\t"   \
    "out __SP_H__, __tmp_reg__              \n\t"   \
    /* Pop register state off the stack */          \
    "pop    r31                             \n\t"   \
    "pop    r30                             \n\t"   \
    "pop    r29                             \n\t"   \
    "pop    r28                             \n\t"   \
    "pop    r27                             \n\t"   \
    "pop    r26                             \n\t"   \
    "pop    r25                             \n\t"   \
    "pop    r24                             \n\t"   \
    "pop    r23                             \n\t"   \
    "pop    r22                             \n\t"   \
    "pop    r21                             \n\t"   \
    "pop    r20                             \n\t"   \
    "pop    r19                             \n\t"   \
    "pop    r18                             \n\t"   \
    "pop    r17                             \n\t"   \
    "pop    r16                             \n\t"   \
    "pop    r15                             \n\t"   \
    "pop    r14                             \n\t"   \
    "pop    r13                             \n\t"   \
    "pop    r12                             \n\t"   \
    "pop    r11                             \n\t"   \
    "pop    r10                             \n\t"   \
    "pop    r9                              \n\t"   \
    "pop    r8                              \n\t"   \
    "pop    r7                              \n\t"   \
    "pop    r6                              \n\t"   \
    "pop    r5                              \n\t"   \
    "pop    r4                              \n\t"   \
    "pop    r3                              \n\t"   \
    "pop    r2                              \n\t"   \
    "pop    __zero_reg__                    \n\t"   \
    "pop    __tmp_reg__                     \n\t"   \
    "out    __SREG__, __tmp_reg__           \n\t"   \
    "pop    __tmp_reg__                     \n\t"   \
    /* Save tmp reg to be able to load PC */        \
    "sts    MT_tmp_reg, __tmp_reg__         \n\t"   \
    /* Load new PC */                               \
    "lds    __tmp_reg__, MT_pc_low          \n\t"   \
    "push   __tmp_reg__                     \n\t"   \
    "lds    __tmp_reg__, MT_pc_high         \n\t"   \
    "push   __tmp_reg__                     \n\t"   \
    /* Restore tmp reg */                           \
    "lds    __tmp_reg__, MT_tmp_reg         \n\t"   \
  );

#endif
