//
//  stack.h
//  y86vm
//
//  Created by Dakota Schneider on 4/19/15.
//  Copyright (c) 2015 Code From Above, LLC. All rights reserved.
//

#ifndef __y86vm__stack__
#define __y86vm__stack__

#include <stdio.h>

#include "environment.h"

Stack * newStack();

void pushl(int val);

void popl(int * reg);

// TODO: hide these
StackNode * push(Stack * stack, int val);
int pop (Stack * stack);


#endif /* defined(__y86vm__stack__) */
