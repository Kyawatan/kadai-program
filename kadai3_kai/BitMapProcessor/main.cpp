#include "BitMapProcessor.h"
#include <iostream>

int main() {
    BitMapProcessor bmp;

    string filename;
    int select;

    //�t�@�C���̓ǂݍ���
    cout << "�ǂݍ���BMP�t�@�C��������͂��Ă��������B�i�g���q�s�v�j" << endl;
    cin >> filename;
    bmp.readData("image/" + filename + ".bmp");

    cout << filename << ".bmp��ǂݍ��݂܂����B" << endl;

    //�R�s�[�������o��
    bmp.writeData("output/copyImage.bmp");

    //RGB�����ւ��ď����o��
    bmp.changeData("output/changeImage.bmp");

    //�����Ƀ��T�C�Y���ď����o��
    bmp.resizeData("output/resizeImage.bmp");

    cout << "�������������܂����B" << endl;

    return 0;
}