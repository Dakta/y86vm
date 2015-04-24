//
//  memoryManager.h
//  y86vm
//
//  Created by Dakota Schneider on 5/2/15.
//  Copyright (c) 2015 Code From Above, LLC. All rights reserved.
//

#ifndef __y86vm__memoryManager__
#define __y86vm__memoryManager__

#include <stdio.h>
#include <stdint.h>

uint8_t getByte(int address);

int getInt(int address);

void writeByte(int address, uint8_t byte);

void writeInt(int address, int value);


#endif /* defined(__y86vm__memoryManager__) */
