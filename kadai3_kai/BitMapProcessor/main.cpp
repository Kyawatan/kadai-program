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
    cout << filename << ".bmp��ǂݍ��݂܂����B\n" << endl;

    //�R�s�[�������o��
    filename = "copyImage.bmp";
    bmp.writeData("output/" + filename);
    cout << "�R�s�[����" << filename << "�������o���܂����B\n" << endl;

    //RGB�����ւ��ď����o��
    filename = "changeImage.bmp";
    bmp.changeData("output/" + filename);
    cout << "RGB�����ւ���" << filename << "�������o���܂����B\n" << endl;

    //�����Ƀ��T�C�Y���ď����o��
    filename = "resizeImage.bmp";
    bmp.resizeData("output/" + filename);
    cout << "50���ɏk������" << filename << "�������o���܂����B\n" << endl;

    //�C�ӂ̏k�����Ń��T�C�Y���ď����o��
    cout << "�摜�̏k��������͂��Ă��������B�i1(��)�`99(��)�̐����l�j" << endl;
    cin >> reductionRate;

    filename = "nearestNeighborImage.bmp";
    bmp.nearestNeighborData("output/" + filename, reductionRate);
    cout << reductionRate << "���ɏk������" << filename << "�������o���܂����B\n" << endl;

    cout << "�S�Ă̏������������܂����B" << endl;

    return 0;
}