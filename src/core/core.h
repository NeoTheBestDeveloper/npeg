#ifndef H_CORE
#define H_CORE

#include "algorithms/algorithm.h"

void process_image(const char *input_path, const char *output_path,
                   const Algorithm *algorithms, u32 algorithms_count);

#endif // !H_CORE
