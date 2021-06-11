#include <iostream>
#include <iomanip>

#define I_FILE "flower.png"
#define O_FILE "k2-2.txt"

using namespace std;

int main() {
	FILE *i_fp;
	long length;
	int *buffer;

	//ファイルオープン
	i_fp = fopen(I_FILE, "rb");
	cout << "filename : " << I_FILE << endl;
	if (i_fp == NULL) {
		cout << "Error : File not found." << endl;
		return -1;
	}

	//ファイルサイズの取得
	fseek(i_fp, 0L, SEEK_END);
	length = ftell(i_fp);
	cout << "filesize : " << length << " bytes" << endl;

	//メモリ割り当て
	buffer = (int *)malloc(length);
	if (buffer == NULL) {
		cout << "Error : Cannot allocate memory." << endl;
		return -1;
	}

	//ファイルの先頭に移動
	fseek(i_fp, 0L, SEEK_SET);
	
	//データ読み込み
	fread(buffer, sizeof(*buffer), length/sizeof(*buffer), i_fp);
	cout << "File read." << endl;
	
	//ファイルクローズ
        fclose(i_fp);


	//読み込んだファイルを4バイト毎にファイル出力
	FILE *o_fp = fopen(O_FILE, "w");
	int i;

	cout << "filename : " << O_FILE << endl;

	for (i=0; i<length/(int)sizeof(*buffer); i++) {
		if (i % 2 == 0) {
			//iが偶数のとき書き込む
			fwrite(buffer+i, sizeof(*buffer), 1, o_fp);
		}
	}

	cout << "File write." << endl;

	fclose(o_fp);

	return 0;
}

/*実行結果

$ ./out
filename : flower.png
filesize : 4766 bytes
File read.
filename : k2-2.txt
File write.
 

元のバイナリファイル

$ od -tx -N 40 flower.png
0000000 474e5089 0a1a0a0d 0d000000 52444849
0000020 64000000 64000000 00000608 95e27000
0000040 12000054 41444965
0000050

間引いたバイナリファイル

$ od -tx -N 40 k2-2.txt 
0000000 474e5089 0d000000 64000000 00000608
0000020 12000054 ed017854 c9feb9d5 810094f3
0000040 10053cc2 aad443ea
0000050

*/
