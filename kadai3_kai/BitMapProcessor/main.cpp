#include "BitMapProcessor.h"
#include <iostream>

int main() {
    BitMapProcessor bmp;

    string filename;
    int reductionRate;

    //ファイルの読み込み
    cout << "読み込むBMPファイル名を入力してください。（拡張子不要）" << endl;
    cin >> filename;

    bmp.readData("image/" + filename + ".bmp");

    cout << filename << ".bmpを読み込みました。" << endl;

    //コピーを書き出し
    bmp.writeData("output/copyImage.bmp");

    //RGBを入れ替えて書き出し
    bmp.changeData("output/changeImage.bmp");

    //半分にリサイズして書き出し
    bmp.resizeData("output/resizeImage.bmp");

    //任意の縮小率でリサイズして書き出し
    cout << "画像の縮小率を入力してください。（％）" << endl;
    cin >> reductionRate;

    bmp.nearestNeighborData("output/nearestNeighborImage.bmp", reductionRate);

    cout << "全ての処理が完了しました。" << endl;

    return 0;
}