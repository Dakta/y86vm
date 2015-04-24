//
//  memoryManager.c
//  y86vm
//
//  Created by Dakota Schneider on 5/2/15.
//  Copyright (c) 2015 Code From Above, LLC. All rights reserved.
//

#include "memoryManager.h"

#include <sysexits.h>
#include <stdlib.h>
#include <stdint.h>

#include "environment.h"
#include "utils.h"

extern VirtualMachineState * state;

uint8_t getByte(int address) {
  // if we're using direct addressing
  
  // bounds checking
  if (address >= MEM_SZ) {
    // out of bounds
    exit(EX_SOFTWARE);
  }
  return state->DMEM[address];
}

int getInt(int address) {
  uint8_t result[4];
  for (int i = 0; i < 4; i++) {
    result[i] = getByte(address + i);
  }
  return littleEndianBytesToInt(result);
}

void writeByte(int address, uint8_t byte) {
  // if we're using direct addressing
  
  // bounds checking
  if (address >= MEM_SZ) {
    // out of bounds
    exit(EX_SOFTWARE);
  }
  state->DMEM[address] = byte;
}

void writeInt(int address, int value) {
  uint8_t * bytes = intToLittleEndianBytes(value);
  for (int i = 0; i < 4; i++) {
    writeByte(address + i, bytes[i]);
  }
  // nothing to return
}
