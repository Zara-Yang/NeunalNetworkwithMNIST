#pragma once
#include<ctime>
#include<fstream>
#include"define.h"
#include"include.h"
#include"MatrixOperation.h"

enum DataType {TRAIN,VALID,TEST};

uint32_t swap_endian(uint32_t val) {
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return((val << 16) | (val >> 16));
}
template<typename T>
struct DataSet {
	Matrix<T> TrainData;
	Matrix<T> ValidData;
	Matrix<T> TestData;

	DataSet() {}
	~DataSet() {}
	void GetBatchData(int* StartIndex,bool* EpochFinish,Matrix<T>* data, Matrix<T>* label) {
		int EndIndex = *StartIndex + BatchSize;
		if ( EndIndex >= TrainData.Nrow) {
			*EpochFinish = true;
			EndIndex = TrainData.Nrow;
		}
		for (int i = *StartIndex; i < EndIndex; i++) {
			for (int j = 0; j < TrainData.Ncol; j++) {
				data->matrix[i - *StartIndex][j] = TrainData.matrix[i][j];
			}
			for (int j = 0; j < LabelType; j++) {
				if (j != TrainData.matrix[i][Row * Col]) {
					label->matrix[i - *StartIndex][j] = 0;
				}
				else {
					label->matrix[i - *StartIndex][j] = 1;
				}
			}
		}
		*StartIndex = EndIndex;
 	}
};
template<typename T>
Matrix<T> ReadMnistData(string& dataPath, string& labelPath) {
	/// <summary>
	/// 读取MNIST数据，直接使用并没有保存
	/// </summary>
	/// <param name="dataPath"></param>
	/// <param name="labelPath"></param>
	/// <returns></returns>
	ifstream datafile;
	datafile.open(dataPath, ios::in | ios::binary);
	if (!datafile.is_open()) {
		cout << "Reading data fail !" << endl;
	}
	ifstream labelfile;
	labelfile.open(labelPath, ios::in | ios::binary);
	if (!datafile.is_open()) {
		cout << "Reading label fail !" << endl;
	}

	uint32_t magic;
	uint32_t num_items;
	uint32_t num_label;
	uint32_t rows;
	uint32_t cols;
	// 读取魔术数（啥是魔术数？）
	datafile.read(reinterpret_cast<char*>(&magic), 4);
	magic = swap_endian(magic);
	labelfile.read(reinterpret_cast<char*>(&magic), 4);
	magic = swap_endian(magic);
	// 读取数据数量
	datafile.read(reinterpret_cast<char*>(&num_items), 4);
	num_items = swap_endian(num_items);
	labelfile.read(reinterpret_cast<char*>(&num_label), 4);
	num_label = swap_endian(num_label);
	if (num_items != num_label) {
		cout << "Data number dosen't match !" << endl;
	}
	// 读取图片大小
	datafile.read(reinterpret_cast<char*>(&rows), 4);
	rows = swap_endian(rows);
	datafile.read(reinterpret_cast<char*>(&cols), 4);
	cols = swap_endian(cols);

	Matrix<T> ResultMatrix(num_items, rows * cols + 1, 0);

	for (int n = 0; n != num_items; n++) {
		unsigned char Label = 0;
		Matrix<T> Image(Row, Col);
		for (int i = 0; i < Row; i++) {
			for (int j = 0; j < Col; j++) {
				unsigned char image = 0;
				datafile.read((char*)&image, sizeof(image));
				Image.matrix[i][j] = image;
				ResultMatrix.matrix[n][i * rows + j] = image;
			}
		}
		labelfile.read((char*)&Label, sizeof(Label));
		ResultMatrix.matrix[n][rows * cols] = (T)Label;
	}
	return(ResultMatrix);
}
template<typename T>
void ExchangeRow(Matrix<T>* M, int row1,int row2) {
	T* buffer = M->matrix[row1];
	M->matrix[row1] = M->matrix[row2];
	M->matrix[row2] = buffer;
}
template<typename T>
void MatrixRowShuffle(Matrix<T>* M) {
	srand((int)time(0));
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < M->Nrow; j++) {
			int randPosition = rand() % (M->Nrow);
			ExchangeRow(M, j, randPosition);
		}
	}
}
template<typename T>
DataSet<T> DataSplit(Matrix<T>& TotalTrainData,Matrix<T>& TotalTestData) {
	DataSet<T> dataset;
	dataset.TrainData = TotalTrainData.MatrixRowSplit(0, TrainDataNum - ValiNum);
	dataset.ValidData = TotalTrainData.MatrixRowSplit(TrainDataNum - ValiNum, TrainDataNum);
	dataset.TestData = TotalTestData;
	return(dataset);
}


template<typename T> 
//DataSet<T>* 
DataSet<T> DataPretreatment(string& TrainDataPath,string& TestDataPath,string& TrainLabelPath,string& TestLabelPath) {
	// 读取训练和测试数据
	Matrix<T> TotalTrainData = ReadMnistData<T>(TrainDataPath, TrainLabelPath);
	Matrix<T> TotalTestData = ReadMnistData<T>(TestDataPath, TestLabelPath);
	MatrixRowShuffle(&TotalTrainData);
	DataSet<T> data = DataSplit(TotalTrainData, TotalTestData);
	return(data);


}



