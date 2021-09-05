// 2020.12.20 Add 图片格式转换 — BitMap ——> png

#include <iostream>


//--------------------------------------------------------------------------------
/*  获得 A1R5G5B5 红色分量 */
//inline BYTE R1555(unsigned short  clr)
//{
//	return  ((clr & 0x7C00) >> 7) & 0x0ff;
//}
///*  获得 A1R5G5B5 绿色分量 */
//inline BYTE G1555(unsigned short  clr)
//{
//	return  ((clr & 0x03E0) >> 2) & 0x0ff;
//}
///*  获得 A1R5G5B5 蓝色分量 */
//inline BYTE B1555(unsigned short  clr)
//{
//	return  ((clr & 0x001F) << 3) & 0x0ff;
//}
///* 四字节对齐 */
//int DWORDAlign(int nBits)
//{
//	int nAlignBytes = (nBits + 31) / 32 * 4;
//	return nAlignBytes;
//}
/*
测试环境：VS2008 with SP1 + WinXP with SP3
功能：[HBITMAP bitmap]中指向的位图压缩成名为[const wchar_t* filename]的JPG文件
入口参数：HDC hDC  当前显示设备句柄
返回值：    TRUE: 成功
			FALSE:失败
说明：只支持16(ARGB1555),24,32 三种颜色深度的 HBITMAP类型
最后修改：kagula,2012-05-15
参考资料
[1]使用jpeglib库实现bmp转jpg (转)
http://www.cppblog.com/cc/archive/2012/02/22/166217.html
[2]bmp轉jpg錯誤總結 + jpeglib使用指南
http://www.guan8.net/Java/409856.html
[3]图片在jpg 格式与bmp24格式之间的转换
http://www.rosoo.net/a/201105/11816.html
[4]依赖库IJG的官网
http://www.ijg.org/
备注：
[1]JPG格式不支持色深超过24位的真彩色图像
*/
//BOOL WriteJPGFile(HBITMAP bitmap, const wchar_t* filename, HDC hDC)
//{
//	BITMAP bmp;
//	struct jpeg_compress_struct jcs;
//	struct jpeg_error_mgr       jerr;
//	FILE                       *outfile;           /* target file */
//	JSAMPROW                    row_pointer[1];    /* pointer to JSAMPLE row[s] */
//	unsigned int                jcs_row_stride;    /* 目标缓存 每一行的字節數 */
//	unsigned int                src_row_stride;    /*   源缓存 每一行的字節數 */
//
//	// create the bitmapinfo header information
//	if (!GetObject(bitmap, sizeof(BITMAP), (void *)(&bmp))) {
//		g_log.Add("WriteJPGFile  Could not retrieve bitmap info");
//		return FALSE;
//	}
//
//	if (bmp.bmBitsPixel < 16)
//	{
//		g_log.Add("WriteJPGFile  只支持16(ARGB1555),24,32 三种深度的 HBITMAP类型");
//		return FALSE;
//	}
//
//	jcs.err = jpeg_std_error(&jerr);
//	jpeg_create_compress(&jcs);
//	if ((outfile = _wfopen(filename, L"wb")) == NULL) {
//		g_log.Add("WriteJPGFile 无法打开文件");
//		return FALSE;
//	}
//
//	jpeg_stdio_dest(&jcs, outfile);
//	jcs.image_width = bmp.bmWidth;
//	jcs.image_height = bmp.bmHeight;
//	jcs.input_components = 3;          /* 在此為1,表示灰度圖， 如果是彩色位圖，則為3。经测试此值只能为1或3。*/
//	jcs.in_color_space = JCS_RGB;     /* JCS_GRAYSCALE表示灰度圖，JCS_RGB表示彩色圖像。*/
//	jpeg_set_defaults(&jcs);
//	jpeg_set_quality(&jcs, JPEG_QUALITY, TRUE);/*[0-100],if 100,quality is best,but size is biggest*/
//	jpeg_start_compress(&jcs, TRUE);
//	jcs_row_stride = bmp.bmWidth*jcs.input_components; /* JSAMPLEs per row in image_buffer */
//
//
//	BITMAPINFOHEADER bih = { 0 };
//	bih.biBitCount = bmp.bmBitsPixel;
//	/* BI_RGB;16位中，最低的5位表示蓝色分量，中间的5位表示绿色分量，高的5位表示红色分量，一共占用了15位，最高的一位保留，设为0 */
//	bih.biCompression = BI_RGB;
//	bih.biHeight = bmp.bmHeight;
//	bih.biPlanes = bmp.bmPlanes;//总是为1。
//	bih.biSize = sizeof(BITMAPINFOHEADER);
//	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;
//	bih.biWidth = bmp.bmWidth;
//
//	/* 取源，像素数据信息.begin */
//	BYTE * bits = (BYTE *)malloc(bmp.bmWidth * 4 * (bmp.bmHeight + 1));/* 分配尽可能大的内存 */
//
//	// Retrieve the color table (RGBQUAD array) and the bits 
//	if (!GetDIBits(hDC, HBITMAP(bitmap), 0, (WORD)bmp.bmHeight, bits, (LPBITMAPINFO)&bih,
//		DIB_RGB_COLORS)) {
//		g_log.Add("WriteJPGFile  GetDIB error");
//		return FALSE;
//	}
//	/* 取源，像素数据信息.end */
//
//	src_row_stride = DWORDAlign(bmp.bmWidthBytes * 8);/* 返回每行字节数，必须DWORD对齐 */
//
//	/* 对源缓存中的像素做预处理.begin */
//	/* int BitPerPixel = GetDeviceCaps(hDC,BITSPIXEL); 取hDC中的设备颜色深度 */
//	if (bmp.bmBitsPixel == 32) /* 32 map to 24 */
//	{
//		int i = 0;
//		int j = 0;
//		for (; j < bmp.bmWidth*bmp.bmHeight * 4; i += 3, j += 4)
//		{
//			*(bits + i) = *(bits + j + 2);
//			*(bits + i + 1) = *(bits + j + 1);
//			*(bits + i + 2) = *(bits + j);
//		}
//	}
//	else if (bmp.bmBitsPixel == 24) /* 24 map to 24(Only need exchange the color component) */
//	{
//		BYTE * bitsT = (BYTE *)malloc(bmp.bmWidth*(bmp.bmHeight + 1) * 4);/* 分配尽可能大的临时空间 */
//		memset(bitsT, 0, bmp.bmWidth*(bmp.bmHeight + 1) * 4);
//
//		for (int row = 0; row < bmp.bmHeight; row++)
//			for (int col = 0; col < bmp.bmWidth; col++)
//			{
//				BYTE *pSrc = bits + row * src_row_stride + col * 3;
//				BYTE *pDst = bitsT + row * jcs_row_stride + col * 3;
//
//				*(pDst) = *(pSrc + 2), *(pDst + 1) = *(pSrc + 1), *(pDst + 2) = *pSrc;
//			}
//
//		memcpy(bits, bitsT, bmp.bmWidth*(bmp.bmHeight + 1) * 3);
//		free(bitsT);
//	}
//	else if (bmp.bmBitsPixel == 16) /* 16 map to 24 */
//	{
//		BYTE * bitsT = (BYTE *)malloc(bmp.bmWidth*(bmp.bmHeight + 1) * 4);/* 分配尽可能大的临时空间 */
//		memset(bitsT, 0, bmp.bmWidth*(bmp.bmHeight + 1) * 4);
//
//		for (int row = 0; row < bmp.bmHeight; row++)
//			for (int col = 0; col < bmp.bmWidth; col++)
//			{
//				BYTE *pSrc = bits + row * src_row_stride + col * 2;
//				BYTE *pDst = bitsT + row * jcs_row_stride + col * 3;
//
//				unsigned short clr16 = *(pSrc) | *(pSrc + 1) << 8;
//
//				*(pDst) = R1555(clr16);
//				*(pDst + 1) = G1555(clr16);
//				*(pDst + 2) = B1555(clr16);
//			}
//
//		memcpy(bits, bitsT, bmp.bmWidth*(bmp.bmHeight + 1) * 3);
//		free(bitsT);
//	}
//	/* 对源缓存中的像素做预处理.end */
//
//	/* 使用While语句进行，逐行壓縮 */
//	while (jcs.next_scanline < jcs.image_height) {
//		row_pointer[0] = &bits[(jcs.image_height - jcs.next_scanline - 1) * jcs_row_stride];
//		/* row_pointer[0] = & bits[jcs.next_scanline * row_stride]; 不做水平翻转*/
//		jpeg_write_scanlines(&jcs, row_pointer, 1);
//	}
//
//	jpeg_finish_compress(&jcs);
//	fclose(outfile);
//	free(bits);
//	jpeg_destroy_compress(&jcs);
//
//	return TRUE;
//}

//--------------------------------------------------------------------------------
// HBITMAP、CBitmap和BITMAP的区别和转换
// (1) HBITMAP、CBitmap和BITMAP的区别
// BITMAP是C++中定义的位图结构体
// HBITMAP是Windows中使用的位图句柄
// CBitmap是MFC封装的位图类

// (2) 图片类型转换
//-----------------
// HBITMAP->CBitmap
// 方法1: 
//HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//CBitmap bitmap;
//bitmap.Attach(hBitmap);
// 方法2: 
//HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//CBitmap *bitmap = CBitmap::FromHandle(hBitmap);
//-----------------
// HBITMAP->BITMAP
//CString str = _T("E:\\picture\\lena.bmp");
//HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//BITMAP bm;
//::GetObject(hBitmap, sizeof(bm), &bm);

//-----------------
// CBitmap->BITMAP
//CBitmap bitmap;
//bitmap.LoadBitmapW(IDB_BITMAP1);
//BITMAP bm;
//bitmap.GetBitmap(&bm);
//-----------------
// CBitmap->HBITMAP
//CBitmap bitmap;
//bitmap.LoadBitmapW(IDB_BITMAP1);
//HBITMAP hBitmap = (HBITMAP)bitmap.m_hObject;

//-----------------
// BITMAP->HBITMAP
//HBITMAP hBitmap;
//pbm->GetHBITMAP(NULL, &hBitmap);
//-----------------
// BITMAP->CBitmap
//Bitmap* pBitmap = new Bitmap(width, height, PixelFormat24bppRGB);
//HBITMAP hBitmap;
//BITMAP bm;
//pBitmap->GetHBITMAP(NULL, &hBitmap);
//CBitmap* bmp;
//bmp.Attach(hBitmap);

//--------------------------------------------------------------------------------
//#include <afxwin.h>

int main()
{
    std::cout << "Hello World!\n";

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

