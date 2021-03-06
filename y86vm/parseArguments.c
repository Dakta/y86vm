//
//  parseArguments.c
//  y86vm
//
//  Created by Dakota Schneider on 4/19/15.
//  Copyright (c) 2015 Code From Above, LLC. All rights reserved.
//

// booleans are nice
#include <stdbool.h>
// and so are named exit codes: http://stackoverflow.com/a/1535733/362042
#include <sysexits.h>
// strtoul() http://stackoverflow.com/a/7021750/362042
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "environment.h"
#include "utils.h"

#include "parseArguments.h"

static void printUsage();

static void printUsage() {
  printf("Usage: ./y86vm [-h|--help] [-V|--version] [-v|--verbose] [-l|--limit [n]] inputfile\n");
  printf("  -h/--help\n    Show this screen and exit.\n");
  printf("  -V/--version\n    Print version information and exit.\n");
  printf("  -v/--verbose\n    Print every step.\n\n");
  printf("  -f/--format (a|b)\n    Set input format. (a)scii, or raw (b)ytes. Default ASCII.\n\n");
  printf("  -l/--limit n\n    Maximum numner of steps (debugging tool to pause execution at\n");
  printf("    n steps). Unlimited when argument is unspecified, %d when argument is bare.\n\n", MAX_STEPS);
  return;
}

// handle arguments
// just a little extra, typical command-line args
Config * parseArguments(int argc, char const *argv[]) {
  Config * config = malloc(sizeof(Config));
  // set defaults
  config->maxSteps = 0;
  config->verbose = false;
  config->logLevel = LOG_OFF;
  config->inputFormat = INPUT_ASCII;
  
  for (int i=1; i<argc; i++) {
    if (strcmp(argv[i], "-h") == 0 ||
        strcmp(argv[i], "--help") == 0
    ) {
      printf(VERSION);
      printf("A bare-bones Y86 virtual machine/emulator.\n");
      
      printUsage();
      
      exit(EX_OK);
    } else if (strcmp(argv[i], "-V") == 0 ||
               strcmp(argv[i], "--version") == 0
               ) {
      printf(VERSION);
      
      exit(EX_OK);
    } else if (strcmp(argv[i], "-v") == 0 ||
               strcmp(argv[i], "--verbose") == 0
               ) {
      config->verbose = true;
      // currently we don't implement selectable loglevel, but it's in the code
      config->logLevel = LOG_DEBUG;
    } else if (strcmp(argv[i], "-f") == 0 ||
               strcmp(argv[i], "--format") == 0
               ) {
      if (i + 1 > argc - 1) {
        // the last argument is always the input file,
        // so don't ever check it
        printf("Invalid argument to -f\n");
        printUsage();
        exit(EX_USAGE);
      }
      
      const char * str = argv[i + 1];
      // only need to check first character
      switch (str[0]) {
        case INPUT_ASCII:
          config->inputFormat = INPUT_ASCII;
          break;
        case INPUT_BYTES:
          config->inputFormat = INPUT_BYTES;
          break;
        default:
          printf("Invalid argument to -f\n");
          printUsage();
          exit(EX_USAGE);
      }
    } else if (strcmp(argv[i], "-l") == 0 ||
               strcmp(argv[i], "--limit") == 0
               ) {
      config->maxSteps = MAX_STEPS;
      if (i + 1 > argc - 1) {
        // the last argument is always the input file,
        // so don't ever check it
        printf("past last\n");
        continue;
      }
      
      const char * str = argv[i + 1];
      char * endptr;
      unsigned long int limit = strtoul(str, &endptr, 0);
      
      if (endptr == str) {
        /* no conversion was performed */
        // failed, the next arg didn't start with an int
        // so assume it's another valid arg and ignore
      } else if (*endptr == '\0') {
        /* the entire string was converted */
        // success
        i++;
        config->maxSteps = limit;
      } else {
        /* the unconverted rest of the string starts at endptr */
        // partial failure, throw an error
        perror("Invalid integer argument to -l/--limit");
        printUsage();
        exit(EX_USAGE);
      }
    } else if (strpos(argv[i], '-') == 0) {
      // flags start with "-", so we use this to ignore the input file
      printf("Unknown argument: %s\n", argv[i]);
      printUsage();
      
      exit(EX_USAGE);
    }
  }
  
  return config;
}
