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
#include <stdint.h>

#define VERSION "y86vm v0.1\n"

#define MAX_STEPS 1024

// status codes
#define STAT_AOK 1
#define STAT_HLT 2
#define STAT_ADR 3
#define STAT_INS 4

// condition codes
#define COND_NONE 0
#define COND_LTE  1
#define COND_LT   2
#define COND_EQ   3
#define COND_NE   4
#define COND_GTE  5
#define COND_GT   6

// registers
#define REG_EAX  0
#define REG_ECX  1
#define REG_EDX  2
#define REG_EBX  3
#define REG_ESP  4
#define REG_EBP  5
#define REG_ESI  6
#define REG_EDI  7
#define REG_NONE 15

// input formats
#define INPUT_BYTES  'b'
#define INPUT_ASCII  'a'
// unused
#define INPUT_DETECT 'd'

// 
#define MEM_SZ 65536 // 2 to the 19 bits in bytes

typedef struct Config {
  bool verbose;
  unsigned int logLevel;
  unsigned long int maxSteps;
  char inputFormat;
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
  // TODO: replace this with storage in program memory
  char * sourceASCII;
  uint8_t * sourceBytes;
  // how many steps has program taken
  unsigned long int steps;
  // program counter
  unsigned int PC;
  // ALU flags
  bool ZF;
  bool SF;
  bool OF;
  // program status
  uint8_t STAT;
  /**
   * 1 AOK - Normal operation
   * 2 HLT - Halt instruction encountered
   * 3 ADR - Bad address (either instruction or data) encountered
   * 4 INS - Invalid instruction encountered
   **/
  // program memory
  uint8_t * DMEM; // 2 to the 19 bits in bytes
  // program registers
  int registers[8];
  
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
