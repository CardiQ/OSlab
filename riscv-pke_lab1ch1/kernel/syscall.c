/*
 * contains the implementation of all syscalls.
 */

#include <stdint.h>
#include <errno.h>

#include "util/types.h"
#include "syscall.h"
#include "string.h"
#include "process.h"
#include "util/functions.h"

#include "spike_interface/spike_utils.h"

//lab1_ch1
#include "elf.h"
#include "string.h"
extern elf_ctx elfloader;

//
// implement the SYS_user_print syscall
//
ssize_t sys_user_print(const char* buf, size_t n) {
  sprint(buf);
  return 0;
}

//
// implement the SYS_user_exit syscall
//
ssize_t sys_user_exit(uint64 code) {
  sprint("User exit with code:%d.\n", code);
  // in lab1, PKE considers only one app (one process). 
  // therefore, shutdown the system when the app calls exit()
  shutdown(code);
}

//lab1_ch1
long sys_user_print_backtrace(int x){
  char* bottom = "main";
  uint64 usersp = current->trapframe->regs.sp+16+24, layer = 0, pre_funcnum = 0,fp = usersp+16,termfp;
  int flag = 0, memo = 0, count = 0, firstflag = 0;
  for(uint64 sp = usersp; layer < x; layer++){
    //sprint("ra:%d",*(uint64*)(sp));
    if(strcmp(&elfloader.strtab[elfloader.symtab[memo].st_name],bottom)==0)//bottom
      break;
    flag = 0;
    memo = 0;
    count = 0;
    for(count = 0; count < elfloader.symsize; count++){
      if((elfloader.symtab[count].st_info == STT_FUNC) && (elfloader.symtab[count].st_value < (*(uint64*)(sp))) && (elfloader.symtab[count].st_value > pre_funcnum)){ //st_value is virtual address, while *(uint64*)(sp) is ra, which is va too
      	pre_funcnum = elfloader.symtab[count].st_value;
      	flag = 1;
      	memo = count;
      }
      //test
      //sprint("count:%d\n",count);
      //sprint("elfloader.symtab[count].st_info:%d\n",elfloader.symtab[count].st_info);
    }
    if(flag)
      sprint("%s\n", &elfloader.strtab[elfloader.symtab[memo].st_name]);
    else{
      sprint("fail to backtrace symbol %lx\n", *(uint64 *)(sp));
      continue; 
    }
    //sprint("sp:%d\nfp:%d\n",sp,fp);
    sp=fp;
    fp+=16;
    //fp = *(uint64*)(fp);
    //sprint("sp:%d\nfp:%d\n",sp,fp);
  }
  return 0;
}

//
// [a0]: the syscall number; [a1] ... [a7]: arguments to the syscalls.
// returns the code of success, (e.g., 0 means success, fail for otherwise)
//
long do_syscall(long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7) {
  switch (a0) {
    case SYS_user_print:
      return sys_user_print((const char*)a1, a2);
    case SYS_user_exit:
      return sys_user_exit(a1);
    //lab1_ch1
    case SYS_user_print_backtrace:
      return sys_user_print_backtrace(a1);
    default:
      panic("Unknown syscall %ld \n", a0);
  }
}
