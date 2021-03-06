/* Copyright 1994, 1995 by Abacus Research and
 * Development, Inc.  All rights reserved.
 */

#if !defined (OMIT_RCSID_STRINGS)
char ROMlib_rcsid_slash[] =
		"$Id: slash.c 63 2004-12-24 18:19:43Z ctm $";
#endif

/*
 * The DOS file manipulation routines don't like to see "//", which is
 * a pain for us because we like the format of a file to be the filesystem
 * (volume) name, a slash, and then the rest of the path.  That means that
 * anything that is in the root directory gets a leading slash.  Stubbing
 * out these routines is much easier than rewriting the other routines.
 */

#include "rsys/common.h"
#include "rsys/lockunlock.h"

#if defined(MSDOS) || defined (CYGWIN32)

#if defined (CYGWIN32)
#include "winfs.h"
#include "win_stat.h"
#endif

#include "rsys/slash.h"

#define DOUBLE_SLASH_REMOVE(str)		\
({						\
  char *retval, *op;				\
  const char *ip;				\
  boolean_t last_was_slash_p;			\
						\
/* char *from;					\
asm ("movl %%ebp, %0" : "=g" (from));		\
from = *(char **)(from+4);  			\
warning_unexpected ("name = \"%s\" from = %p", str, from); */	\
  retval = alloca (strlen (str) + 1);		\
  last_was_slash_p = FALSE;			\
  for (ip = str, op = retval; *ip; ++ip)	\
    {						\
      if (*ip != '/' || !last_was_slash_p)	\
	*op++ = *ip;				\
      last_was_slash_p = *ip == '/';		\
    }						\
  *op = 0;					\
  retval;					\
})

int Uaccess(const char *path, int mode)
{
  path = DOUBLE_SLASH_REMOVE (path);
  return access(path, mode);
}

int Uchdir(const char *path)
{
  path = DOUBLE_SLASH_REMOVE (path);
  return chdir(path);
}

int Uchmod(const char *path, int mode)
{
  path = DOUBLE_SLASH_REMOVE (path);
  return chmod(path, mode);
}

int Uchown(const char *path, uid_t owner, gid_t group)
{
  int retval;

  path = DOUBLE_SLASH_REMOVE (path);
#if !defined (CYGWIN32)
  retval = chown(path, owner, group);
#else
  retval = 0;
#endif
  return retval;
}

int Ucreat(const char *path, int mode)
{
  path = DOUBLE_SLASH_REMOVE (path);
  return creat(path, mode);
}

FILE *Ufopen(const char *path, const char *type)
{
  path = DOUBLE_SLASH_REMOVE (path);
  return fopen(path, type);
}

int Ulink(const char *path, const char *newpath)
{
  path = DOUBLE_SLASH_REMOVE (path);
  newpath = DOUBLE_SLASH_REMOVE (newpath);
  return link(path, newpath);
}

int Umkdir(const char *path, int mode)
{
  path = DOUBLE_SLASH_REMOVE (path);
  return mkdir(path
#if !defined (CYGWIN32)
	       ,mode
#endif
	       );
}

DIR *Uopendir(const char *path)
{
  path = DOUBLE_SLASH_REMOVE (path);
  return opendir((char *) path);
}

int Urename(const char *path, const char *new)
{
  path = DOUBLE_SLASH_REMOVE (path);
  new = DOUBLE_SLASH_REMOVE (new);
  return rename(path, new);
}

int Urmdir(const char *path)
{
  path = DOUBLE_SLASH_REMOVE (path);
  return rmdir(path);
}

int Ustat(const char *path, struct stat *buf)
{
  int retval;

  path = DOUBLE_SLASH_REMOVE (path);
  retval = stat(path, buf);
  if (strlen (path) == 2 && path[1] == ':')
    {
#if defined (CYGWIN32)
      buf->st_ino = 0;
      buf->st_rdev = 1;
#else
      buf->st_ino = 1;
#endif
    }
#if defined (CYGWIN32)
  else
    {
      uint32 ino;

      ino = ino_from_name (path);
      buf->st_ino = ino >> 16; /* See definition of ST_INO */
      buf->st_rdev = ino;
    }
#endif
  return retval;
}

int Ustatfs(const char *path, struct statfs *buf)
{
  path = DOUBLE_SLASH_REMOVE (path);
  return statfs(path, buf);
}

int Uunlink(const char *path)
{
  path = DOUBLE_SLASH_REMOVE (path);
  return unlink(path);
}

int Uutimes(const char *path, struct timeval tvp[2])
{
#if 0
  path = DOUBLE_SLASH_REMOVE (path);
  return utimes(path, tvp);
#else
#warning "Blowing off utimes"
  return -1;
#endif
}

#else

#define DOUBLE_SLASH_REMOVE(path) path

#endif

int Uopen(const char *path, int flags, int mode)
{
  int retval;

  path = DOUBLE_SLASH_REMOVE (path);
  retval = open(path, flags, mode);
  ROMlib_fd_clear_locks_after_open (retval, TRUE);
  return retval;
}

int Uclose (int fd)
{
  int retval;

  ROMlib_fd_release_locks_for_close (fd);
  retval = close (fd);
  return retval;
}

