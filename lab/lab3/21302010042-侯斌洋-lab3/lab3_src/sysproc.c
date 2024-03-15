#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
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

#ifdef LAB_PGTBL
int sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 start_va;
  int pg_num;
  uint64 user_addr_bits_mask;

  // get args
  argaddr(0, &start_va);
  argint(1, &pg_num);
  argaddr(2, &user_addr_bits_mask);

  // get pagetables
  struct proc *cur_proc = myproc();
  pagetable_t pgtb = cur_proc->pagetable;

  unsigned int bits_buf = 0;
  unsigned long bits_mask = 1;
  pte_t *pte;

  //vmprint(pgtb);

  for (uint64 i = 0; i < pg_num; i++)
  {
    pte = walk(pgtb, start_va + i * PGSIZE, 0);
    if ((*pte & PTE_A) != 0)
    {
      bits_buf += bits_mask;
    }
    // printf("pte : %p\n", *pte);
    // printf("bits_mask : %p\n", bits_mask);
    // printf("bits_buf  : %p\n", bits_buf);
    bits_mask = (bits_mask << 1);
    *pte = ((*pte) & (~PTE_A));
  }
  copyout(pgtb, user_addr_bits_mask, (char *)(&bits_buf), sizeof(user_addr_bits_mask));
  return 0;
}
#endif

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
