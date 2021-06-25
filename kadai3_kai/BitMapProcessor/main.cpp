#include "BitMapProcessor.h"

int main() {
    BitMapProcessor bmp;

    //ファイルの読み込み
    bmp.readData("image/Parrots.bmp");

    //コピーを書き出し
    bmp.writeData("output/copyImage.bmp");

    //RGBを入れ替えて書き出し
    //bmp.changeData("output/changeImage.bmp");

    //半分にリサイズして書き出し
    bmp.resizeData("output/resizeImage.bmp");

    return 0;
}