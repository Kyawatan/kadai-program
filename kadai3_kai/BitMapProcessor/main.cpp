#include "BitMapProcessor.h"

int main() {
    BitMapProcessor bmp;

    //�t�@�C���̓ǂݍ���
    bmp.readData("image/Parrots.bmp");

    //�R�s�[�������o��
    bmp.writeData("output/copyImage.bmp");

    //RGB�����ւ��ď����o��
    //bmp.changeData("output/changeImage.bmp");

    //�����Ƀ��T�C�Y���ď����o��
    bmp.resizeData("output/resizeImage.bmp");

    return 0;
}