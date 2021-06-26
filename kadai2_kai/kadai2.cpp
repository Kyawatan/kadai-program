#include "kadai2.h"

#include <iostream>
#include <iomanip>
#include <string>

#define ALIGNMENT 4

using namespace std;

//コンストラクタ
MemoryProcessor::MemoryProcessor() {
	i_fp = NULL;
	o_fp = NULL;
	length = 0;
	buffer = NULL;
}

//デストラクタ
MemoryProcessor::~MemoryProcessor() {
	free(buffer);
}

//ファイルを読む
void MemoryProcessor::readFile(string filename) {
	//ファイルオープン
        i_fp = fopen(filename.c_str(), "rb");
        cout << "filename : " << filename << endl;
        if (i_fp == NULL) {
                cout << "Error : File not found." << endl;
                exit(EXIT_FAILURE);
        }

        //ファイルサイズの取得
        fseek(i_fp, 0L, SEEK_END);
        length = ftell(i_fp);
        cout << "filesize : " << length << " bytes" << endl;

        //メモリ割り当て
        buffer = (uint8_t*)malloc(length);
        if (buffer == NULL) {
                cout << "Error : Cannot allocate memory." << endl;
                exit(EXIT_FAILURE);
        }

        //ファイルの先頭に移動
        fseek(i_fp, 0L, SEEK_SET);

        //データ読み込み
        int n = fread(buffer, sizeof(uint8_t), length, i_fp);
        cout << n << " bytes read." << endl;

        //ファイルクローズ
        fclose(i_fp);
}

//読み込んだファイルを4バイト毎にファイル出力
void MemoryProcessor::writeFile(string filename) {
	//ファイルオープン
	o_fp = fopen(filename.c_str(), "wb");
        cout << "filename : " << filename << endl;
        if (i_fp == NULL) {
                cout << "Error : File not found." << endl;
                exit(EXIT_FAILURE);
        }

	//データ書き出し
	int n = 0;
	int flag = -1;

	for (int i = 0; i < length; i++) {
		//4バイト毎にflagを切り替える
		if (i % ALIGNMENT == 0) {
			flag *= -1;
		}
		//flagが1の間書き出す
		if (flag == 1) {
			n += fwrite(buffer + i, sizeof(uint8_t), sizeof(uint8_t), o_fp);
		}
	}
	cout << n << " bytes write." << endl;
	
	//ファイルクローズ
	fclose(o_fp);
}
