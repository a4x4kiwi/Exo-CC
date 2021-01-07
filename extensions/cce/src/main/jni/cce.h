#ifndef CCEXTRACTOR_H
#define CCEXTRACTOR_H

// Needed to avoid warning implicit declaration of function ‘asprintf’ on Linux
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#ifdef ENABLE_OCR
#include <leptonica/allheaders.h>
#endif

#include <stdio.h>
#include "lib_ccx/lib_ccx.h"
#include "lib_ccx/configuration.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <jni.h>
#include "lib_ccx/ccx_common_option.h"
#include "lib_ccx/ccx_mp4.h"
#include "lib_ccx/hardsubx.h"
#include "lib_ccx/ccx_share.h"
#ifdef WITH_LIBCURL
CURL *curl;
CURLcode res;
#endif
#if defined(ENABLE_OCR) && defined(_WIN32)
#define LEPT_MSG_SEVERITY L_SEVERITY_NONE
#endif

#ifdef PYTHON_API
#include "Python.h"
#include "funcobject.h"

PyObject* py_callback_func;
void api_add_param(struct ccx_s_options* api_options,char* arg);
int compile_params(struct ccx_s_options *api_options,int argc);
void py_callback(char *line, int encoding);
int api_param_count(struct ccx_s_options* api_options);
char * api_param(struct ccx_s_options* api_options, int count);
#endif

extern struct ccx_s_options ccx_options;
extern struct lib_ccx_ctx *signal_ctx;
//volatile int terminate_asap = 0;

extern struct ccx_s_options* g_api_options;
extern struct lib_ccx_ctx* g_ctx;

extern unsigned char shared_byte_array[1024*1024*8];
extern int shared_byte_array_size;
extern pthread_mutex_t mutex1;

extern unsigned char shared_subtitles[1024*1024*8];
extern int shared_subtitles_size;
extern int shared_subtitles_lines;
extern pthread_mutex_t mutex2;

extern FILE* log_fp;

struct ccx_s_options* api_init_options();

void sigterm_handler(int sig);
void sigint_handler(int sig);
void print_end_msg(void);

int init_ccextractor(int argc, char **argv/*, const char* path*/);
int run_ccextractor(long total_inputsize);
int write_shared_bytes(unsigned char* byteArray, int length);
int get_header(unsigned char** data);
int append_subtitle();
void deinit_ccextractor();
void thread_exit_handler(int);

#endif //CCEXTRACTOR_H
