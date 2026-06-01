#include "stddef.h"
#include "stdlib.h"
#include "time/timer.h"

const timer_config_t default_timer_config = {};

struct Timer
{
  void *unused;
};

struct Timer *timer_create(const timer_config_t *config)
{
  struct Timer *result = malloc(sizeof(struct Timer));
  if (!result)
  {
    return NULL;
  }
  return result;
}

void timer_start(struct Timer *timer, duration_t duration,
                 timer_callback_t callback, void *context)
{
  callback(timer, context);
}

void timer_delay_for(struct Timer *timer, duration_t duration)
{
  // nop
}

void timer_destroy(struct Timer *timer)
{
  if (!timer)
  {
    return;
  }
  free(timer);
}
