// ------------------------- LIBRARIES -------------------------
#include <assert.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// We've to compile with flags `pkg-config --cflags --libs glib-2.0`

#include "command.h"
#include "strextra.h"
// Needed library for the queue defined structure (GQueue)

// ------------------------- SCOMMAND TAD -------------------------

// ----------- STRUCTURE --------------

struct scommand_s {
  GQueue* sc_arguments;
  char* sc_file_input;
  char* sc_file_output;
};

// ----------- FUNCTIONS --------------

scommand scommand_new(void)
{
  // Allocating memory for the structure.
  scommand new_sc = malloc(sizeof(struct scommand_s));
  new_sc->sc_arguments = g_queue_new();
  new_sc->sc_file_input = NULL;
  new_sc->sc_file_output = NULL;

  assert(new_sc != NULL && scommand_is_empty(new_sc) && scommand_get_redir_in(new_sc) == NULL && scommand_get_redir_out(new_sc) == NULL);
  return new_sc;
}

scommand scommand_destroy(scommand self)
{
  assert(self != NULL);

  g_queue_free_full(self->sc_arguments, free);
  if (self->sc_file_input != NULL) {
    free(self->sc_file_input);
    self->sc_file_input = NULL;
  }
  if (self->sc_file_output != NULL) {
    free(self->sc_file_output);
    self->sc_file_output = NULL;
  }
  free(self);
  self = NULL;

  assert(self == NULL);
  return self;
}

void scommand_push_back(scommand self, char* argument)
{
  assert(self != NULL && argument != NULL);

  // Push the argument to the end of the queue.
  g_queue_push_tail(self->sc_arguments, argument);

  assert(!scommand_is_empty(self));
}

void scommand_pop_front(scommand self)
{
  assert(self != NULL && !scommand_is_empty(self));

  // removed_argument points to the first element in the queue.
  char* removed_argument = g_queue_pop_head(self->sc_arguments);

  // Removing memory allocated for removed_argument since it's a useless variable.
  free(removed_argument);
  removed_argument = NULL;
}

void scommand_set_redir_in(scommand self, char* filename)
{
  assert(self != NULL);

  // Set input dir.
  self->sc_file_input = filename;
}

void scommand_set_redir_out(scommand self, char* filename)
{
  assert(self != NULL);

  // Set output dir.
  self->sc_file_output = filename;
}

bool scommand_is_empty(const scommand self)
{
  assert(self != NULL);

  return g_queue_is_empty(self->sc_arguments);
}

unsigned int scommand_length(const scommand self)
{
  assert(self != NULL);

  unsigned int sc_args_length = g_queue_get_length(self->sc_arguments);

  assert((sc_args_length == 0) == scommand_is_empty(self));
  return sc_args_length;
}

char* scommand_front(const scommand self)
{
  assert(self != NULL && !scommand_is_empty(self));

  // head element is stored in sc_front.
  char* sc_arg_front = g_queue_peek_head(self->sc_arguments);

  assert(sc_arg_front != NULL);
  return sc_arg_front;
}

char* scommand_get_redir_in(const scommand self)
{
  assert(self != NULL);

  return self->sc_file_input;
}

char* scommand_get_redir_out(const scommand self)
{
  assert(self != NULL);

  return self->sc_file_output;
}

// ----------- FUNCTION TO DEBUG --------------

char* scommand_to_string(const scommand self)
{
  assert(self != NULL);

  // sc_shell_representation is the output of scommand_to_string.
  // Not NULL since we can't concatenate a string with NULL.
  char* sc_shell_representation = "";

  unsigned int sc_args_length = g_queue_get_length(self->sc_arguments);
  if (sc_args_length != 0) {
    sc_shell_representation = g_queue_peek_head(self->sc_arguments);

    // auxiliar_to_remove is used for the deletion of allocated memory blocks in previous strmerge calls.
    for (unsigned int i = 1; i < sc_args_length; i++) {
      char* auxiliar_to_remove = sc_shell_representation;
      sc_shell_representation = strmerge(sc_shell_representation, " ");
      if (i != 1) {
	free(auxiliar_to_remove);
      }

      auxiliar_to_remove = sc_shell_representation;
      sc_shell_representation = strmerge(sc_shell_representation, g_queue_peek_nth(self->sc_arguments, i));
      free(auxiliar_to_remove);
    }
  }

  // Add argument in the in redir field.
  if (self->sc_file_input != NULL) {
    char* auxiliar_to_remove = sc_shell_representation;
    sc_shell_representation = strmerge(sc_shell_representation, " < ");
    free(auxiliar_to_remove);

    auxiliar_to_remove = sc_shell_representation;
    sc_shell_representation = strmerge(sc_shell_representation, self->sc_file_input);
    free(auxiliar_to_remove);
  }

  // Add argument in the out redir field.
  if (self->sc_file_output != NULL) {
    char* auxiliar_to_remove = sc_shell_representation;
    sc_shell_representation = strmerge(sc_shell_representation, " > ");
    free(auxiliar_to_remove);

    auxiliar_to_remove = sc_shell_representation;
    sc_shell_representation = strmerge(sc_shell_representation, self->sc_file_output);
    free(auxiliar_to_remove);
  }

  assert(scommand_get_redir_in(self) == NULL || scommand_get_redir_out(self) == NULL || strlen(sc_shell_representation) > 0);
  return sc_shell_representation;
}

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
  assert(self != NULL && !pipeline_is_empty(self));
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

char* pipeline_to_string(const pipeline self)
{
  assert(self != NULL);
  char* result;
  result = "";
  for (unsigned int i = 0u; i < pipeline_length(self); ++i) {
    strcat(result, g_queue_peek_nth(self->commands, i));
    if (i < pipeline_length(self) - 1) {
      strcat(result, " | ");
    }
  }
  assert(pipeline_is_empty(self) || pipeline_get_wait(self) || strlen(result) > 0);
  return result;
}
