//
//  arithmeticLogic.c
//  y86vm
//
//  Created by Dakota Schneider on 4/19/15.
//  Copyright (c) 2015 Code From Above, LLC. All rights reserved.
//

#include <stdbool.h>

#include "arithmeticLogic.h"
#include "environment.h"

extern VirtualMachineState * state;

void OPl(char ifun, int rA, int rB) {
  // TODO validation, flags
  
  long int valA = state->registers[rA];
  long int valB = state->registers[rB];
  long int result = 0;
  
  switch (ifun) {
    case '0':
      // addl
      result = state->registers[rB] + state->registers[rA];
      break;
    case '1':
      // subl
      result = state->registers[rB] - state->registers[rA];
      break;
    case '2':
      // andl
      result = state->registers[rB] & state->registers[rA];
      break;
    case '3':
      // xorl
      result = state->registers[rB] ^ state->registers[rA];
      break;
      
    default:
      state->STAT = STAT_INS;
      break;
  }
  
  // overflow (true when sign of result not equal to sign of equal operands)
  state->OF = ( (valA > 0) && (valB > 0) ) != (result > 0);
  // sign (true for negatives)
  state->SF = (result < 0);
  // zero (true for zero)
  state->ZF = (result == 0);
  
  state->registers[rB] = result;
}

bool Cond(uint8_t ifun) {
  switch (ifun) {
    case COND_NONE:
      return true;
    case COND_LTE:
      return (bool)(state->ZF || state->SF);
    case COND_LT:
      return (bool)(state->SF);
    case COND_EQ:
      return (bool)(state->ZF);
    case COND_NE:
      return (bool)(!state->ZF);
    case COND_GTE:
      return (bool)(!state->SF || state->ZF);
    case COND_GT:
      return (bool)(!state->SF);
    
    default:
      return false;
  }
}
