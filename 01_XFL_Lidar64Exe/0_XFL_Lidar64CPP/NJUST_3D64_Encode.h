#ifndef _NJUST_3D64_ENCODE_H_
#define _NJUST_3D64_ENCODE_H_

#include "NJUST_ALV_BYD_H/ALV_BYD.h"
/////////////////////////////////////////////////////////////////////////////
//
//
//---------------------------OBS-------------------------------------------//
//
//
/////////////////////////////////////////////////////////////////////////////
int NJUST_3D64_Encode_Compress_OBS(NJUST_3D64_OBS_TO_FU *p3D64Obs, 
								   unsigned char *p3D64Buf,
								   int maxResBufSize
								   );
/////////////////////////////////////////////////////////////////////////////
//
//
//---------------------------Boundary--------------------------------------//
//
//
/////////////////////////////////////////////////////////////////////////////
int NJUST_3D64_Encode_Compress_Boundary(NJUST_3D64_ROAD_BOUNDARY_TO_FU *p3D64Boundary, 
										unsigned char *p3D64Buf,
										int maxResBufSize
										);
/////////////////////////////////////////////////////////////////////////////
//
//
//---------------------------Grid------------------------------------------//
//
//
/////////////////////////////////////////////////////////////////////////////
int NJUST_3D64_Encode_Compress_Grid(NJUST_3D64_GRID_TO_FU *p3D64Grid, 
									unsigned char *p3D64Buf,
									int maxResBufSize
									);
/////////////////////////////////////////////////////////////////////////////
//
//
//---------------------------Cross-----------------------------------------//
//
//
/////////////////////////////////////////////////////////////////////////////
int NJUST_3D64_Encode_Compress_Cross(NJUST_3D64_CROSS_TO_FU *p3D64Cross, 
									 unsigned char *p3D64Buf,
									 int maxResBufSize
									 );

int NJUST_3D64_Encode_Compress(  int DataType,//压缩包的参数
								 NJUST_3D64_OBS_TO_FU *p3D64Obs,
								 NJUST_3D64_ROAD_BOUNDARY_TO_FU *p3D64Boundary,
								 NJUST_3D64_GRID_TO_FU *p3D64Grid,
								 NJUST_3D64_CROSS_TO_FU *p3D64Cross,
								 unsigned char *p3D64Buf,
								 int maxResBufSize
							  );

#endif