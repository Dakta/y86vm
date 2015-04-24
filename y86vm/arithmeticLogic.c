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


bool evaluateConditionCodes(uint8_t ifun) {
  // condition reference: http://acm.hit.edu.cn/hoj/static/img/pic/100603-B.htm
  switch (ifun) {
    case COND_NONE:
      return true;
    case COND_LTE:
      return (bool)((state->SF != state->OF) || state->ZF);
    case COND_LT:
      return (bool)(state->SF != state->OF);
    case COND_EQ:
      return (bool)(state->ZF);
    case COND_NE:
      return (bool)(!state->ZF);
    case COND_GTE:
      return (bool)(!(state->SF != state->OF));
    case COND_GT:
      return (bool)(!(state->SF != state->OF) && !state->ZF);
    
    default:
      return false;
  }
}
