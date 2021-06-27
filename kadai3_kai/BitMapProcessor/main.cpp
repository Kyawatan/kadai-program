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
    cout << filename << ".bmpを読み込みました。\n" << endl;

    //コピーを書き出し
    filename = "copyImage.bmp";
    bmp.writeData("output/" + filename);
    cout << "コピーした" << filename << "を書き出しました。\n" << endl;

    //RGBを入れ替えて書き出し
    filename = "changeImage.bmp";
    bmp.changeData("output/" + filename);
    cout << "RGBを入れ替えた" << filename << "を書き出しました。\n" << endl;

    //半分にリサイズして書き出し
    filename = "resizeImage.bmp";
    bmp.resizeData("output/" + filename);
    cout << "50％に縮小した" << filename << "を書き出しました。\n" << endl;

    //任意の縮小率でリサイズして書き出し
    cout << "画像の縮小率を入力してください。（1(％)～99(％)の整数値）" << endl;
    cin >> reductionRate;

    filename = "nearestNeighborImage.bmp";
    bmp.nearestNeighborData("output/" + filename, reductionRate);
    cout << reductionRate << "％に縮小した" << filename << "を書き出しました。\n" << endl;

    cout << "全ての処理が完了しました。" << endl;

    return 0;
}