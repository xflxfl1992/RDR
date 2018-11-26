#ifndef BMP_FILE_H
#define BMP_FILE_H

//Latest Date: 2013.10.07

BYTE *Rmw_Read8BitBmpFile2Img(const char * filename,int *width,int *height);
bool Rmw_Write8BitImg2BmpFile(BYTE *pImg,int width,int height,const char * filename);
bool Rmw_Write8BitImgMark(BYTE *pImg,int width,int height,const char * filename);

BYTE *Rmw_Read24BitBmpFile2Img(const char * filename,int *width,int *height);
bool Rmw_Write24BitImg2BmpFile(BYTE *pImg,int width,int height,const char * filename);

#endif
