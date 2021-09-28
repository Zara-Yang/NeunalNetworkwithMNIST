#pragma once
#include"define.h"
#include<cmath>
#include"MatrixOperation.h"

template <typename T>
T MseLoss(Matrix<T>* Value, Matrix<T>* Target) {
	Matrix<T> Buffer = (*Target - *Value);
	Buffer.Sqr();
	return(Buffer.Mean());
}
template <typename T>
Matrix<T> MseLossPrime(Matrix<T>* Target, Matrix<T>* Value) {
	return(*Target - *Value);
}
template <typename T>
T Tanh(T* x) {
	return((pow(E, *x) - pow(E, -*x)) / (pow(E, *x) + pow(E, -*x)));
}
template <typename T>
T TanhPrime(T* x) {
	return(1 - pow(Tanh(x), 2));
}
template <typename T>
T Linear(T* x) {
	return(*x);
}
template <typename T>
T LinearPrime(T* x) {
	return(1.);
}
template <typename T>
Matrix<T> MatrixCalc(Matrix<T> &M, T (*func)(T*)) {
	/// <summary>
	/// 对矩阵中每一个值调用一遍该函数，用于神经网络数据矩阵的激活
	/// </summary>
	/// <param name="M"></param>
	/// <param name="func"></param>
	/// <returns></returns>
	Matrix<T> buffer(M.Nrow, M.Ncol);
	for (int i = 0; i < M.Nrow; i++) {
		for (int j = 0; j < M.Ncol; j++) {
			buffer.matrix[i][j] = func(&M.matrix[i][j]);
		}
	}
	return(buffer);
}
template <typename T>
struct Layer {

	Matrix<T> weight;
	Matrix<T> bia;
	Matrix<T> weightDev;
	Matrix<T> biaDev;
 	Matrix<T> dataIn;
	Matrix<T> dataOut;
	Matrix<T> deltaOut;
	Matrix<T> delta;

	Layer() {}
	~Layer() {}

	Layer(int weightRowSize, int weightColSize, int weightInit, int biaInit){
		weight = Matrix<T>(weightRowSize, weightColSize, weightInit);
		bia = Matrix<T>(weightRowSize, 1, biaInit);
	}

	void ForwardStep(Matrix<T>* data, T (*func)(T*)) {
		/// <summary>
		/// 单层的前向传播
		/// </summary>
		/// <param name="data"></param>
		/// <param name="func"></param>
		Matrix<T> a = *data;
		dataIn = *data;
		deltaOut = weight * dataIn + bia;
		dataOut = MatrixCalc(deltaOut, func);
	}

	void BackwardStep(Matrix<T>& input , T (*ActivePrime)(T*)) {
		Matrix<T> temp = MatrixCalc(deltaOut,ActivePrime);
		delta = temp.SchurProduct(input);
		biaDev = delta*alpha;
		weightDev = delta * dataIn.Trans()*alpha;
	}
	void LearningParam() {
		weight -= weightDev;
		bia -= biaDev;
	}


};
template <typename T>
class NetWork {
private:
	int* LayerSize;
	Layer<T> Layers[HiddenNum + OutputNum];
public:
	NetWork() {}
	~NetWork() {}
	NetWork(int* ls)
		:LayerSize(ls) {
		cout << "New Network has been Created !" << endl;
		for (int i = 0; i < HiddenNum + OutputNum; i++) {
			Layers[i] = Layer<T>(LayerSize[i+1], LayerSize[i], 1, 0);
		}
	}
	
	void DisplayInfo() {
		for (int i = 0; i < Depth; i++) {
			cout << "Weight in layer " << i << endl;
			cout << Layers[i].weight << endl;
			cout << "Delta in layer " << i << endl;
			cout << Layers[i].delta << endl;
			cout << "DeltaOut in layer " << i << endl;
			cout << Layers[i].deltaOut << endl;
 		}
	}
	
	void FeedForward(Matrix<T>* data) {
		/// <summary>
		/// 全神经网络前向传播
		/// </summary>
		/// <param name="data"></param>
		cout << "Compute Layer 0 " << endl;
		Layers[0].ForwardStep(data, Linear);
		for (int i = 1; i < Depth; i++) {
			cout << "Compute Layer " << i << endl;
			Layers[i].ForwardStep(&Layers[i - 1].dataOut, Linear);
		}
	}

	double ComputeLoss(Matrix<T>* Lable, T (*func)(Matrix<T>*, Matrix<T>*)) {
		/// <summary>
		/// 计算损失函数的数值
		/// </summary>
		/// <param name="Lable"></param>
		/// <param name="func"></param>
		/// <returns></returns>
		return(func(&Layers[Depth - 1].dataOut,Lable));
	}
	
	void Backward(Matrix<T>* Lable, Matrix<T>(*LossPrimefunc)(Matrix<T>*, Matrix<T>*)) {
		Matrix<T> LossPrime = LossPrimefunc(Lable, &Layers[Depth - 1].dataOut);
		LossPrime = LossPrime.Mean(0);
		Layers[Depth - 1].BackwardStep(LossPrime, LinearPrime);
		for (int i = Depth - 2; i >= 0; i--) {
			Matrix<T> temp = Layers[i + 1].weight.Trans() * Layers[i+1].delta;
			Layers[i].BackwardStep(temp, LinearPrime);
		}
		for (int i = Depth - 1; i >= 0; i--) {
			Layers[i].LearningParam();
		}
	}

	void BatchRun(Matrix<T>* BatchData, Matrix<T>* Label) {
		FeedForward(BatchData);
		ComputeLoss(Label, MseLoss);
		// DisplayInfo();
		Backward(Label,MseLossPrime);
		DisplayInfo();

	}


};