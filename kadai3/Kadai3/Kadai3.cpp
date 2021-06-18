#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cstring>

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
    FILE *bmp;                  //画像のファイルポインタ
    uint8_t* buffer;
    uint8_t headerBuffer[DEFAULT_HEADER_SIZE];    //ヘッダの40バイト分
    BITMAPFILEHEADER *fHeader;   //ファイルヘッダ
    BITMAPINFOHEADER *iHeader;   //情報ヘッダ
    int width;
    int height;

public:
    BitMapProcessor() {
        bmp = NULL;
        buffer = NULL;
        fHeader = (BITMAPFILEHEADER*)headerBuffer;
        iHeader = (BITMAPINFOHEADER*)(headerBuffer + FILE_HEADER_SIZE);
        width = NULL;
        height = NULL;
    };

    ~BitMapProcessor() {
        fclose(bmp);
    }

    void loadData(string filename);

private:
    void readFileHeader();
    void readInfoHeader();
    void readBmpData();
};

void BitMapProcessor::loadData(string filename) {
    int length;
    
    //ファイルオープン
    bmp = fopen(filename.c_str(), "rb");
    if (bmp == NULL) {
        cout << "ファイルオープンに失敗しました。" << endl;
        exit(EXIT_FAILURE);
    }

    //ファイルサイズの取得
    fseek(bmp, 0L, SEEK_END);
    length = ftell(bmp);

    //メモリ割り当て
    buffer = (uint8_t*)malloc(length);
    if (buffer == NULL) {
        cout << "メモリ割り当てに失敗しました。" << endl;
        exit(EXIT_FAILURE);
    }

    //ファイルの先頭に移動
    fseek(bmp, 0L, SEEK_SET);

    //データ読み込み
    int n = fread(buffer, sizeof(uint8_t), length, bmp);
    if (n != length) {
        cout << "データ読み込みに失敗しました。" << endl;
        exit(EXIT_FAILURE);
    }

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

    //とりあえず24ビットマップのみを読み込む
    if (iHeader->biBitCount != 24) {
        cout << "24ビットマップではありません。" << endl;
        exit(EXIT_FAILURE);
    }

    width = iHeader->biWidth;   //画像の幅
    height = iHeader->biHeight; //画像の高さ
}

/*
* 画像データを読む
*/
void BitMapProcessor::readBmpData() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {

        }
    }
}

int main()
{
    BitMapProcessor bmp;

    //ファイルのロード
    bmp.loadData("C:/Users/nicov/source/repos/Kadai3/flower.bmp");

    return 0;
}

