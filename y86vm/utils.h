//
//  utils.h
//  y86vm
//
//  Created by Dakota Schneider on 4/19/15.
//  Copyright (c) 2015 Code From Above, LLC. All rights reserved.
//

#ifndef __y86vm__utils__
#define __y86vm__utils__

#include <stdio.h>
#include <stdbool.h>

int strpos(char * string, char find);

char * statusString(uint8_t stat);

char * registerString(uint8_t stat);

long slurp(char const* path, char **buf, bool add_nul);

char * littleToBigEndianChars(char * littleEndian);

int littleEndianBytesToInt(uint8_t * littleEndian);

long int bigEndianCharArrayToInt(char * bigEndian);


#endif /* defined(__y86vm__utils__) */
