#include "BitMapProcessor.h"

int main() {
    BitMapProcessor bmp;

    //�t�@�C���̓ǂݍ���
    bmp.readData("image/flower.bmp");

    //�����Ƀ��T�C�Y
    bmp.resizeData("output/resizeImage.bmp");

    //RGB�����ւ���
    //bmp.changeData();

    //�t�@�C���̏����o��
    //bmp.writeData("output/outputImage.bmp");

    return 0;
}