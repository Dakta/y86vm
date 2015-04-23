//
//  arithmeticLogic.h
//  y86vm
//
//  Created by Dakota Schneider on 4/19/15.
//  Copyright (c) 2015 Code From Above, LLC. All rights reserved.
//

#ifndef __y86vm__arithmeticLogic__
#define __y86vm__arithmeticLogic__

#include <stdio.h>

void OPl(char ifun, int rA, int rB);

bool evaluateConditionCodes(uint8_t ifun);


#endif /* defined(__y86vm__arithmeticLogic__) */
