#include "BitMapProcessor.h"
#include <iostream>

int main() {
    BitMapProcessor bmp;

    string filename;
    int reductionRate;

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

    //�C�ӂ̏k�����Ń��T�C�Y���ď����o��
    cout << "�摜�̏k��������͂��Ă��������B�i���j" << endl;
    cin >> reductionRate;

    bmp.nearestNeighborData("output/nearestNeighborImage.bmp", reductionRate);

    cout << "�S�Ă̏������������܂����B" << endl;

    return 0;
}