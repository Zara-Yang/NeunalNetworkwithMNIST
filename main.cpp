#include"include.h"
#include"MatrixOperation.h"
#include"NeualNetwork.h"
#include"DataPretreatmentEdit.h"

int main() {
	/*
	int LayerSize[3] = { 3,5,4 };
	Matrix<double> InputData(3, 1, 1);
	Matrix<double> Target(4, 1, 16);
	cout << InputData << endl;

	NetWork NN(LayerSize);
	NN.BatchRun(&InputData, &Target);
			*/
	// ��ȡ��׼������
	string TrainDataPath = "D:/data/cpp_project/MNISTdata/train-images.idx3-ubyte";
	string TrainLabelPath = "D:/data/cpp_project/MNISTdata/train-labels.idx1-ubyte";

	string TestDataPath = "D:/data/cpp_project/MNISTdata/t10k-images.idx3-ubyte";
	string TestLabelPath = "D:/data/cpp_project/MNISTdata/t10k-labels.idx1-ubyte";
	//����Ԥ����
	DataSet<float> data = DataPretreatment<float>(TrainDataPath, TestDataPath, TrainLabelPath, TestLabelPath);
	// �������
	int Layers[] = { 784,500,200,100,10 };
	NetWork<float> NN(Layers);

	Matrix<float> d(784, 1000, 1);
	//NN.FeedForward(&d);


	
	//��ʼѵ��
	int IndexBuffer = 0;
	bool EpochFinish = false;
	Matrix<float> BatchData(BatchSize, Row * Col, 0);
	Matrix<float> BatchLabel(BatchSize, LabelType, 0);
	while (!EpochFinish) {
		cout << IndexBuffer << " : " << EpochFinish << endl;
		data.GetBatchData(&IndexBuffer, &EpochFinish, &BatchData, &BatchLabel);
		Matrix<float>BD = BatchData.Trans();

		NN.FeedForward(&BD);
		NN.DisplayInfo()

	}
	
}	

