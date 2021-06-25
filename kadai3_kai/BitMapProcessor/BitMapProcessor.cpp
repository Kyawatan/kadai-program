#pragma warning(disable : 4996) //fopen使用エラー回避

#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "BitMapProcessor.h"

#define FILE_TYPE 0x4D42    //BM
#define FILE_HEADER_SIZE 14 //ヘッダサイズ
#define INFO_HEADER_SIZE 40 //Windows情報ヘッダサイズ
#define DEFAULT_HEADER_SIZE (FILE_HEADER_SIZE + INFO_HEADER_SIZE)

using namespace std;

/*
* コンストラクタ
*/
BitMapProcessor::BitMapProcessor() {

    //memset(headerBuffer, sizeof(uint8_t) * DEFAULT_HEADER_SIZE);
    buffer = NULL;
    rgbBuffer = NULL;
    rgb24Buffer = NULL;
    fHeader = (BITMAPFILEHEADER*)headerBuffer ;
    iHeader = (BITMAPINFOHEADER*)(headerBuffer + FILE_HEADER_SIZE);
};

/*
* デストラクタ
*/
BitMapProcessor::~BitMapProcessor() {
    free(buffer);
    for (int i = 0; i < iHeader->biHeight; i++) {
        delete rgb24Buffer[i];
    }
    delete[]rgb24Buffer;
}

/*
* 画像を読む
*/
void BitMapProcessor::readData(string filename) {
    int length;

    //ファイルオープン
    FILE* i_fp = fopen(filename.c_str(), "rb");
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

    printHeader();
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
}

/*
* 画像データを読む
*/
void BitMapProcessor::readBmpData() {
    int colorPallete;
    int height = iHeader->biHeight;
    int width = iHeader->biWidth;
    int padding;

    switch (iHeader->biBitCount) {
    case 1:
    case 4:
    case 8:
        //カラーパレット数分メモリ確保
        colorPallete = 2 ^ iHeader->biBitCount;
        rgbBuffer = new RGBQUAD[colorPallete];

        //カラーパレットのRGBを読む
        for (int i = 0; i < colorPallete; i++) {
            int n = DEFAULT_HEADER_SIZE;
            memcpy(&rgbBuffer[i], buffer + n, sizeof(RGBQUAD));
            n += sizeof(RGBQUAD);
        }
        break;

    case 24:
        padding = width % 4;

        //RGBを格納する二次元配列の動的確保
        rgb24Buffer = new RGB_24 * [height];
        for (int i = 0; i < height; i++) {
            rgb24Buffer[i] = new RGB_24[width];
        }

        //1ピクセルずつデータを読む
        int n = DEFAULT_HEADER_SIZE;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                memcpy(&rgb24Buffer[y][x], buffer + n, sizeof(RGB_24));
                n += sizeof(RGB_24);
            }
            n += padding; //パディングを読み飛ばす
        }
        break;
    }
}

/*
* 画像を書く
*/
void BitMapProcessor::writeData(string filename) {
    int width = iHeader->biWidth;
    int height = iHeader->biHeight;
    int offset = fHeader->bfOffBits;    //オフセット
    int padding = width % 4;
    int lineByte = width * iHeader->biBitCount / 8 + padding; //パディングを含めた1列あたりのバイト数

    //ファイルオープン
    FILE* o_fp = fopen(filename.c_str(), "wb");
    if (o_fp == NULL) {
        cout << "ファイルオープンに失敗しました。" << endl;
        exit(EXIT_FAILURE);
    }

    //オフセット書き込み
    fwrite(headerBuffer, sizeof(uint8_t), offset, o_fp);

    //画像幅1列分のサイズの配列を動的確保
    uint8_t* line = new uint8_t[lineByte];
    for (int i = 0; i < lineByte; i++) {
        line[i] = 0;
    }

    //画像幅1列ずつデータ書き込み
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int n = x * sizeof(RGB_24);
            line[n] = rgb24Buffer[y][x].rgbBlue;
            line[n + 1] = rgb24Buffer[y][x].rgbGreen;
            line[n + 2] = rgb24Buffer[y][x].rgbRed;
        }
        fwrite(line, sizeof(uint8_t), lineByte, o_fp);
    }

    delete[]line;

    //ファイルクローズ
    fclose(o_fp);
}

/*
* ヘッダを表示する
*/
void BitMapProcessor::printHeader() {
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
* ヘッダを更新する
*/
void BitMapProcessor::updateHeader() {
    //biXPelsPerMeter = ;
    //biYPelsPerMeter = ;
    iHeader->biPlanes = 1;
    iHeader->biSizeImage = iHeader->biWidth * iHeader->biBitCount / 8 * iHeader->biHeight;
    iHeader->biSize = INFO_HEADER_SIZE;

    fHeader->dgType = FILE_TYPE;
    fHeader->bfReserved1 = 0;
    fHeader->bfReserved2 = 0;
    fHeader->bfOffBits = DEFAULT_HEADER_SIZE;
    fHeader->bfSize = iHeader->biSizeImage + fHeader->bfOffBits;

    cout << "ヘッダ情報を更新しました。" << endl;
    printHeader();
}

/*
* 画像サイズを縮小する
*/
void BitMapProcessor::resizeData(string filename) {
    //幅・高さを1/2にする
    iHeader->biWidth /= 2;
    iHeader->biHeight /= 2;

    //ヘッダ情報を更新
    updateHeader();

    int offset = fHeader->bfOffBits; //オフセット
    int width = iHeader->biWidth;
    int height = iHeader->biHeight;
    int padding = width % 4;
    int lineByte = width * iHeader->biBitCount / 8 + padding; //パディングを含めた1列あたりのバイト数

    //ファイルオープン
    FILE* o_fp = fopen(filename.c_str(), "wb");
    if (o_fp == NULL) {
        cout << "ファイルオープンに失敗しました。" << endl;
        exit(EXIT_FAILURE);
    }

    //オフセット書き込み
    fwrite(headerBuffer, sizeof(uint8_t), offset, o_fp);

    //パディング分の配列を動的確保
    uint8_t* zeroPadding = new uint8_t[padding];
    for (int i = 0; i < padding; i++) {
        zeroPadding[i] = 0;
    }

    //画像幅1列ずつデータ書き込み
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fwrite(&rgb24Buffer[y*2][x*2], sizeof(RGB_24), 1, o_fp);
        }
        fwrite(&zeroPadding, sizeof(uint8_t), padding, o_fp);
    }

    delete[]zeroPadding;

    //ファイルクローズ
    fclose(o_fp);
}

/*
* RGBを入れ替える
*/
void BitMapProcessor::changeData(string filename) {
    int width = iHeader->biWidth;
    int height = iHeader->biHeight;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint8_t tmp = rgb24Buffer[y][x].rgbBlue;
            rgb24Buffer[y][x].rgbBlue = rgb24Buffer[y][x].rgbRed;
            rgb24Buffer[y][x].rgbRed = tmp;
        }
    }

    //画像を出力
    writeData(filename);
}