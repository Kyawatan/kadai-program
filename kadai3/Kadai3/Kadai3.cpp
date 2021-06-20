#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>

#define FILE_TYPE 0x4D42    //BM
#define FILE_HEADER_SIZE 14 //ヘッダサイズ
#define INFO_HEADER_SIZE 40 //Windows情報ヘッダサイズ
#define DEFAULT_HEADER_SIZE (FILE_HEADER_SIZE + INFO_HEADER_SIZE)

#pragma warning(disable : 4996) //fopen使用エラー回避

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
typedef struct RGBQUAD {
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
    uint8_t rgbReserved;   //予約領域、必ず0
} RGBQUAD;

/*
* ビットマップ処理クラス
*/
class BitMapProcessor {
    uint8_t* buffer;
    uint8_t headerBuffer[DEFAULT_HEADER_SIZE];
    int bmpData;                //画像データ
    BITMAPFILEHEADER *fHeader;  //ファイルヘッダ
    BITMAPINFOHEADER *iHeader;  //情報ヘッダ

public:
    BitMapProcessor() {
        buffer = NULL;
        bmpData = 0;
        fHeader = (BITMAPFILEHEADER*)headerBuffer;
        iHeader = (BITMAPINFOHEADER*)(headerBuffer + FILE_HEADER_SIZE);
    };

    ~BitMapProcessor() {
        free(buffer);
    }

    void readData(string filename);
    void writeData(string filename);
    void resizeData(string filename, double d);

private:
    void readFileHeader();
    void readInfoHeader();
    void readBmpData();
};

/*
* 画像を読む
*/
void BitMapProcessor::readData(string filename) {
    int length;
    
    //ファイルオープン
    FILE *i_fp = fopen(filename.c_str(), "rb");
    if (i_fp == NULL) {
        cout << "ファイルオープンに失敗しました。" << endl;
        exit(EXIT_FAILURE);
    }

    //ファイルサイズの取得
    fseek(i_fp, 0L, SEEK_END);
    length = ftell(i_fp);

    //メモリ割り当て
    buffer = (uint8_t*)malloc(length);
    if (buffer == NULL) {
        cout << "メモリ割り当てに失敗しました。" << endl;
        exit(EXIT_FAILURE);
    }

    //ファイルの先頭に移動
    fseek(i_fp, 0L, SEEK_SET);

    //データ読み込み
    size_t n = fread(buffer, sizeof(uint8_t), length, i_fp);
    if (n != length) {
        cout << "データ読み込みに失敗しました。" << endl;
        exit(EXIT_FAILURE);
    }

    //ファイルクローズ
    fclose(i_fp);

    readFileHeader();
    readInfoHeader();
    readBmpData();
}

/*
* ファイルヘッダを読む
*/
void BitMapProcessor::readFileHeader() {
    for (int i = 0; i < FILE_HEADER_SIZE; i++) {
        headerBuffer[i] = *(buffer + i);
    }

    if (fHeader->dgType != FILE_TYPE) {
        cout << "BMPファイルではありません。" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "dgType      : " << fHeader->dgType << endl;
    cout << "              " << sizeof(fHeader->dgType) << " byte" << endl;
    cout << "bfSize      : " << fHeader->bfSize << endl;
    cout << "              " << sizeof(fHeader->bfSize) << " byte" << endl;
    cout << "bfReserved1 : " << fHeader->bfReserved1 << endl;
    cout << "              " << sizeof(fHeader->bfReserved1) << " byte" << endl;
    cout << "bfRserved2  : " << fHeader->bfReserved2 << endl;
    cout << "              " << sizeof(fHeader->bfReserved2) << " byte" << endl;
    cout << "bfOffBits   : " << fHeader->bfOffBits << endl;
    cout << "              " << sizeof(fHeader->bfOffBits) << " byte" << endl;
}

/*
* 情報ヘッダを読む
*/
void BitMapProcessor::readInfoHeader() {
    for (int i = FILE_HEADER_SIZE; i < DEFAULT_HEADER_SIZE; i++) {
        headerBuffer[i] = *(buffer + i);
    }

    if (iHeader->biSize != INFO_HEADER_SIZE) {
        cout << "Windowsフォーマットではありません。" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "biSize          : " << iHeader->biSize << endl;
    cout << "                  " << sizeof(iHeader->biSize) << " byte" << endl;
    cout << "biWidth         : " << iHeader->biWidth << endl;
    cout << "                  " << sizeof(iHeader->biWidth) << " byte" << endl;
    cout << "biHeight        : " << iHeader->biHeight << endl;
    cout << "                  " << sizeof(iHeader->biHeight) << " byte" << endl;
    cout << "biPlanes        : " << iHeader->biPlanes << endl;
    cout << "                  " << sizeof(iHeader->biPlanes) << " byte" << endl;
    cout << "biBitCount      : " << iHeader->biBitCount << endl;
    cout << "                  " << sizeof(iHeader->biBitCount) << " byte" << endl;
    cout << "biCompression   : " << iHeader->biCompression << endl;
    cout << "                  " << sizeof(iHeader->biCompression) << " byte" << endl;
    cout << "biSizeImage     : " << iHeader->biSizeImage << endl;
    cout << "                  " << sizeof(iHeader->biSizeImage) << " byte" << endl;
    cout << "biXPelsPerMeter : " << iHeader->biXPelsPerMeter << endl;
    cout << "                  " << sizeof(iHeader->biXPelsPerMeter) << " byte" << endl;
    cout << "biYPelsPerMeter : " << iHeader->biYPelsPerMeter << endl;
    cout << "                  " << sizeof(iHeader->biYPelsPerMeter) << " byte" << endl;
    cout << "biClrUsed       : " << iHeader->biClrUsed << endl;
    cout << "                  " << sizeof(iHeader->biClrUsed) << " byte" << endl;
    cout << "biClrImportant  : " << iHeader->biClrImportant << endl;
    cout << "                  " << sizeof(iHeader->biClrImportant) << " byte" << endl;
}

/*
* ビットマップデータを読む
*/
void BitMapProcessor::readBmpData() {
    switch (iHeader->biBitCount) {
    case 1:
        bmpData = 2;
        break;
    case 4:
        bmpData = 16;
        break;
    case 8:
        bmpData = 256;
        break;
    }
}

/*
* 画像を書く
*/
void BitMapProcessor::writeData(string filename) {
    int width = iHeader->biWidth;       //画像の幅
    int height = iHeader->biHeight;     //画像の高さ
    int offset = fHeader->bfOffBits;    //オフセット
    int size = iHeader->biSizeImage;    //画像サイズ

    //ファイルオープン
    FILE* o_fp = fopen(filename.c_str(), "wb");

    //オフセット書き込み
    fwrite(buffer, sizeof(*buffer), offset, o_fp);

    //画像データの書き込み
    fwrite(buffer + offset, sizeof(*buffer), size, o_fp);

    //ファイルクローズ
    fclose(o_fp);
}

void BitMapProcessor::resizeData(string filename, double d) {
    int width = iHeader->biWidth;       //画像の幅
    int height = iHeader->biHeight;     //画像の高さ
    int offset = fHeader->bfOffBits;    //オフセット
    int size = iHeader->biSizeImage;    //画像サイズ
    int colorByte = iHeader->biBitCount / 4; //1ピクセルあたりのカラーバイト数
    int padding = width % 4;
    int new_padding = width * 2 % 4;

    //ファイルオープン
    FILE* o_fp = fopen(filename.c_str(), "wb");

    //オフセット書き込み
    fwrite(buffer, sizeof(*buffer), offset, o_fp);

    //画像データの書き込み
    int i = offset;
    int n = 1;
    while (i < size) {
        if (i / colorByte == width) {
            for (int j = 0; j < new_padding; j++) {
                fwrite(buffer + i, sizeof(*buffer), 1, o_fp);
            }
            i += padding;
        }
        if (n == 1) {
            fwrite(buffer + i, sizeof(*buffer), colorByte, o_fp);
        }
        i += colorByte;
        n *= (-1);
    }

    width = iHeader->biWidth *= d;
    //height = iHeader->biHeight *= d;
    size  = (((iHeader->biBitCount * width) + padding) * height / 8);

    //ファイルクローズ
    fclose(o_fp);

    cout << "RESIZE" << endl;
    cout << "width  : " << width << endl;
    cout << "height : " << height << endl;
    cout << "size   : " << size << endl;
}

int main()
{
    BitMapProcessor bmp;

    //ファイルの読み込み
    bmp.readData("image/rgb24.bmp");
    //0.5倍にリサイズ
    //bmp.resizeData("output/resizeImage.bmp", 0.5);
    //ファイルの書き出し
    //bmp.writeData("output/outputImage.bmp");

    return 0;
}

