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


#define LOG_OFF    0
#define LOG_FATAL  1
#define LOG_ERROR  2
#define LOG_WARN   3
#define LOG_INFO   4
#define LOG_DEBUG  5
#define LOG_TRACE  6


int strpos(char * string, char find);

char * statusString(uint8_t stat);

char * registerString(uint8_t stat);

void logprintf(int level, const char *format, ...);


long slurp(char const* path, char **buf, bool add_nul);

char * littleToBigEndianChars(char * littleEndian);

int littleEndianBytesToInt(uint8_t * littleEndian);

long int bigEndianCharArrayToInt(char * bigEndian);


#endif /* defined(__y86vm__utils__) */
