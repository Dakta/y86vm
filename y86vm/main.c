//
//  main.c
//  y86vm
//
//  Created by Dakota Schneider on 4/19/15.
//  Copyright (c) 2015 Code From Above. All rights reserved.
//

/**
 * y86vm
 * A bare-bones emulator/virtual machine for the Y86 assembly language.
 * © 2015 Dakota Schneider, all rights reserved. Unlicensed, do not distriute.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sysexits.h>

#include "utils.h"
#include "environment.h"
#include "parseArguments.h"


// instruction handlers
#include "stack.h"
#include "arithmeticLogic.h"

VirtualMachineState * state;
Config * config;

int main(int argc, char const *argv[]) {
  // parse arguments and get our configuration
  config = parseArguments(argc, argv);
  
  if (config->verbose) {
    printf("Config {\n  verbose = %s;\n  maxSteps = %lu;\n}\n",
           config->verbose ? "true" : "false",
           config->maxSteps);
  }

  // initialize our VM state
  state = malloc(sizeof(VirtualMachineState));
  // program counter
  state->PC = 0;
  // ALU flags
  state->ZF = false;
  state->SF = false;
  state->OF = false;
  // program status
  state->STAT = STAT_AOK;
  // program memory
  state->DMEM = malloc(MEM_SZ);
  
  // TODO: replace this with in-memory stack
  state->stack = newStack();
  
//  // load program into an easy-to-index variable
//  // http://stackoverflow.com/a/14002993/362042
//  FILE *f = fopen(argv[argc - 1], "rb"); // TODO: value checking on the source file path
//  fseek(f, 0, SEEK_END);
//  long fsize = ftell(f);
//  fseek(f, 0, SEEK_SET);
//  
//  char *program = malloc(fsize + 1);
//  fread(program, fsize, 1, f);
//  fclose(f);
//  
//  program[fsize] = 0; // set null terminator
//
  
  /*
   * Load program into char buffer.
   */
  long  file_size;
  
  // TODO: error checking on this argument
  file_size = slurp(argv[argc - 1], &state->code, false);
  if( file_size < 0L ) {
    perror("File read failed");
    exit(EX_NOINPUT);
  }
  /* Remember to free() memory allocated by slurp() */
  
  // begin the main execution loop
  do {
    state->steps++;
    
    long int valA;
    long int valB;
    long int valC;
    long int valE;
    long int valP;
    char value[8]; // used for parsing hardcoded values
    
    // 1. Fetch:
    //     - Instruction bytes are read from memory based on Program Counter (P.C.)
    //     - First byte is two four-bit values: instruction code (icode) and function code (ifun)
    //     - One or more register operands are loaded (rA and rB)
    //     - May load a 32-bit constant valC
    //     - Next program instruction address is computed, called `valP`
    
    // Fetch will be performed here, and a switch case will pass execution to a specialized function.
    
    /* example.yo:
     30f0cdab0000
     a00f
     30f20e000000
     6120
     b02f
     00
     */
    char icode = state->code[state->PC];
    char ifun  = state->code[state->PC + 1];
    
    int rA = state->code[state->PC + 2] - 48;
    int rB = state->code[state->PC + 3] - 48;

    printf("icode = %c\n ifun = %c\n", icode, ifun);
    
    switch(icode) {
      case '0':
        // halt
        state->STAT = STAT_HLT;
        break;
      case '1':
        // nop
        state->PC += 2;
        break;
      case '2':
        // rrmovl
        // TODO validation
        state->registers[rB] = state->registers[rA];
        state->PC += 4;
        break;
      case '3':
        // irmovl
        // de-little-endian bytes
        // TODO: validation
        strncpy(value, &state->code[state->PC + 4], 8);
        valA = bigEndianCharArrayToInt(littleToBigEndianChars(value));
        state->registers[rA] = valA;
        state->PC += 12;
        break;
      case '4':
        // rmmovl
        strncpy(value, &state->code[state->PC + 4], 8);
        valA = state->registers[rA];
        valB = state->registers[rB];
        valC = bigEndianCharArrayToInt(littleToBigEndianChars(value));
        valP = state->PC + 12;
        
        valE = valB + valC;
        state->DMEM[valE] = valA;
        state->PC = valP;
        break;
      case '5':
        // mrmovl
        state->PC += 12;
        break;
      case '6':
        // ALU OPl
        OPl(ifun, rA, rB);
        state->PC += 4;
        break;
      case '7':
        // jump
        state->PC += 10;
        break;
      case '8':
        // call
        strncpy(value, &state->code[state->PC + 2], 8);
        valA = bigEndianCharArrayToInt(littleToBigEndianChars(value));
        state->PC = valA;
        break;
      case '9':
        // ret
        state->PC += 2;
        break;
      case 'a':
        // pushl
        rA = state->code[state->PC + 2] - 48; // convert char to int
        valA = state->registers[rA];
        push(state->stack, valA);
        state->PC += 4;
        break;
      case 'b':
        // popl
        rA = state->code[state->PC + 2] - 48; // convert char to int
        valA = pop(state->stack);
        state->registers[rA] = valA;
        state->PC += 4;
        break;
      default:
        // invalid instruction
        state->STAT = STAT_INS;
        break;
    }
    
    // All of the following steps will be performed within functions
    
    // 2. Decode:
    //     - Read register values into valA and valB
    //     - Read %esp
    
    // 3. Execute:
    //     - ALU performs operation defined by instruction -> ValE
    //     - Effective memory address calculation -> ValE
    //     - Stack pointer increment/decrement -> ValE
    //     - Condition codes are set
    
    // 4. Memory:
    //     - May write data to memory
    //     - May read value from memory into ValM
    
    // until we run out of program or exceed the specified max steps
  } while (state->PC < file_size
           && !(!config->maxSteps != !(state->steps < config->maxSteps))
           && state->STAT == STAT_AOK);
  
  while (state->stack->top) {
    printf("%lu\n", pop(state->stack));
  }
  
  return 0;
}
