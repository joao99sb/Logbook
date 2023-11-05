refs: https://www.gnu.org/software/libc/manual/html_node/I_002fO-Overview.html

# Introduction

For historical reasons, the type of the C data structure that represents a stream is called FILE rather than “stream”. Since most of the library functions deal with objects of type FILE \*, sometimes the term file pointer is also used to mean “stream”. This leads to unfortunate confusion over terminology in many books on C. This manual, however, is careful to use the terms “file” and “stream” only in the technical sense.

The FILE type is declared in the header file stdio.h.

Data Type: FILE
This is the data type used to represent stream objects. A FILE object holds all of the internal state information about the connection to the associated file, including such things as the file position indicator and buffering information. Each stream also has error and end-of-file status indicators that can be tested with the ferror and feof functions; 

FILE objects are allocated and managed internally by the input/output library functions. Don’t try to create your own objects of type FILE; let the library do it. Your programs should deal only with pointers to these objects (that is, FILE \* values) rather than the objects themselves.

# Type FILE

type FILE is a type of structure called \_IO_FILE wich is defined like this:

```C
 /* The tag name of this struct is _IO_FILE to preserve historic
  C++ mangled names for functions taking FILE* arguments.
  That name should not be used in new code.  */
struct _IO_FILE
{
 int _flags;		/* High-order word is _IO_MAGIC; rest is flags. */

 /* The following pointers correspond to the C++ streambuf protocol. */
 char *_IO_read_ptr;	/* Current read pointer */
 char *_IO_read_end;	/* End of get area. */
 char *_IO_read_base;	/* Start of putback+get area. */
 char *_IO_write_base;	/* Start of put area. */
 char *_IO_write_ptr;	/* Current put pointer. */
 char *_IO_write_end;	/* End of put area. */
 char *_IO_buf_base;	/* Start of reserve area. */
 char *_IO_buf_end;	/* End of reserve area. */

 /* The following fields are used to support backing up and undo. */
 char *_IO_save_base; /* Pointer to start of non-current get area. */
 char *_IO_backup_base;  /* Pointer to first valid character of backup area */
 char *_IO_save_end; /* Pointer to end of non-current get area. */

 struct _IO_marker *_markers;

 struct _IO_FILE *_chain;

 int _fileno;
 int _flags2;
 __off_t _old_offset; /* This used to be _offset but it's too small.  */

 /* 1+column number of pbase(); 0 is unknown. */
 unsigned short _cur_column;
 signed char _vtable_offset;
 char _shortbuf[1];

 _IO_lock_t *_lock;

 __off64_t _offset;
 /* Wide character stream stuff.  */
 struct _IO_codecvt *_codecvt;
 struct _IO_wide_data *_wide_data;
 struct _IO_FILE *_freeres_list;
 void *_freeres_buf;
 size_t __pad5;
 int _mode;
 /* Make sure we don't get into trouble again.  */
 char _unused2[15 * sizeof (int) - 4 * sizeof (void *) - sizeof (size_t)];
};
```
