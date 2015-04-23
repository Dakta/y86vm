//
//  utils.c
//  y86vm
//
//  Created by Dakota Schneider on 4/19/15.
//  Copyright (c) 2015 Code From Above, LLC. All rights reserved.
//

#include "utils.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sysexits.h>

#include "environment.h"
#include "hexString.h"

/**
 * strpos()
 * equivalent to common indexOf(): returns first position of char
 * in search string, -1 when not found.
**/
int strpos(char * string, char find) {
  char * found = strchr(string, find);
  if (found) {
    return (int)(found - string);
  } else {
    return -1;
  }
}

char * statusString(uint8_t stat) {
  // status codes
  char * status = malloc(sizeof(char)*4);
  switch (stat) {
    case STAT_AOK:
      status = "AOK";
      break;
    case STAT_HLT:
      status = "HLT";
      break;
    case STAT_ADR:
      status = "ADR";
      break;
    case STAT_INS:
      status = "INS";
      break;
    default:
      printf("Unknown status code %d", stat);
      exit(EXIT_FAILURE);
  }
  return status;
}

char * registerString(uint8_t stat) {
  // status codes
  char * reg = malloc(sizeof(char)*5);
  switch (stat) {
    case REG_EAX:
      reg = "%eax";
      break;
    case REG_ECX:
      reg = "%ecx";
      break;
    case REG_EDX:
      reg = "%edx";
      break;
    case REG_EBX:
      reg = "%ebx";
      break;
    case REG_ESP:
      reg = "%esp";
      break;
    case REG_EBP:
      reg = "%ebp";
      break;
    case REG_ESI:
      reg = "%esi";
      break;
    case REG_EDI:
      reg = "%edi";
      break;
    case REG_NONE:
      reg = "NONE";
      break;
    default:
      printf("Unknown register code %d", stat);
      exit(EXIT_FAILURE);
  }
  return reg;
}


char * littleToBigEndianChars(char * littleEndian) {
  int size = sizeof(littleEndian);
  char * bigEndian = malloc(size);
  size = size / 2;
  for (int i = 0; i < size; i++) {
    bigEndian[((size - 1) - i) * 2] = littleEndian[(i * 2)];
    bigEndian[(((size - 1) - i) * 2) + 1] = littleEndian[(i * 2) + 1];
  }
  return bigEndian;
}

int littleEndianBytesToInt(uint8_t * littleEndian) {
  int retval;
  uint8_t * bigEndian = (uint8_t *)&retval;

  // X86 ints are little-endian, so we can just copy the bytes
  for (int i = 0; i < 4; i++) {
    bigEndian[i] = littleEndian[i];
  }
  
  return retval;
}

long int bigEndianCharArrayToInt(char * bigEndian) {
  char * endptr;
  long int number = strtol(bigEndian, &endptr, 16);
  
  if (endptr == bigEndian) {
    /* no conversion was performed */
    // failed, should throw a STAT_INS
  } else if (*endptr == '\0') {
    /* the entire string was converted */
    // success
  } else {
    /* the unconverted rest of the string starts at endptr */
    // partial failure, throw a STAT_INS
    perror("Invalid integer argument to -l/--limit");
  }
  return number;
}


/*
 * 'slurp' reads the file identified by 'path' into a character buffer
 * pointed at by 'buf', optionally adding a terminating NUL if
 * 'add_nul' is true. On success, the size of the file is returned; on
 * failure, -1 is returned and ERRNO is set by the underlying system
 * or library call that failed.
 *
 * WARNING: 'slurp' malloc()s memory to '*buf' which must be freed by
 * the caller.
 */
long slurp(char const* path, char **buf, bool add_nul)
{
  FILE  *fp;
  size_t fsz;
  long   off_end;
  int    rc;
  
  /* Open the file */
  fp = fopen(path, "r");
  if( NULL == fp ) {
    return -1L;
  }
  
  /* Seek to the end of the file */
  rc = fseek(fp, 0L, SEEK_END);
  if( 0 != rc ) {
    return -1L;
  }
  
  /* Byte offset to the end of the file (size) */
  if( 0 > (off_end = ftell(fp)) ) {
    return -1L;
  }
  fsz = (size_t)off_end;
  
  /* Allocate a buffer to hold the whole file */
  *buf = malloc( fsz+(int)add_nul );
  if( NULL == *buf ) {
    return -1L;
  }
  
  /* Rewind file pointer to start of file */
  rewind(fp);
  
  /* Slurp file into buffer */
  if( fsz != fread(*buf, 1, fsz, fp) ) {
    free(*buf);
    return -1L;
  }
  
  /* Close the file */
  if( EOF == fclose(fp) ) {
    free(*buf);
    return -1L;
  }
  
  if( add_nul ) {
    /* Make sure the buffer is NUL-terminated, just in case */
    *buf[fsz] = '\0';
  }
  
  /* Return the file size */
  return (long)fsz;
}


