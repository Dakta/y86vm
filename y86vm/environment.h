//
//  environment.h
//  y86vm
//
//  Created by Dakota on 4/19/15.
//  Copyright (c) 2015 Code From Above. All rights reserved.
//

#ifndef y86vm_environment_h
#define y86vm_environment_h

#include <stdbool.h>

#define VERSION "y86vm v0.1\n"

#define MAX_STEPS 1024

// status codes
#define STAT_AOK 1
#define STAT_HLT 2
#define STAT_ADR 3
#define STAT_INS 4

// 
#define MEM_SZ 65536 // 2 to the 19 bits in bytes

typedef struct Config {
  bool verbose;
  unsigned long int maxSteps;
  // struct Node* ord_prev;
} Config;

typedef struct StackNode {
  int val;
  struct StackNode * prev;
} StackNode;

typedef struct Stack {
  int size;
  struct StackNode * top;
} Stack;

typedef struct VirtualMachineState {
  char * code;
  // program counter
  unsigned long int PC;
  // ALU flags
  bool ZF;
  bool SF;
  bool OF;
  // program status
  int STAT;
  /**
   * 1 AOK - Normal operation
   * 2 HLT - Halt instruction encountered
   * 3 ADR - Bad address (either instruction or data) encountered
   * 4 INS - Invalid instruction encountered
   **/
  // program memory
  char * DMEM; // 2 to the 19 bits in bytes
  // program registers
  long int registers[8];
  
//  unsigned int eax = 0;
//  unsigned int ebx = 0;
//  unsigned int ecx = 0;
//  unsigned int edx = 0;
//  // ---
//  unsigned int esi = 0;
//  unsigned int edi = 0;
//  unsigned int esp = 0;
//  unsigned int ebp = 0;
  
  // we have a stack
  Stack * stack;
} VirtualMachineState;

#endif
