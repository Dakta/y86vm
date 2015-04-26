//
//  main.c
//  y86vm
//
//  Created by Dakota Schneider on 4/19/15.
//  Copyright (c) 2015 Code From Above. All rights reserved.
//

/**
 * A bare-bones emulator/virtual machine for the Y86 assembly language.
 * © 2015 Dakota Schneider/Code From Above, LLC, all rights reserved. Unlicensed, do not distriute.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sysexits.h>

#include "utils.h"
#include "environment.h"
#include "parseArguments.h"
#include "hexString.h"


// instruction handlers
#include "stack.h"
#include "arithmeticLogic.h"


// yes, I know, global variables are icky
VirtualMachineState * state;
Config * config;

// let's rock and roll
int main(int argc, char const *argv[]) {
  // parse arguments and get our configuration
  config = parseArguments(argc, argv);
  
  logprintf(LOG_INFO, "Config {\n  verbose = %s;\n  maxSteps = %lu;\n}\n",
         config->verbose ? "true" : "false",
         config->maxSteps);

  // initialize our VM state
  state = malloc(sizeof(VirtualMachineState));
  // program counter
  // pad with 2 bytes to account for address offset
  state->PC = 2;
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
  
  /*
   * Load program into char buffer.
   */
  long file_size;
  
  // TODO: error checking on this argument
  file_size = slurp(argv[argc - 1], &state->source, false);
  if ( file_size < 0L ) {
    perror("File read failed");
    exit(EX_NOINPUT);
  }
  /* Remember to free() memory allocated by slurp() */
  
  // load program source into memory, convert chars to bytes
  uint8_t * programBytes = hexStringToBytes(state->source);
  // pad program by two bytes
  memcpy(state->DMEM + 2, programBytes, strlen(state->source) / 2 + 1);
  
  // begin the main execution loop
  do {
    state->steps++;
    
    int valA;
    int valB;
    int valC;
    int valE;
    int valP;
    int valM;
    bool Cnd; // condition code conditions
    
    // 1. Fetch:
    //     - Instruction bytes are read from memory based on Program Counter (P.C.)
    //     - First byte is two four-bit values: instruction code (icode) and function code (ifun)
    //     - One or more register operands are loaded (rA and rB)
    //     - May load a 32-bit constant valC
    //     - Next program instruction address is computed, called `valP`
    
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

    // Fetch will be performed here, and a switch case will pass execution to a specialized function.
    
    // Get the instruction code and instruction function
    // these are nibbles, but we have to use byte-sized variables
    uint8_t icode = (state->DMEM[state->PC]>>4) & 0xF;
    uint8_t ifun  = state->DMEM[state->PC] & 0xF;
    
    // These fetch operations are performed frequently,
    // so just do them every time.
    uint8_t rA = (state->DMEM[state->PC + 1]>>4) & 0xF;
    uint8_t rB = state->DMEM[state->PC + 1] & 0xF;

    // TODO verbosity
    // printf("icode = %#03x\n ifun = %#03x\n", icode, ifun);
    
    // perform operation specified by instruction code
    switch(icode) {
      case 0x0:
        // halt
        logprintf(LOG_DEBUG, "halt\n");
        state->STAT = STAT_HLT;
        break;
      case 0x1:
        // nop
        logprintf(LOG_DEBUG, "nop\n");
        state->PC += 1;
        break;
      case 0x2:
        // rrmovl
        logprintf(LOG_DEBUG, "rrmovl  %s, %s\n", registerString(rA), registerString(rB));
        // fetch
        valP = state->PC + 2;
        // decode
        valA = state->registers[rA];
        // execute
        valE = valA;
        // writeback
        state->registers[rB] = valE;
        // PC update
        state->PC = valP;
        break;
      case 0x3:
        // irmovl
        logprintf(LOG_DEBUG, "irmovl  $%d, %s\n", littleEndianBytesToInt(&state->DMEM[state->PC + 2]), registerString(rB));
        // fetch
        valC = littleEndianBytesToInt(&state->DMEM[state->PC + 2]);
        valP = state->PC + 6;
        // execute
        valE = valC;
        // writeback
        state->registers[rB] = valE;
        // PC update
        state->PC = valP;
        break;
      case 0x4:
        // rmmovl
        logprintf(LOG_DEBUG, "rmmovl  %s, $%d(%s)\n", registerString(rA), littleEndianBytesToInt(&state->DMEM[state->PC + 2]), registerString(rB));
        // fetch
        valC = littleEndianBytesToInt(&state->DMEM[state->PC + 2]);
        valP = state->PC + 6;
        // decode
        valA = state->registers[rA];
        valB = state->registers[rB];
        // execute
        valE = valB + valC;
        // memory
        state->DMEM[valE] = valA;
        // PC update
        state->PC = valP;
        break;
      case 0x5:
        // mrmovl
        // note: order of operands is "backwards" in the source
        logprintf(LOG_DEBUG, "mrmovl  $%d(%s), %s\n", littleEndianBytesToInt(&state->DMEM[state->PC + 2]), registerString(rB), registerString(rA));
        // fetch
        valC = littleEndianBytesToInt(&state->DMEM[state->PC + 2]);
        valP = state->PC + 6;
        // decode
        valB = state->registers[rB];
        // execute
        valE = valB + valC;
        // memory
        valM = state->DMEM[valE];
        // writeback
        state->registers[rA] = valM;
        // PC update
        state->PC = valP;
        break;
      case 0x6:
        // ALU OPl
        // fetch
        valP = state->PC + 2;
        // decode
        valA = state->registers[rA];
        valB = state->registers[rB];
        // execute
        switch (ifun) {
          case 0x0:
            // addl
            logprintf(LOG_DEBUG, "addl    %s, %s\n", registerString(rA), registerString(rB));
            valE = state->registers[rB] + state->registers[rA];
            break;
          case 0x1:
            // subl
            logprintf(LOG_DEBUG, "subl    %s, %s\n", registerString(rA), registerString(rB));
            valE = state->registers[rB] - state->registers[rA];
            break;
          case 0x2:
            // andl
            logprintf(LOG_DEBUG, "andl    %s, %s\n", registerString(rA), registerString(rB));
            valE = state->registers[rB] & state->registers[rA];
            break;
          case 0x3:
            // xorl
            logprintf(LOG_DEBUG, "xorl    %s, %s\n", registerString(rA), registerString(rB));
            valE = state->registers[rB] ^ state->registers[rA];
            break;
          // there is room in the encoding here for an additional 12 ALU operations
            
          default:
            state->STAT = STAT_INS;
            break;
        }
        // set CC
        // overflow (true when sign of result not equal to sign of equal operands)
        state->OF = ( (valA > 0) && (valB > 0) ) != (valE > 0);
        // sign (true for negatives)
        state->SF = (valE < 0);
        // zero (true for zero)
        state->ZF = (valE == 0);
        
        // writeback
        state->registers[rB] = valE;
        // PC update
        state->PC = valP;
        break;
      case 0x7:
        // jump
        // fetch
        logprintf(LOG_DEBUG, "jmp%d    $%d\n", ifun, littleEndianBytesToInt(&state->DMEM[state->PC + 1]));
        valC = littleEndianBytesToInt(&state->DMEM[state->PC + 1]);
        valP = state->PC + 5;
        // execute
        Cnd = evaluateConditionCodes(ifun);
        // PC update
        state->PC = Cnd ? valC : valP;
        break;
      case 0x8:
        // call
        logprintf(LOG_DEBUG, "call    $%d\n", littleEndianBytesToInt(&state->DMEM[state->PC + 1]));
        // fetch
        valC = littleEndianBytesToInt(&state->DMEM[state->PC + 1]);
        valP = state->PC + 5;
        // decode
        valB = state->registers[REG_ESP];
        // execute
        valE = valB - 4;
        // memory
        state->DMEM[valE] = valP;
        // writeback
        state->registers[REG_ESP] = valE;
        // PC update
        state->PC = valC;
        break;
      case 0x9:
        // ret
        logprintf(LOG_DEBUG, "ret\n");
        // fetch
        valP = state->PC + 1;
        // decode
        valA = state->registers[REG_ESP];
        valB = state->registers[REG_ESP];
        // execute
        valE = valB + 4;
        // memory
        valM = state->DMEM[valA];
        // writeback
        state->registers[REG_ESP] = valE;
        // PC update
        state->PC = valM;
        break;
      case 0xA:
        // pushl
        logprintf(LOG_DEBUG, "pushl   %s\n", registerString(rA));
        // fetch
        valP = state->PC + 2;
        // decode
        valA = state->registers[rA];
        valB = state->registers[REG_ESP];
        // execute
        valE = valB - 4;
        // memory
        state->DMEM[valE] = valA;
        // writeback
        state->registers[REG_ESP] = valE;
        // PC update
        state->PC += 2;
        break;
      case 0xB:
        // popl
        logprintf(LOG_DEBUG, "popl    %s\n", registerString(rA));
        // fetch
        valP = state->PC + 2;
        // decode
        valA = state->registers[REG_ESP];
        valB = state->registers[REG_ESP];
        // execute
        valE = valB + 4;
        // memory
        valM = state->DMEM[valA];
        // writeback
        state->registers[REG_ESP] = valE;
        state->registers[rA] = valM;
        // PC update
        state->PC = valP;
        break;
      // there is room in the encoding here for 4 new categories of operations
      default:
        // invalid instruction
        state->STAT = STAT_INS;
        break;
    }
    
    // until we run out of program, exceed the specified max steps, or abort
  } while (state->PC < file_size
           && !(!config->maxSteps != !(state->steps < config->maxSteps))
           && state->STAT == STAT_AOK);

  printf("Steps: %lu\n", state->steps);
  printf("PC: %#010x\n", state->PC);
  printf("Status: %s\n", statusString(state->STAT));
  printf("CZ: %d\n", (int)state->ZF);
  printf("CS: %d\n", (int)state->SF);
  printf("CO: %d\n", (int)state->OF);
  printf("%%eax: %#010x\n", state->registers[0]);
  printf("%%ecx: %#010x\n", state->registers[1]);
  printf("%%edx: %#010x\n", state->registers[2]);
  printf("%%ebx: %#010x\n", state->registers[3]);
  printf("%%esp: %#010x\n", state->registers[4]);
  printf("%%ebp: %#010x\n", state->registers[5]);
  printf("%%esi: %#010x\n", state->registers[6]);
  printf("%%edi: %#010x\n", state->registers[7]);

  //  while (state->stack->top) {
//    printf("%lu\n", pop(state->stack));
//  }
  
  return 0;
}
