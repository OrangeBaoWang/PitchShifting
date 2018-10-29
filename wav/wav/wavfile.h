#include <stdafx.h>
#include <iostream>
#include <fstream>
using namespace  std;
class wavfile
{
public:
	unsigned short cahnnel;			// ������
	unsigned int frequency;			// ����Ƶ��
	unsigned int byte_rate;			// �����ʣ�frequency * cahnnel * bit_per_sample /8
	unsigned short bit_per_sample;	// ����λ�� 8λ����16 24��
	unsigned int file_size;			// �ļ���С
	unsigned long data_size;		// ʵ�������ļ���С���ֽ���ʽ��
	unsigned char *original_data;	// ʵ�ʴ洢������ �����ֽ���ʽ����ת��)
	double *norm_data;				// ��һ������
	double *data;					// default output is mono data
	unsigned long len;				// ʵ�ʵ���,data_size/(bit_per_sample/8) /channel
	double duration;				// ����ʱ��(second)
	bool is_open;

	wavfile();
	~wavfile();
	int read(std::string file_neme);		// read the wav file
	void show();					// show wav file message
	int write(std::string file_name);	// write data to wav file
private:

};

wavfile::wavfile()
{
	// ���캯�� �൱�ڳ�ʼ��

}

wavfile::~wavfile()
{
	// �������� �ͷ���Դ
	this->is_open = false;
}

int wavfile::read(std::string file_neme)
{
	ifstream fp;																	// ����ʱ ifstream
	fp.open(file_neme, ios::in | ios::binary);										// ��ȡ�ļ� try except
	if (!fp.is_open()) {
		std::cout << "Can not open it." << endl;
		fp.close();
		return false;
	}
	fp.seekg(0, ios::end);								// seek get
	this->file_size = fp.tellg();						// �Ӻ���ǰ��������������ļ��Ĵ�С(�ֽ���ʽ)
	fp.seekg(0x16);
	fp.read((char*)&this->cahnnel, 2);					// ����
	fp.seekg(0x18);										// offset 24 is num channels
	fp.read((char *)&this->frequency, 4);				// ����Ƶ��
	fp.read((char*)&this->byte_rate, 4);				// ������
	fp.seekg(0x22);
	fp.read((char*)&this->bit_per_sample, 2);			// ����λ��	
	fp.seekg(0x28);
	fp.read((char*)&this->data_size, 4);

	this->original_data = new unsigned  char[this->data_size];// �������ݿռ����

	fp.seekg(0x2c);										// 44��ʼ��data��Ķ���
	fp.read((char*)this->original_data, this->data_size);

	this->norm_data = new double[this->data_size / 2];
	for (unsigned long win_len = 0; win_len < this->data_size; win_len = win_len + 2) {
		long temp_value = 0;
		if ((this->original_data[win_len + 1] >> 7) == 1) {								// �ж�������,x86ϵͳ����С��ģʽ����λ�͵�ַ����λ�ߵ�ַ�����ֽ�
			temp_value = this->original_data[win_len] | (this->original_data[win_len + 1] << 8) - 65536;
		}
		else {
			temp_value = this->original_data[win_len] | this->original_data[win_len + 1] << 8;			// ����
		}
		this->norm_data[win_len / 2] = (double)temp_value / 32768.0;									// ��һ��

	}

	// output data

	if (this->cahnnel > 1) {
		this->data = new double[this->data_size / 4];
		long count = 0;
		for (long win_len = 0; win_len < this->data_size / 4; win_len += 2) {
			this->data[count++] = (this->norm_data[win_len] + this->norm_data[win_len + 1]) / 2.0;
		}
	}
	else
		this->data = this->norm_data;
	fp.close();
	this->len = this->data_size / 4;
	this->duration = this->len / this->frequency;
	this->is_open = true;
	return true;
}

void wavfile::show()
{
	if (this->is_open) {
		std::cout << "The Wav file message:" << endl
			<< "Frequency	: " << this->frequency << " Hz" << endl
			<< "Bit_per_sample	: " << this->bit_per_sample << " bps" << endl
			<< "Size	: " << this->len << " samples" << endl
			<< "Duration: " << this->duration << " seconds" << endl;
	}
	else
		std::cout << "Have not open any wav file." << endl;
}

int wavfile::write(std::string file_name)
{
	ofstream fp;
	fp.open(file_name, ios::out || ios::binary);
	//fp.write()
	return 0;
}
