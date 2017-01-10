#include "skynet.h"
#include "skynet_timer.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define BUFF_SIZE 1024

struct logger {
	char date[BUFF_SIZE];
	char file[BUFF_SIZE];
	FILE* handle;
	int close;
};

struct logger *
loggerx_create(void) {
	struct logger * inst = skynet_malloc(sizeof(*inst));
	memset(inst, 0, sizeof(*inst));
	return inst;
}

void
loggerx_release(struct logger * inst) {
	if (inst->close) {
		fclose(inst->handle);
	}
	skynet_free(inst);
}

static void 
format_date(char *buf, int len) {
	double now = (skynet_gettime_fixsec() + skynet_gettime()) * 0.01;

    time_t sec = (time_t)now;

	strftime(buf, len, "%F", localtime(&sec));
}

static void 
format_time(char *buf, int len) {
	double now = (skynet_gettime_fixsec() + skynet_gettime()) * 0.01;

    time_t sec = (time_t)now;
    int ms = (now - sec) * 100;

    char tmp_time[BUFF_SIZE];
    strftime(tmp_time, BUFF_SIZE, "%F %T", localtime(&sec));

    snprintf(buf, len, "%s.%02d", tmp_time, ms);
}

static int
_logger(struct skynet_context * context, void *ud, int type, int session, uint32_t source, const void * msg, size_t sz) {
	struct logger * inst = ud;

	if (inst->handle != stdout) {
		char date[BUFF_SIZE];
		format_date(date, BUFF_SIZE);

		if (strcmp(inst->date, date) != 0) {
			fclose(inst->handle);

			memcpy(inst->date, date, BUFF_SIZE);

			char tmp[BUFF_SIZE];
			memset(tmp, 0, BUFF_SIZE);
			snprintf(tmp, BUFF_SIZE, "%s-%s.log", inst->file, inst->date);

			inst->handle = fopen(tmp,"a+");
			if (inst->handle == NULL) {
				inst->close = 0;
				fprintf(stderr, "can't open file: %s\n", tmp);
				return 0;
			}
		}
	}

	char time_str[BUFF_SIZE] = {0};
	format_time(time_str, BUFF_SIZE);

	fprintf(inst->handle, "%s", time_str);
	fprintf(inst->handle, ":[%08x] ",source);
	fwrite(msg, sz , 1, inst->handle);
	fprintf(inst->handle, "\n");
	fflush(inst->handle);

	return 0;
}

int
loggerx_init(struct logger * inst, struct skynet_context *ctx, const char * parm) {
	if (parm) {
		snprintf(inst->file, BUFF_SIZE, "%s", parm);
	} 

	if (strlen(inst->file) != 0) {
		char date[BUFF_SIZE];
		format_date(date, BUFF_SIZE);

		memcpy(inst->date, date, BUFF_SIZE);

		char tmp[BUFF_SIZE];
		memset(tmp, 0, BUFF_SIZE);
		snprintf(tmp, BUFF_SIZE, "%s-%s.log", inst->file, inst->date);

		inst->handle = fopen(tmp,"a+");
		if (inst->handle == NULL) {
			fprintf(stderr, "can't open file: %s\n", tmp);
			return 1;
		}
		inst->close = 1;
	} else {
		inst->handle = stdout;
	}
	
	if (inst->handle) {
		skynet_callback(ctx, inst, _logger);
		skynet_command(ctx, "REG", ".logger");
		return 0;
	}
	return 1;
}
