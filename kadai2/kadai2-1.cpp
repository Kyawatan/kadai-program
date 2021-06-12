#include <iostream>
#include <iomanip>

#define FILENAME "bin3.bin"

using namespace std;

int main() {
	FILE *fp;
	long length;
	int *buffer;

	//ファイルオープン
	fp = fopen(FILENAME, "rb");
	cout << "filename : " << FILENAME << endl;
	if (fp == NULL) {
		cout << "Error : File not found." << endl;
		return -1;
	}

	//ファイルサイズの取得
	fseek(fp, 0L, SEEK_END);
	length = ftell(fp);
	cout << "filesize : " << length << " bytes" << endl;

	//メモリ割り当て
	buffer = (int *)malloc(length);
	if (buffer == NULL) {
		cout << "Error : Cannot allocate memory." << endl;
		return -1;
	}

	//ファイルの先頭に移動
	fseek(fp, 0L, SEEK_SET);

	//ファイルサイズがが4バイト未満の場合の読み込みデータ数の処理
	int n;
	if (length < 4) n = 1;
	else  n = length/sizeof(*buffer);

	//データ読み込み
	fread(buffer, sizeof(*buffer), n, fp);
	cout << "File read." << endl;

	//読み込んだデータを16進数で出力(先頭から40バイトだけ)
	int i;
	for (i=0; i<10; i++) {
		cout << hex << setw(8) << setfill('0') << *(buffer + i) << " ";
	}
	cout << endl;

	//ファイルクローズ
	fclose(fp);

	return 0;
}

/*実行結果

$ g++ -Wall -o out kadai2-1.cpp 
$ ./out
filename : flower.png
filesize : 4766 bytes
File read.
474e5089 0a1a0a0d 0d000000 52444849 64000000 64000000 00000608 95e27000 12000054 41444965 


確認：画像の先頭から40バイトを16進数で表示

$ od -tx -N 40 flower.png
0000000 474e5089 0a1a0a0d 0d000000 52444849
0000020 64000000 64000000 00000608 95e27000
0000040 12000054 41444965
0000050


追記：ファイルサイズが3バイトの場合

$ ./out
filename : bin3.bin
filesize : 3 bytes
File read.
00221100 00000000 00000000 00000000 00000000 00000000 0000db31 00000000 00000000 00000000 

*/
