#pragma once
#include"define.h"
#include"include.h"


template <typename T>
class Matrix {
public:
	int Nrow, Ncol;
	T** matrix;

	void MatrixInit() {
		matrix = new T * [Nrow];
		for (int i = 0; i < Nrow; i++) {
			matrix[i] = new T[Ncol];
		}
	}
	Matrix():Matrix(1, 1, 1) {}
	~Matrix() {
		for (int i = 0; i < Nrow; i++) {
		delete[] matrix[i];
	 	}
	}
	Matrix(int rs, int cs, T num)
		:Nrow(rs), Ncol(cs) {
		MatrixInit();
		for (int i = 0; i < Nrow; i++) {
			for (int j = 0; j < Ncol; j++) {
				matrix[i][j] = num;
			}
		}
	}
	Matrix(int rs, int cs)
		:Matrix(rs, cs, 0) {
	}
	Matrix(const Matrix<T>& m) {
		Nrow = m.Nrow;
		Ncol = m.Ncol;
		MatrixInit();
		for (int i = 0; i < Nrow; i++) {
			for (int j = 0; j < Ncol; j++) {
				matrix[i][j] = m.matrix[i][j];
			}
		}
	}
	void operator=(const Matrix<T>& m) {
		if (Nrow != m.Nrow || Ncol != m.Ncol) {
			for (int i = 0; i < Nrow; i++) {
				delete[] matrix[i];
			}
			delete[] matrix;
			Nrow = m.Nrow;
			Ncol = m.Ncol;
			MatrixInit();
		}
		for (int i = 0; i < Nrow; i++) {
			for (int j = 0; j < Ncol; j++) {
				matrix[i][j] = m.matrix[i][j];
			}
		}
	}
	Matrix<T> operator+(const Matrix<T>& m) const {
		Matrix<T> temp(Nrow, Ncol);
		if (m.Ncol == Ncol && m.Nrow == Nrow) {
			for (int i = 0; i < Nrow; i++) {
				for (int j = 0; j < Ncol; j++) {
					temp.matrix[i][j] = matrix[i][j] + m.matrix[i][j];
				}
			}
		}
		else if (m.Ncol == 1 && m.Nrow == Nrow) {
			for (int i = 0; i < Nrow; i++) {
				for (int j = 0; j < Ncol; j++) {
					temp.matrix[i][j] = matrix[i][j] + m.matrix[i][0];
				}
			}
		}
		else if (m.Ncol == Nrow && m.Nrow == 1) {
			for (int i = 0; i < Nrow; i++) {
				for (int j = 0; j < Ncol; j++) {
					temp.matrix[i][j] = matrix[i][j] + m.matrix[0][j];
				}
			}
		}
		else {
			cout << "Matrix Shape Error in Add !" << endl;
			exit(-1);
		}
		return(temp);
	}
	void operator+=(const Matrix<T>& m) {
		for (int i = 0; i < Nrow; i++) {
			for (int j = 0; j < Ncol; j++) {
				matrix[i][j] += m.matrix[i][j];
			}
		}
	}
	Matrix<T> operator-(const Matrix<T>& m) const {
		Matrix<T> temp(Nrow, Ncol);
		for (int i = 0; i < Nrow; i++) {
			for (int j = 0; j < Ncol; j++) {
				temp.matrix[i][j] = matrix[i][j] - m.matrix[i][j];
			}
		}
		return(temp);
	}
	void operator-=(const Matrix<T>& m) {
		for (int i = 0; i < Nrow; i++) {
			for (int j = 0; j < Ncol; j++) {
				matrix[i][j] -= m.matrix[i][j];
			}
		}
	}
	Matrix<T> operator*(const Matrix<T>& m) const {
		Matrix<T> temp(Nrow, m.Ncol);
		for (int i = 0; i < temp.Nrow; i++) {
			for (int j = 0; j < temp.Ncol; j++) {
				for (int k = 0; k < Ncol; k++) {
					temp.matrix[i][j] += (matrix[i][k] * m.matrix[k][j]);
				}
			}
		}
		return temp;
	}
	void operator*=(const Matrix<T>& m) {
		Matrix<T> temp(Nrow, m.Ncol);
		for (int i = 0; i < temp.Nrow; i++) {
			for (int j = 0; j < temp.Ncol; j++) {
				for (int k = 0; k < Ncol; k++) {
					temp.matrix[i][j] += (matrix[i][k] * m.matrix[k][j]);
				}
			}
		}
		*this = temp;
	}
	Matrix<T> operator*(T m) {
		Matrix<T> temp(Nrow, Ncol);
		for (int i = 0; i < Nrow; i++) {
			for (int j = 0; j < Ncol; j++) {
				temp.matrix[i][j] = matrix[i][j] * m;
			}
		}
		return(temp);
	}
	void operator*=(T m) {
		for (int i = 0; i < Nrow; i++) {
			for (int j = 0; j < Ncol; j++) {
				matrix[i][j] *= m;
			}
		}
	}
	Matrix<T> Trans() const {
		Matrix<T> temp(Ncol, Nrow);
		for (int i = 0; i < Nrow; i++) {
			for (int j = 0; j < Ncol; j++) {
				temp.matrix[j][i] = matrix[i][j];
			}
		}
		return(temp);
	}
	Matrix<T> Sqr() {
		Matrix<T> r(Nrow, Ncol, 0);
		for (int i = 0; i < Nrow; i++) {
			for (int j = 0; j < Ncol; j++) {
				r.matrix[i][j] = pow(matrix[i][j], 2);
			}
		}
		return(r);
	}
	T Mean() {
		T buffer = 0;
		for (int i = 0; i < Nrow; i++) {
			for (int j = 0; j < Ncol; j++) {
				buffer += matrix[i][j];
			}
		}
		return(buffer / (Nrow * Ncol));
	}
	Matrix<T> Mean(int axis) {
		cout << *this << endl;
		Matrix<T> temp(Nrow, 1, 0);
		for (int i = 0; i < Nrow; i++) {
			for (int j = 0; j < Ncol; j++) {
				temp.matrix[i][0] = temp.matrix[i][0] + matrix[i][j];
			}
			temp.matrix[i][0] /= Ncol;
		}
		return(temp);
	}
	Matrix<T> SchurProduct(Matrix<T>& b) {
		Matrix<T> r(Nrow, Ncol, 1);
		for (int i = 0; i < Nrow; i++) {
			for (int j = 0; j < Ncol; j++) {
				r.matrix[i][j] = matrix[i][j] * b.matrix[i][j];
			}
		}
		return(r);
	}
	Matrix<T> Flattern() {
		Matrix<T> temp(1,Nrow*Ncol,0);
		for (int i = 0; i < Nrow; i++) {
			for (int j = 0; j < Ncol; j++) {
				temp.matrix[0][i * Nrow + j] = matrix[i][j];
			}
		}
		return(temp);
	}
	Matrix<T> MatrixRowSplit(int RowStart, int RowEnd) {
		Matrix<T> temp(RowEnd - RowStart, Ncol, 0);
		for (int i = RowStart; i < RowEnd; i++) {
			for (int j = 0; j < Ncol; j++) {
				temp.matrix[i - RowStart][j] = matrix[i][j];
			}
		}
		return(temp);
	}
	Matrix<T> Reshape(int shapeX, int shapeY) {
		// 看啥看！还没写呢！！
	}
	


};


template<typename T>
Matrix<T> Concatenate(Matrix<T>* MatrixList, int MatrixNumber,int axis) {
	if (axis == 0) {
		int TotalRow = 0;
		int TotalCol = MatrixList->Ncol;
		for (int n = 0; n < MatrixNumber; n++) {
			TotalRow += MatrixList[n].Nrow;
			if (MatrixList[n].Ncol != TotalCol) {
				cout << " Concatenate Size Doesn't Match ! " << endl;
			}

		}
		Matrix<T> temp(TotalRow, TotalCol, 0);
		int RowPoint = 0;
		for (int n = 0; n < MatrixNumber; n++) {
			for (int i = 0; i < MatrixList[n].Nrow; i++) {
				for (int j = 0; j < MatrixList[n].Ncol; j++) {
					temp.matrix[RowPoint + i][j] = MatrixList[n].matrix[i][j];
				}
			}
			RowPoint += MatrixList[n].Nrow;
		}
		return(temp);
	}
	if (axis == 1) {
		int TotalRow = MatrixList->Nrow;
		int TotalCol = 0;
		for (int n = 0; n < MatrixNumber; n++) {
			TotalCol += MatrixList[n].Ncol;
			if (MatrixList[n].Nrow != TotalRow) {
				cout << " Concatenate Size Doesn't Match ! " << endl;
			}

		}
		Matrix<T> temp(TotalRow, TotalCol, 0);
		int ColPoint = 0;
		for (int n = 0; n < MatrixNumber; n++) {
			for (int i = 0; i < MatrixList[n].Nrow; i++) {
				for (int j = 0; j < MatrixList[n].Ncol; j++) {
					temp.matrix[i][ColPoint+ j] = MatrixList[n].matrix[i][j];
				}
			}
			ColPoint += MatrixList[n].Ncol;
		}
		return(temp);
	}
}


template <typename T>
ostream& operator<<(ostream& stream, Matrix<T>& a) {
	for (int i = 0; i < a.Nrow; i++) {
		for (int j = 0; j < a.Ncol; j++) {
			stream << a.matrix[i][j] << "  ";
		}
		stream << endl;
	}
	return(stream);
}

