//
//  arithmeticLogic.c
//  y86vm
//
//  Created by Dakota Schneider on 4/19/15.
//  Copyright (c) 2015 Code From Above, LLC. All rights reserved.
//

#include "arithmeticLogic.h"
#include "environment.h"

extern VirtualMachineState * state;

void OPl(char ifun, int rA, int rB) {
  // TODO validation, flags
  switch (ifun) {
    case '0':
      // addl
      state->registers[rB] += state->registers[rA];
      break;
    case '1':
      // subl
      state->registers[rB] -= state->registers[rA];
      break;
    case '2':
      // andl
      state->registers[rB] &= state->registers[rA];
      break;
    case '3':
      // xorl
      state->registers[rB] ^= state->registers[rA];
      break;
      
    default:
      state->STAT = STAT_INS;
      break;
  }
}
