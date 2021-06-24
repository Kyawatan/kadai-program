#pragma once

#include <iomanip>

#define FILE_TYPE 0x4D42    //BM
#define FILE_HEADER_SIZE 14 //�w�b�_�T�C�Y
#define INFO_HEADER_SIZE 40 //Windows���w�b�_�T�C�Y
#define DEFAULT_HEADER_SIZE (FILE_HEADER_SIZE + INFO_HEADER_SIZE)

using namespace std;

/*
* �t�@�C���w�b�_�\����
*/
#pragma pack(2) //�\���̂̃A���C�����g���E��2byte
typedef struct BITMAPFILEHEADER {
    uint16_t dgType;        //BMP�`����F������}�b�g���ʎq : 2Byte�@0x4D42�i���g���G���f�B�A���j
    uint32_t bfSize;        //�t�@�C���S�̂̃T�C�Y : 4Byte
    uint16_t bfReserved1;   //�\��̈�1�A�����o���͕K��0 : 2Byte
    uint16_t bfReserved2;   //�\��̈�2�A�����o���͕K��0 : 2Byte
    uint32_t bfOffBits;     //�t�@�C���擪����摜���܂ł̃I�t�Z�b�g�o�C�g�� : 4Byte
} BITMAPFILEHEADER;

/*
* ���w�b�_�\����
*/
#pragma pack()  //�f�t�H���g
typedef struct BITMAPINFOHEADER {
    uint32_t biSize;           //���̃w�b�_�̃T�C�Y : 4Byte
    int32_t biWidth;           //�摜�̉������̃s�N�Z���� : 4Byte�@���̂�
    int32_t biHeight;          //�摜�̏c�����̃s�N�Z���� : 4Byte�@���̓{�g���A�b�v
    uint16_t biPlanes;         //�摜�̃v���[�����A�K��1 : 2Byte
    uint16_t biBitCount;       //1�s�N�Z��������̃r�b�g�� : 2Byte�@0,1,4,8,16,24,32
    uint32_t biCompression;    //�s�N�Z���̊i�[���� : 4Byte�@�f�t�H���gBI_RGB
    uint32_t biSizeImage;      //�w�b�_���������摜�̈�̃f�[�^�T�C�Y : 4Byte
    int32_t biXPelsPerMeter;   //�������̉�f���x�A1���[�g��������̃s�N�Z���� : 4Byte
    int32_t biYPelsPerMeter;   //�c�����̉�f���x�A1���[�g��������̃s�N�Z���� : 4Byte
    uint32_t biClrUsed;        //�J���[�p���b�g�����̏ꍇ�Ɏg�p����F���A����ȊO�̏ꍇ��0 : 4Byte
    uint32_t biClrImportant;   //�d�v�ȐF���A0�̏ꍇ��biClrUsed�Ɠ��� : 4Byte
} BITMAPINFOHEADER;

/*
* RGB�\����
*/
typedef struct RGB_24 {
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
} RGB_24;

/*
* �r�b�g�}�b�v�����N���X
*/
class BitMapProcessor {
private:
    uint8_t* buffer;
    uint8_t headerBuffer[DEFAULT_HEADER_SIZE];  //�w�b�_���i�[�p
    RGB_24** imageBuffer;  //RGB�i�[�p
    BITMAPFILEHEADER* fHeader;
    BITMAPINFOHEADER* iHeader;
    RGB_24* rgb24;

public:
    BitMapProcessor();
    ~BitMapProcessor();
    void readData(string);
    void writeData(string);
    void resizeData(string);
    void changeData();

private:
    void readFileHeader();
    void readInfoHeader();
    void readBmpData();
    void writePadding(uint8_t*, int, int);
    void printHeader();
    void updateHeader();
};