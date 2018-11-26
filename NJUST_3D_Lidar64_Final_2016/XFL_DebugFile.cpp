#include "NJUST_ALV_BYD_H/ALV_BYD.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 文件调试
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void XFL_DebugFile(int i,char *pStr)
{
	static bool isFirst=true;
	static int sn=0;
	FILE *fp;

	if (isFirst)
	{	
        #ifdef _NJUST_OS_USE_LINUX_
		fp=fopen("bin/XFL_debug.txt","w+t");
        #endif
	    #ifdef _NJUST_OS_USE_WINDOWS_
		fp=fopen("d:\\tmp\\XFL_debug.txt","w+t");
        #endif
		if (fp)
		{
			fprintf(fp,"%08d : %d__%s\n",sn++,i,pStr);
			fclose(fp);
		}
		isFirst=false;
	}
	else
	{
		#ifdef _NJUST_OS_USE_LINUX_
		fp=fopen("bin/XFL_debug.txt","a+t");
        #endif
	    #ifdef _NJUST_OS_USE_WINDOWS_
		fp=fopen("d:\\tmp\\XFL_debug.txt","a+t");
        #endif
		if (fp)
		{
			fprintf(fp,"%08d : %d__%s\n",sn++,i,pStr);
			fclose(fp);
		}
	}
	return;
}
 