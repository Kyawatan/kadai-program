#pragma warning(disable : 4996) //fopen使用エラー回避

#include <cstdio>
#include <iostream>
#include <cstring>
#include "BitMapProcessor.h"

#define FILE_TYPE 0x4D42    //BM
#define FILE_HEADER_SIZE 14 //ヘッダサイズ
#define INFO_HEADER_SIZE 40 //Windows情報ヘッダサイズ
#define DEFAULT_HEADER_SIZE (FILE_HEADER_SIZE + INFO_HEADER_SIZE)
#define ALIGNMENT 4

using namespace std;

/*
* コンストラクタ
*/
BitMapProcessor::BitMapProcessor() {

    //memset(headerBuffer, sizeof(uint8_t) * DEFAULT_HEADER_SIZE);
    buffer = NULL;
    rgb24Buffer = NULL;
    fHeader = (BITMAPFILEHEADER*)headerBuffer ;
    iHeader = (BITMAPINFOHEADER*)(headerBuffer + FILE_HEADER_SIZE);
    height = NULL;
    width = NULL;
    padding = NULL;
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

    //printHeader();
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

    if (iHeader->biBitCount != 24) {
        cout << "24ビットマップではありません。" << endl;
        exit(EXIT_FAILURE);
    }

    height = iHeader->biHeight;
    width = iHeader->biWidth;
    padding = width % ALIGNMENT;
}

/*
* 画像データを読む
*/
void BitMapProcessor::readBmpData() {
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
}

/*
* ヘッダを表示する
*/
void BitMapProcessor::printHeader() {
    cout << "----------------------------------------" << endl;
    cout << "dgType      : " << fHeader->dgType << ", " << sizeof(fHeader->dgType) << " byte" << endl;
    cout << "bfSize      : " << fHeader->bfSize << ", " << sizeof(fHeader->bfSize) << " byte" << endl;
    cout << "bfReserved1 : " << fHeader->bfReserved1 << ", " << sizeof(fHeader->bfReserved1) << " byte" << endl;
    cout << "bfRserved2  : " << fHeader->bfReserved2 << ", " << sizeof(fHeader->bfReserved2) << " byte" << endl;
    cout << "bfOffBits   : " << fHeader->bfOffBits << ", " << sizeof(fHeader->bfOffBits) << " byte" << endl;
    cout << "biSize          : " << iHeader->biSize << ", " << sizeof(iHeader->biSize) << " byte" << endl;
    cout << "biWidth         : " << iHeader->biWidth << ", " << sizeof(iHeader->biWidth) << " byte" << endl;
    cout << "biHeight        : " << iHeader->biHeight << ", " << sizeof(iHeader->biHeight) << " byte" << endl;
    cout << "biPlanes        : " << iHeader->biPlanes << ", " << sizeof(iHeader->biPlanes) << " byte" << endl;
    cout << "biBitCount      : " << iHeader->biBitCount << ", " << sizeof(iHeader->biBitCount) << " byte" << endl;
    cout << "biCompression   : " << iHeader->biCompression << ", " << sizeof(iHeader->biCompression) << " byte" << endl;
    cout << "biSizeImage     : " << iHeader->biSizeImage << ", " << sizeof(iHeader->biSizeImage) << " byte" << endl;
    cout << "biXPelsPerMeter : " << iHeader->biXPelsPerMeter << ", " << sizeof(iHeader->biXPelsPerMeter) << " byte" << endl;
    cout << "biYPelsPerMeter : " << iHeader->biYPelsPerMeter << ", " << sizeof(iHeader->biYPelsPerMeter) << " byte" << endl;
    cout << "biClrUsed       : " << iHeader->biClrUsed << ", " << sizeof(iHeader->biClrUsed) << " byte" << endl;
    cout << "biClrImportant  : " << iHeader->biClrImportant << ", " << sizeof(iHeader->biClrImportant) << " byte" << endl;
    cout << "----------------------------------------" << endl;
}

/*
* ヘッダを更新する
*/
void BitMapProcessor::updateHeader() {
    iHeader->biHeight = height;
    iHeader->biWidth = width;
    iHeader->biPlanes = 1;
    iHeader->biSizeImage = iHeader->biWidth * iHeader->biBitCount / 8 * iHeader->biHeight;
    iHeader->biSize = INFO_HEADER_SIZE;

    fHeader->dgType = FILE_TYPE;
    fHeader->bfReserved1 = 0;
    fHeader->bfReserved2 = 0;
    fHeader->bfOffBits = DEFAULT_HEADER_SIZE;
    fHeader->bfSize = iHeader->biSizeImage + fHeader->bfOffBits;

    //printHeader();
}

/*
* 画像データをリセットする
*/
void BitMapProcessor::resetData() {
    int n = DEFAULT_HEADER_SIZE;

    //ヘッダを上書き
    for (int i = 0; i < n; i++) {
        headerBuffer[i] = *(buffer + i);
    }

    //RGBデータを上書き
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            memcpy(&rgb24Buffer[y][x], buffer + n, sizeof(RGB_24));
            n += sizeof(RGB_24);
        }
        n += padding; //パディングを読み飛ばす
    }
}

/*
* 画像を書く
*/
void BitMapProcessor::writeData(string filename) {
    int zeroPadding = 0; //パディング用

    //ファイルオープン
    FILE* o_fp = fopen(filename.c_str(), "wb");
    if (o_fp == NULL) {
        cout << "ファイルオープンに失敗しました。" << endl;
        exit(EXIT_FAILURE);
    }

    //ヘッダー書き込み
    fwrite(headerBuffer, sizeof(uint8_t), DEFAULT_HEADER_SIZE, o_fp);

    //1ピクセルずつデータ書き込み
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fwrite(&rgb24Buffer[y][x], sizeof(RGB_24), 1, o_fp);
        }
        fwrite(&zeroPadding, sizeof(uint8_t), padding, o_fp); //パディング
    }

    //ファイルクローズ
    fclose(o_fp);
}

/*
* 画像サイズを1/2に縮小する（間引き）
*/
void BitMapProcessor::resizeData(string filename) {
    int zeroPadding = 0; //パディング用
    
    //幅・高さを1/2にする
    height = iHeader->biHeight /= 2;
    width = iHeader->biWidth /= 2;
    padding = width % ALIGNMENT;

    //ヘッダ情報を更新
    updateHeader();

    //printHeader();

    //ファイルオープン
    FILE* o_fp = fopen(filename.c_str(), "wb");
    if (o_fp == NULL) {
        cout << "ファイルオープンに失敗しました。" << endl;
        exit(EXIT_FAILURE);
    }

    //ヘッダ書き込み
    fwrite(headerBuffer, sizeof(uint8_t), DEFAULT_HEADER_SIZE, o_fp);

    //1ピクセルずつデータ書き込み
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fwrite(&rgb24Buffer[y*2][x*2], sizeof(RGB_24), 1, o_fp);
        }
        fwrite(&zeroPadding, sizeof(uint8_t), padding, o_fp); //パディング
    }

    //ファイルクローズ
    fclose(o_fp);

    //変更した画像データをリセット
    resetData();
}

/*
* RGBを入れ替える
*/
void BitMapProcessor::changeData(string filename) {
    //RとBを入れ替える
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint8_t tmp = rgb24Buffer[y][x].rgbBlue;
            rgb24Buffer[y][x].rgbBlue = rgb24Buffer[y][x].rgbRed;
            rgb24Buffer[y][x].rgbRed = tmp;
        }
    }

    //画像を書く
    writeData(filename);

    //変更した画像データをリセット
    resetData();
}

/*
* 任意の縮小率で画像サイズを縮小する（最近傍）
*/
void BitMapProcessor::nearestNeighborData(string filename, int reductionRate) {
     if (reductionRate <= 0 || 100 <= reductionRate) {
        cout << "無効な数字です。" << endl;
        exit(EXIT_FAILURE);
    }

    const double rouding = 0.5; //四捨五入
    double rate = reductionRate / 100.0; //縮小率を小数点表示にする
    int zeroPadding = 0;

    //縮小前の高さと幅を保持しておく
    int exHeight = iHeader->biHeight;
    int exWidth = iHeader->biWidth;
    
    //縮小後の高さと幅を算出
    height = (int)(iHeader->biHeight * rate + rouding);
    width = (int)(iHeader->biWidth * rate + rouding);
    padding = width % ALIGNMENT; //縮小後のパディング値

    //元画像を縮小後の画像の幅・高さの倍で分割し、各格子の中央を指すようにする
    double a = exHeight / (height * 2);
    double b = exWidth / (width * 2);

    //ヘッダ情報を更新
    updateHeader();

    //printHeader();

//ファイルオープン
    FILE* o_fp = fopen(filename.c_str(), "wb");
    if (o_fp == NULL) {
        cout << "ファイルオープンに失敗しました。" << endl;
        exit(EXIT_FAILURE);
    }

    //ヘッダ書き込み
    fwrite(headerBuffer, sizeof(uint8_t), DEFAULT_HEADER_SIZE, o_fp);

    //1ピクセルずつデータ書き込み
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int a2 = (int)(a * ((long long)y * 2 + 1) + rouding);
            int b2 = (int)(b * ((long long)x * 2 + 1) + rouding);
            fwrite(&rgb24Buffer[a2][b2], sizeof(RGB_24), 1, o_fp);
        }
        fwrite(&zeroPadding, sizeof(uint8_t), padding, o_fp); //パディング
    }

    //ファイルクローズ
    fclose(o_fp);

    //変更した画像データをリセット
    resetData();

}