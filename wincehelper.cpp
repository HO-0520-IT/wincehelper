///////////////////////////////////////////////////////////////////////////////
//  wincehelper.cpp
//  WinCE での動作を前提としていないプログラムを
//  WinCE で動かせるようにするための自作関数
//  (C) HO_0520_IT
///////////////////////////////////////////////////////////////////////////////


#include <io.h>
#include <stdio.h>
#include <winsock2.h>
#include <Shlwapi.h>

#include "wincehelper.h"
char wceh_cwd[MAX_PATH + 1] = "";
char *wceh_getcwd(char *buffer, int maxlen) {
	if (wceh_cwd[0] == 0) {
		TCHAR buf[MAX_PATH + 1];
		if (GetModuleFileName(NULL, buf, MAX_PATH) == 0) {
			printf("Fatal error. GetModuleFileName failed.\n");
			exit(1);
		}
		char buf_char[MAX_PATH + 1] = "";
		WideCharToMultiByte(CP_ACP, 0, buf, -1, buf_char, sizeof(buf_char), NULL, NULL);
		char* dir_backslash;
		dir_backslash = strrchr(buf_char, '\\');
		strncpy(wceh_cwd, buf_char, dir_backslash - buf_char + 1);
		wceh_cwd[dir_backslash - buf_char + 2] = '\0';
	}
	if (maxlen <= strlen(wceh_cwd) ) {
		printf("Fatal error. Insufficient \"buffer\" length.\n");
		exit(1);
	}
	buffer = wceh_cwd;
	printf("wceh_cwd set: %s\n", wceh_cwd);
	return wceh_cwd;
}

int wceh_CHDIR(const char *dirname) {
	if ((dirname[0] != 0) && (strlen(dirname) <= MAX_PATH + 1)) {
		strncpy(wceh_cwd, dirname, strlen(dirname));
		printf("wceh_CHDIR set: %s\n", wceh_cwd);
		return 0;
	} else {
		return -1;
	}
}

FILE *wceh_fopen(const char *filename, const char *mode) {
	FILE *ret;
	printf("fopen: %s\n", filename);
	ret = fopen(filename, mode);
	if (ret == NULL) {
		char buf[MAX_PATH + 1] = "";
		strncpy(buf, wceh_cwd, strlen(wceh_cwd));
		strncat(buf, filename, strlen(filename));
		printf("fopen: %s\n", buf);
		ret = fopen(buf, mode);
	}
	if (ret == NULL) {
		printf("file not found!!\n");
		//exit(1);
	}
	return ret;
}
