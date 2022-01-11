/* $Id$ */
/* Copyright (c) 2010-2015 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Desktop Auditor */
/* All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */



#ifndef AUDITOR_TASK_H
# define AUDITOR_TASK_H

# include <time.h>


/* Task */
/* types */
typedef struct _Task Task;


/* functions */
Task * task_new(void);
Task * task_new_from_file(char const * filename);
void task_delete(Task * task);


/* accessors */
char const * task_get_description(Task * task);
int task_get_done(Task * task);
time_t task_get_end(Task * task);
char const * task_get_filename(Task * task);
char const * task_get_priority(Task * task);
time_t task_get_start(Task * task);
char const * task_get_title(Task * task);

int task_set_description(Task * task, char const * description);
int task_set_done(Task * task, int done);
int task_set_end(Task * task, time_t end);
int task_set_filename(Task * task, char const * filename);
int task_set_priority(Task * task, char const * priority);
int task_set_start(Task * task, time_t start);
int task_set_title(Task * task, char const * title);


/* useful */
int task_load(Task * task);
int task_save(Task * task);
int task_unlink(Task * task);

#endif /* !AUDITOR_TASK_H */
