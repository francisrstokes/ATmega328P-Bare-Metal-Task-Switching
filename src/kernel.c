#include "kernel.h"
#include "util.h"
#include <string.h>

void MT_kernel_init() {
  // Turn off interrupts until the kernel is started
  cli();

  MT_kernel.tasksRegistered = 0;
  MT_kernel.activeTask = 0;
  MT_kernel.nextStackAddress = RAMEND;

  WDTCSR = _BV(WDIE); // WDT Enabled, in interrupt mode, every 16ms
}

void MT_kernel_start() {
  if (MT_kernel.tasksRegistered) {
    // Get the stack pointer of this task, but leave space to push a return address
    uint16_t sp = MT_kernel.tasks[MT_kernel.activeTask].sp - sizeof(void *);
    MT_sp_low = U16_TO_U8_L(sp);
    MT_sp_high = U16_TO_U8_H(sp);

    __asm__ __volatile__ (
      "lds __tmp_reg__, MT_sp_low   \n\t" // Set the stack pointer
      "out __SP_L__, __tmp_reg__    \n\t" // Set the stack pointer
      "lds __tmp_reg__, MT_sp_high  \n\t" // Set the stack pointer
      "out __SP_H__, __tmp_reg__    \n\t" // Set the stack pointer
      "push %[pcl]                  \n\t" // Push low byte of pc to stack
      "push %[pch]                  \n\t" // Push high byte of pc to stack
      : /* no outputs */
      : [pcl] "r"(U16_TO_U8_L(MT_kernel.tasks[MT_kernel.activeTask].pc))
      , [pch] "r"(U16_TO_U8_H(MT_kernel.tasks[MT_kernel.activeTask].pc))
    );

    // Enable interrupts
    sei();

    // On return, control will transfer to the first task
    return;
  }
}

void MT_register_task(struct MT_TaskDefinition* task) {
  if (MT_kernel.tasksRegistered >= MT_NUM_TASKS) {
    PANIC();
  }

  MT_kernel.tasks[MT_kernel.tasksRegistered].task = task;
  MT_kernel.tasks[MT_kernel.tasksRegistered].stackStart = MT_kernel.nextStackAddress;
  MT_kernel.tasks[MT_kernel.tasksRegistered].sp = MT_kernel.nextStackAddress;
  MT_kernel.tasks[MT_kernel.tasksRegistered].pc = (uint16_t)task->task;

  // Every task after the first will need to perform a stack restore, even on it's
  // first run. Here we make sure that the stack pointer is correctly offset, and that
  // the stack memory is zeroed out.
  if (MT_kernel.tasksRegistered > 0) {
    MT_kernel.tasks[MT_kernel.tasksRegistered].sp -= SAVED_STATE_SIZE;
    memset((void *)(MT_kernel.nextStackAddress - SAVED_STATE_SIZE), 0, SAVED_STATE_SIZE);
  }

  MT_kernel.tasksRegistered++;
  MT_kernel.nextStackAddress -= task->stackSize + SAVED_STATE_SIZE;
}

// -DMT_ENABLED must be passed to gcc at compile time in order to register the interrupt handler
#ifdef MT_ENABLED
void WDT_vect (void) __attribute__ ((signal, naked));
void WDT_vect (void) {
  if (MT_kernel.tasksRegistered > 0) {
    cli();

    // Save the current state
    MT_SAVE_TASK_CTX();

    // Save this tasks PC
    MT_kernel.tasks[MT_kernel.activeTask].pc = (
      ((uint16_t)MT_pc_high << 8) | (uint16_t)(MT_pc_low)
    );

    // Save this tasks SP
    MT_kernel.tasks[MT_kernel.activeTask].sp = (
      ((uint16_t)MT_sp_high << 8) | (uint16_t)(MT_sp_low)
    );

    // Set up next task
    MT_kernel.activeTask += 1;
    if (MT_kernel.activeTask >= MT_kernel.tasksRegistered) {
      MT_kernel.activeTask = 0;
    }

    // Place this tasks stack pointer in the global reference
    MT_sp_low  = U16_TO_U8_L(MT_kernel.tasks[MT_kernel.activeTask].sp);
    MT_sp_high = U16_TO_U8_H(MT_kernel.tasks[MT_kernel.activeTask].sp);

    // Place this tasks program counter in the global reference
    MT_pc_low  = U16_TO_U8_L(MT_kernel.tasks[MT_kernel.activeTask].pc);
    MT_pc_high = U16_TO_U8_H(MT_kernel.tasks[MT_kernel.activeTask].pc);

    MT_RESTORE_TASK_CTX();
  }

  // Enable interrupts
  sei();

  reti();
}
#endif

uint8_t MT_tmp_reg;
uint8_t MT_pc_high;
uint8_t MT_pc_low;
uint8_t MT_sp_high;
uint8_t MT_sp_low;
struct MT_Kernel MT_kernel;