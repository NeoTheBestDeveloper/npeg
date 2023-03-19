#ifndef H_POST_PROCESS
#define H_POST_PROCESS

typedef enum {
  NONE_PROCESS = 0,
  BILINEAR_INTER = 1,
  LANCZOS_INTER = 2,
  AVG_INTER = 3,
  UP_DOWN_SCALE = 4,
} PostProcess;

#endif // !H_POST_PROCESS
