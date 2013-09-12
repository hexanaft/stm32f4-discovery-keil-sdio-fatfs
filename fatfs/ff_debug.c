#include "ff_debug.h"

void FR_print_error( FRESULT fresult )
{
	/* File function return code (FRESULT) */
	const char *FRErrorStr[] = {
		"FR_OK",					/* (0) Succeeded */
		"FR_DISK_ERR",				/* (1) A hard error occurred in the low level disk I/O layer */
		"FR_INT_ERR",				/* (2) Assertion failed */
		"FR_NOT_READY",				/* (3) The physical drive cannot work */
		"FR_NO_FILE",				/* (4) Could not find the file */
		"FR_NO_PATH",				/* (5) Could not find the path */
		"FR_INVALID_NAME",			/* (6) The path name format is invalid */
		"FR_DENIED",				/* (7) Access denied due to prohibited access or directory full */
		"FR_EXIST",					/* (8) Access denied due to prohibited access */
		"FR_INVALID_OBJECT",		/* (9) The file/directory object is invalid */
		"FR_WRITE_PROTECTED",		/* (10) The physical drive is write protected */
		"FR_INVALID_DRIVE",			/* (11) The logical drive number is invalid */
		"FR_NOT_ENABLED",			/* (12) The volume has no work area */
		"FR_NO_FILESYSTEM",			/* (13) There is no valid FAT volume */
		"FR_MKFS_ABORTED",			/* (14) The f_mkfs() aborted due to any parameter error */
		"FR_TIMEOUT",				/* (15) Could not get a grant to access the volume within defined period */
		"FR_LOCKED",				/* (16) The operation is rejected according to the file sharing policy */
		"FR_NOT_ENOUGH_CORE",		/* (17) LFN working buffer could not be allocated */
		"FR_TOO_MANY_OPEN_FILES",	/* (18) Number of open files > _FS_SHARE */
		"FR_INVALID_PARAMETER"		/* (19) Given parameter is invalid */
	};
	
	printf("\t!FRESULT: %d - %s \n", fresult, FRErrorStr[fresult] );
}

#define _DF1S	0

FRESULT scan_files (char* path)
{
	FRESULT res;
	FILINFO fno;
	DIR dir;
	int i;
	char *fn;
#if _USE_LFN
	static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
	fno.lfname = lfn;
	fno.lfsize = sizeof(lfn);
#endif

	DEBUG_PRINT(("f_opendir:\n"));
	res = f_opendir(&dir, path);
	if (res == FR_OK) {
		i = strlen(path);
		for (;;) {
			DEBUG_PRINT(("f_readdir:\n"));
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0) 
				{
					#ifdef DEBUG
					FR_print_error(res);
					#endif
					break;
				}
			if (fno.fname[0] == '.') continue;
#if _USE_LFN
			fn = *fno.lfname ? fno.lfname : fno.fname;
#else
			fn = fno.fname;
#endif
			if (fno.fattrib & AM_DIR) {
				DEBUG_PRINT(("path:%s\n", fn));
				sprintf(&path[i], "/%s", fn);
				res = scan_files(path);
				if (res != FR_OK)
				{
					#ifdef DEBUG
					FR_print_error(res);
					#endif
					break;
				}
				path[i] = 0;
			} else {
				printf("%s/%s\n", path, fn);
			}
		}
	}
	else 
	{
		#ifdef DEBUG
		FR_print_error(res);
		#endif
	}

	return res;
}
