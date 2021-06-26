#include "kadai2.h"

#include <iostream>

using namespace std;

int main() {
	MemoryProcessor a;

	string i_filename;
	string o_filename = "output.bin";

	cout << "ファイル名を入力 : ";
        cin >> i_filename;

	a.readFile("binaryfiles/" + i_filename);

	a.writeFile(o_filename);

	return 0;
}

/*実行結果

$ g++ -Wall -o out kadai2.cpp main.cpp
$ ./out
ファイル名を入力 : bin10.bin
filename : binaryfiles/bin10.bin
filesize : 10 bytes
10 bytes read.
filename : output.bin
6 bytes write.

$ xxd -g4 output.bin
00000000: 00112233 8899                        .."3..

$ xxd -g4 binaryfiles/bin10.bin
00000000: 00112233 44556677 8899               .."3DUfw..


$ ./out
ファイル名を入力 : bin16.bin
filename : binaryfiles/binary.bin
filesize : 16 bytes
16 bytes read.
filename : output.bin
8 bytes write.

$ xxd -g4 output.bin
00000000: 00112233 8899aabb                    .."3....

$ xxd -g4 binaryfiles/bin16.bin
00000000: 00112233 44556677 8899aabb ccddeeff  .."3DUfw........

*/
