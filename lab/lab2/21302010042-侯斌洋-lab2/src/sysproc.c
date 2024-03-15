#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (killed(myproc()))
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

extern struct proc proc[NPROC];
uint64
sys_procnum()
{
  //for debug
  //procdump();

  // get int pointer
  uint64 proc_addr;
  argaddr(0, &proc_addr);

  // count processs
  int count = 0;
  uint64 src = 0;
  struct proc *p;
  for (p = proc; p < &proc[NPROC]; p++)
  {
    if (p->state == UNUSED)
      count++;
  }
  src = NPROC - count;

  // copy data from kernel to user
  struct proc *cur_proc = myproc();
  copyout(cur_proc->pagetable, proc_addr, (char *)(&src), sizeof(proc_addr));
  return src;
}

extern int get_free_bytes_num();
uint64
sys_freemem()
{
  // get int pointer
  uint64 proc_addr;
  argaddr(0, &proc_addr);

  // get free bytes
  // the function below is in kalloc.c
  int num = get_free_bytes_num();
  uint64 src = num;

  // copy data from kernel to user
  struct proc *cur_proc = myproc();
  copyout(cur_proc->pagetable, proc_addr, (char *)(&src), sizeof(proc_addr));
  return num;
}

uint64
sys_trace()
{
  // get trace_mask
  int trace_mask;
  argint(0, &trace_mask);

  // add trace_mask to the process
  struct proc *cur_proc = myproc();
  acquire(&(cur_proc->lock));
  cur_proc->trace_mask = trace_mask;
  release(&(cur_proc->lock));
  return 0;
}
