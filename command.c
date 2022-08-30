#include "command.h"
#include <assert.h>
#include <glib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct pipeline_s {
  GQueue∗ commands;
  bool should_wait;
} pipeline_s;

pipeline pipeline_new(void)
{
  pipeline self;
  self = malloc(sizeof(pipeline_s));
  self->commands = NULL;
  self->should_wait = true;

  // Didn't put predicate members inside variables to prevent useless computation when deactivating assertions
  assert(self != NULL && pipeline_is_empty(self) && pipeline_get_wait(self));
  return self;
}

pipeline pipeline_destroy(pipeline self)
{
  assert(self != NULL);
  g_queue_free_full(self->commands, scommand_destroy); // free queue using second arg to destroy each element
  self->commands = NULL;
  free(self);
  self = NULL;
  assert(self == NULL);
  return self;
}

void pipeline_push_back(pipeline self, scommand sc)
{
  assert(self != NULL && sc != NULL);
  g_queue_push_tail(self->commands, sc); // Send sc to end of Queue
  assert(!pipeline_is_empty(self));
}

void pipeline_pop_front(pipeline self)
{
  assert(self != NULL && !pipeline_is_empty(self));
  g_queue_pop_head(self->commands); // Remove first element from Queue
}

void pipeline_set_wait(pipeline self, const bool w)
{
  assert(self != NULL);
  self->should_wait = w;
}

bool pipeline_is_empty(const pipeline self)
{
  assert(self != NULL);
  return g_queue_is_empty(self->commands);
}

unsigned int pipeline_length(const pipeline self)
{
  assert(self != NULL);
  unsigned int result;
  result = g_queue_get_length(self->commands);
  assert((result == 0) == pipeline_is_empty(self));
  return result;
}

scommand pipeline_front(const pipeline self)
{
  assert(self != NULL && ! pipeline_is_empty(self));
  scommand result;
  result = g_queue_peek_head(self->commands); // Returns the first element of the queue.
  assert(result != NULL);
  return result;
}

bool pipeline_get_wait(const pipeline self)
{
  assert(self != NULL);
  return self->should_wait;
}

char * pipeline_to_string(const pipeline self)
{
  assert(self != NULL);
  char * result;
  result = "";
  for(unsigned int i = 0u; i < pipeline_length(self); ++i)
  {
    strcat(result, g_queue_peek_nth(self->commands,i));
    if( i < pipeline_length(self) - 1 )
    {
      strcat(result, " | ");
    }
  }
  assert(pipeline_is_empty(self) || pipeline_get_wait(self) || strlen(result)>0);
  return result;
}