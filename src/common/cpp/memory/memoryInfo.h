//
// memory info - for windows only at the momen
//

#ifndef MEMINFO_H
#define MEMINFO_H

#include <stdlib.h>

static size_t startUsage = 0;

size_t getCurrentUsage();
size_t getPeakUsage();


#endif
