#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_shutdown(void)
{
  /* Either of the following will work. Does not harm to put them together. */
  outw(0xB004, 0x0|0x2000); // working for old qemu
  outw(0x604, 0x0|0x2000); // working for newer qemu
  
  return 0;
}

extern int sched_trace_enabled;
extern int sched_trace_counter;
int sys_enable_sched_trace(void)
{
  if (argint(0, &sched_trace_enabled) < 0)
 {
    cprintf("enable_sched_trace() failed!\n");
  }
  
  sched_trace_counter = 0;

  return 0;
}

// Daniel: added fork_winner
int fork_winner_flag = 0; // Default: Parent runs first

int sys_fork_winner(void) {
    int winner;
    if (argint(0, &winner) < 0)
        return -1;
    fork_winner_flag = (winner == 1) ? 1 : 0;
    return 0;
}

// Daniel: added set_schedint
int scheduler_type = 0; // 0 for RR, 1 for Stride

int sys_set_sched(void){
    int sched_type;

    if (argint(0, &sched_type) < 0){
      return -1; // Fail
    }

    if (sched_type == 0 || sched_type == 1){
      scheduler_type = sched_type;
      return 0;  // Success
    }
    return 0;
}

// Xavier 
int sys_tickets_owned( void ) {
  int pid;
  struct proc* p;
  
  if ( argint( 0, &pid ) < 0 ) return -1;
  acquire( &ptable.lock );

  for ( p = ptable.proc; p < &ptable.proc[ NPROC ]; p++ ) {
    if ( p->pid == pid && p->state != UNUSED ) { 
      int tickets = p->tickets;
      release( &ptable.lock );
      return tickets;
    }
  }

  release( &ptable.lock );
  return -1;
} 
