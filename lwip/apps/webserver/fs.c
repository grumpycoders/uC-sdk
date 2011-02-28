/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#include "lwip/opt.h"
#include "lwip/def.h"
#include "fs.h"
#include <string.h>
#include <hash-djb2.h>
#include <romfs.h>

/*-----------------------------------------------------------------------------------*/
/* Define the number of open files that we can support. */
#ifndef LWIP_MAX_OPEN_FILES
#define LWIP_MAX_OPEN_FILES     10
#endif

static const u8_t * webfs_romfs = NULL;

/* Define the file system memory allocation structure. */
struct webfs_table {
  struct webfs_file file;
  u8_t inuse;
};

/* Allocate file system memory */
struct webfs_table webfs_memory[LWIP_MAX_OPEN_FILES];

int webfs_open_custom(struct webfs_file *file, const char *name) {
    uint32_t len, h;
    const uint8_t * buf;
    if (*name == '/')
        name++;
    h = hash_djb2((const uint8_t *) name, -1);
    buf = romfs_get_file_by_hash(webfs_romfs, h, &len);
    if (!buf)
        return 0;
    
    file->data = (const char *) buf;
    file->len = len;
    file->index = 0;
    file->pextension = NULL;
    file->http_header_included = 0;
    
    return 1;
}

void webfs_close_custom(struct webfs_file *file) { }

/*-----------------------------------------------------------------------------------*/
static struct webfs_file *
webfs_malloc(void)
{
  int i;
  for(i = 0; i < LWIP_MAX_OPEN_FILES; i++) {
    if(webfs_memory[i].inuse == 0) {
      webfs_memory[i].inuse = 1;
      return(&webfs_memory[i].file);
    }
  }
  return(NULL);
}

/*-----------------------------------------------------------------------------------*/
static void
webfs_free(struct webfs_file *file)
{
  int i;
  for(i = 0; i < LWIP_MAX_OPEN_FILES; i++) {
    if(&webfs_memory[i].file == file) {
      webfs_memory[i].inuse = 0;
      break;
    }
  }
  return;
}

/*-----------------------------------------------------------------------------------*/
struct webfs_file *
webfs_open(const char *name)
{
  struct webfs_file *file;

  file = webfs_malloc();
  if(file == NULL) {
    return NULL;
  }

  if(webfs_open_custom(file, name)) {
    return file;
  }

  webfs_free(file);
  return NULL;
}

/*-----------------------------------------------------------------------------------*/
void
webfs_close(struct webfs_file *file)
{
  webfs_free(file);
}
/*-----------------------------------------------------------------------------------*/
int
webfs_read(struct webfs_file *file, char *buffer, int count)
{
  int read;

  if(file->index == file->len) {
    return -1;
  }

  read = file->len - file->index;
  if(read > count) {
    read = count;
  }

  MEMCPY(buffer, (file->data + file->index), read);
  file->index += read;

  return(read);
}
/*-----------------------------------------------------------------------------------*/
int webfs_bytes_left(struct webfs_file *file)
{
  return file->len - file->index;
}
/*-----------------------------------------------------------------------------------*/
void webfs_init(const u8_t *romfs)
{
  webfs_romfs = romfs;
}
