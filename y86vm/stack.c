//
//  stack.c
//  y86vm
//
//  Created by Dakota Schneider on 4/19/15.
//  Copyright (c) 2015 Code From Above, LLC. All rights reserved.
//

#include "stack.h"

#include <stdlib.h>

#include "environment.h"


// TODO: hide these
// static stackNode * push(stackNode * stack, int val);
// static int pop (stackNode * stack);

// void pushl(int val) {
//     push(stack, val);
// }

// void popl(int * reg) {
//     if (stack) {
//         *reg = pop(stack);
//     } else {
//         // nothing to pop, invalid instruction
//         STAT = STAT_INS;
//     }
// }


Stack * newStack() {
  Stack * stack= malloc(sizeof(StackNode));
  if (!stack) return NULL;
  stack->size = 0;
  stack->top = NULL;
  return stack;
}

StackNode * push(Stack * stack, int val) {
  StackNode * node = malloc(sizeof(StackNode));
  if (!node) return NULL;
  node->val = val;
  node->prev = stack->top;
  stack->top = node;
  stack->size++;
  return node;
}

int pop(Stack * stack) {
  int val = stack->top->val;
  StackNode * prev = stack->top->prev;
  free(stack->top);
  stack->top = prev;
  stack->size--;
  return val;
}

