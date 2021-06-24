#pragma once

#include <iomanip>

#define FILE_TYPE 0x4D42    //BM
#define FILE_HEADER_SIZE 14 //ヘッダサイズ
#define INFO_HEADER_SIZE 40 //Windows情報ヘッダサイズ
#define DEFAULT_HEADER_SIZE (FILE_HEADER_SIZE + INFO_HEADER_SIZE)

using namespace std;

/*
* ファイルヘッダ構造体
*/
#pragma pack(2) //構造体のアライメント境界を2byte
typedef struct BITMAPFILEHEADER {
    uint16_t dgType;        //BMP形式を認識するマット識別子 : 2Byte　0x4D42（リトルエンディアン）
    uint32_t bfSize;        //ファイル全体のサイズ : 4Byte
    uint16_t bfReserved1;   //予約領域1、書き出しは必ず0 : 2Byte
    uint16_t bfReserved2;   //予約領域2、書き出しは必ず0 : 2Byte
    uint32_t bfOffBits;     //ファイル先頭から画像情報までのオフセットバイト数 : 4Byte
} BITMAPFILEHEADER;

/*
* 情報ヘッダ構造体
*/
#pragma pack()  //デフォルト
typedef struct BITMAPINFOHEADER {
    uint32_t biSize;           //このヘッダのサイズ : 4Byte
    int32_t biWidth;           //画像の横方向のピクセル数 : 4Byte　正のみ
    int32_t biHeight;          //画像の縦方向のピクセル数 : 4Byte　正はボトムアップ
    uint16_t biPlanes;         //画像のプレーン数、必ず1 : 2Byte
    uint16_t biBitCount;       //1ピクセルあたりのビット数 : 2Byte　0,1,4,8,16,24,32
    uint32_t biCompression;    //ピクセルの格納方式 : 4Byte　デフォルトBI_RGB
    uint32_t biSizeImage;      //ヘッダを除いた画像領域のデータサイズ : 4Byte
    int32_t biXPelsPerMeter;   //横方向の画素密度、1メートルあたりのピクセル数 : 4Byte
    int32_t biYPelsPerMeter;   //縦方向の画素密度、1メートルあたりのピクセル数 : 4Byte
    uint32_t biClrUsed;        //カラーパレット方式の場合に使用する色数、それ以外の場合は0 : 4Byte
    uint32_t biClrImportant;   //重要な色数、0の場合はbiClrUsedと同じ : 4Byte
} BITMAPINFOHEADER;

/*
* RGB構造体
*/
typedef struct RGB_24 {
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
} RGB_24;

/*
* ビットマップ処理クラス
*/
class BitMapProcessor {
private:
    uint8_t* buffer;
    uint8_t headerBuffer[DEFAULT_HEADER_SIZE];  //ヘッダ部格納用
    RGB_24** imageBuffer;  //RGB格納用
    BITMAPFILEHEADER* fHeader;
    BITMAPINFOHEADER* iHeader;
    RGB_24* rgb24;

public:
    BitMapProcessor();
    ~BitMapProcessor();
    void readData(string);
    void writeData(string);
    void resizeData(string);
    void changeData();

private:
    void readFileHeader();
    void readInfoHeader();
    void readBmpData();
    void writePadding(uint8_t*, int, int);
    void printHeader();
    void updateHeader();
};