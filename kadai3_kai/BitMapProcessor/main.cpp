#include "BitMapProcessor.h"

int main() {
    BitMapProcessor bmp;

    //ファイルの読み込み
    bmp.readData("image/flower.bmp");

    //半分にリサイズ
    bmp.resizeData("output/resizeImage.bmp");

    //RGBを入れ替える
    //bmp.changeData();

    //ファイルの書き出し
    //bmp.writeData("output/outputImage.bmp");

    return 0;
}