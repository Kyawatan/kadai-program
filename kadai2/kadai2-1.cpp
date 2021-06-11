#include <iostream>

#define FILENAME "binary.bin"

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
	fseek(fp, 0, SEEK_END);
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

	//データ読み込み
	fread(buffer, sizeof(*buffer), sizeof(buffer)/sizeof(*buffer), fp);
	cout << "File read." << endl;
	
	//読み込んだデータの出力
	int i;
	for (i=0; i<(sizeof(buffer)/sizeof(*buffer)-1); i++) {
		cout << hex << *(buffer + i) << " ";
	}
	cout << endl;

	//ファイルクローズ
	fclose(fp);

	return 0;
}

/*実行結果
$ ./out
filename : binary.bin
filesize : 4 bytes
File read.
efbeadde

char型でメモリ確保した場合

$ ./out
filename : binary2.bin
filesize : 7 bytes
File read.
A B C D E F G 
*/
