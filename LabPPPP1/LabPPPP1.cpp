#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

class matrixSlicer //class for pseudo slicing matrixes into 4
{
	int** matrix;
	int rowSize;
	int columnSize;
	int extendedDim;

public:
	matrixSlicer(int** Matrix, int rowSize, int columnSize, int extendedDim)
	{
		matrix = Matrix;
		this->rowSize = rowSize;
		this->columnSize = columnSize;
		this->extendedDim = extendedDim;
	}

	int get(int matrixNum, int i, int j)
	{
		if (matrixNum == 2 || matrixNum == 4) //second column in quarters
			j += extendedDim / 2;

		if (matrixNum == 3 || matrixNum == 4) //second row in quarters
			i += extendedDim / 2;

		if (i < rowSize && j < columnSize)
		{
			return matrix[i][j];
		}
		else //extending with zeros
		{
			return 0;
		}
	}

	int get(int i, int j)
	{
		if (i < rowSize && j < columnSize)
		{
			return matrix[i][j];
		}
		else //extending with zeros
		{
			return 0;
		}
	}
};


class pMatrix //class for computing transitional matrixes and result matrix
{
	vector<int**> P;
	int extendedDim;
public:
	pMatrix(matrixSlicer sMatrix1, matrixSlicer sMatrix2, int extendedDim)
	{
		this->extendedDim = extendedDim;

		//memory allocation for P matrixes
		for (int i = 0; i < 7; i++)
		{
			P.push_back(new int* [extendedDim / 2]);
			for (int j = 0; j < extendedDim / 2; j++)
			{
				//cout << i << " " << j << "\n";
				P[i][j] = new int[extendedDim / 2];
			}
		}

		//computing P1 ... P7
		for (int i = 0; i < extendedDim / 2; i++)
		{
			for (int j = 0; j < extendedDim / 2; j++)
			{
				for (int num = 0; num < 7; num++)
				{
					P[num][i][j] = 0;
					switch (num + 1)
					{
					case 1:
						for (int z = 0; z < extendedDim / 2; z++)
							P[num][i][j] += (sMatrix1.get(1, i, z) + sMatrix1.get(4, i, z))
											* (sMatrix2.get(1, z, j) + sMatrix2.get(4, z, j));
						break;
					case 2:
						for (int z = 0; z < extendedDim / 2; z++)
							P[num][i][j] += (sMatrix1.get(3, i, z) + sMatrix1.get(4, i, z))
											* sMatrix2.get(1, z, j);
						break;
					case 3:
						for (int z = 0; z < extendedDim / 2; z++)
							P[num][i][j] += sMatrix1.get(1, i, z) 
											* (sMatrix2.get(2, z, j) - sMatrix2.get(4, z, j));
						break;
					case 4:
						for (int z = 0; z < extendedDim / 2; z++)
							P[num][i][j] += sMatrix1.get(4, i, z) 
											* (sMatrix2.get(3, z, j) - sMatrix2.get(1, z, j));
						break;
					case 5:
						for (int z = 0; z < extendedDim / 2; z++)
							P[num][i][j] += (sMatrix1.get(1, i, z) + sMatrix1.get(2, i, z)) 
											* sMatrix2.get(4, z, j);
						break;
					case 6:
						for (int z = 0; z < extendedDim / 2; z++)
							P[num][i][j] += (sMatrix1.get(3, i, z) - sMatrix1.get(1, i, z)) 
											* (sMatrix2.get(1, z, j) + sMatrix2.get(2, z, j));
						break;
					case 7:
						for (int z = 0; z < extendedDim / 2; z++)
							P[num][i][j] += (sMatrix1.get(2, i, z) - sMatrix1.get(4, i, z)) 
											* (sMatrix2.get(3, z, j) + sMatrix2.get(4, z, j));
						break;
					}
				}
			}
		}
	}

	~pMatrix()
	{
		for (int i = 0; i < 7; i++)
		{
			P.push_back(new int* [extendedDim / 2]);
			for (int j = 0; j < extendedDim / 2; j++)
			{
				delete[] P[i][j];
			}
			delete[] P[i];
		}
	}

	int** computeResult()
	{
		//initializing result matrix
		int** resultMatrix = new int* [extendedDim];
		for (int i = 0; i < extendedDim; i++)
		{
			resultMatrix[i] = new int[extendedDim];
		}

		//computing result matrix
		int shift = extendedDim / 2;
		for (int i = 0; i < extendedDim / 2; i++)
		{
			for (int j = 0; j < extendedDim / 2; j++)
			{
				resultMatrix[i][j]                = P[0][i][j] + P[3][i][j] - P[4][i][j] + P[6][i][j];
				resultMatrix[i][j + shift]        = P[2][i][j] + P[4][i][j];
				resultMatrix[i + shift][j]        = P[1][i][j] + P[3][i][j];
				resultMatrix[i+ shift][j + shift] = P[0][i][j] - P[1][i][j] + P[2][i][j] + P[5][i][j];
			}
		}
		return resultMatrix;
	}
};


class croppedMatrix //class for cropping matrix
{
public:
	int** resultMatrix;
	int rowSize;
	int columnSize;

	croppedMatrix(int** extendedMatrix, int extendedDim)
	{
		int countedSize = 0;
		rowSize = extendedDim;
		columnSize = extendedDim;
		bool stopped = false;

		//row crop
		for (int i = extendedDim - 1; i >= 0; i--)
		{
			for (int j = extendedDim - 1; j >= 0; j--)
			{
				if (extendedMatrix[i][j] != 0)
				{
					stopped = true;
					break;
				}
			}
			if (stopped) break;
			rowSize--;
		}

		stopped = false;
		//column crop
		for (int i = extendedDim - 1; i >= 0; i--)
		{
			for (int j = extendedDim - 1; j >= 0; j--)
			{
				if (extendedMatrix[j][i] != 0)
				{
					stopped = true;
					break;
				}
			}
			if (stopped)
			{
				break;
			}
			columnSize--;
		}

		//cropped result allocating and filling
		resultMatrix = new int* [rowSize];
		for (int i = 0; i < rowSize; i++)
		{
			resultMatrix[i] = new int[columnSize];
			for (int j = 0; j < columnSize; j++)
				resultMatrix[i][j] = extendedMatrix[i][j];
		}
	}

	~croppedMatrix()
	{
		for (int i = 0; i < rowSize; i++)
		{
			delete[] resultMatrix[i];
		}
		delete[] resultMatrix;
	}
};


///////////////////////////////////////////////////////////////////////////////


	void main()
	{
		srand(time(NULL));
		int rowSize1, columnSize1, rowSize2, columnSize2, k;
		system("chcp 1251");
		cout << "Вас приветствует программа" << endl <<
			"быстрого перемножения матриц методом Штрассена\n\n";

		//////////////////////////////////
		/* Get matrixes sizes from user */
		//////////////////////////////////
		do
		{
			cout << "Введите размеры первой матрицы\n";
			cin >> rowSize1 >> columnSize1;
		} while (rowSize1 <= 0 || columnSize1 <= 0);
		do
		{
			cout << "Введите размеры второй матрицы\n";
			cin >> rowSize2 >> columnSize2;
		} while (rowSize2 <= 0 || columnSize2 <= 0);


		int** Matrix1 = new int* [rowSize1];
		for (int i = 0; i < rowSize1; i++)
			Matrix1[i] = new int[columnSize1];

		int** Matrix2 = new int* [rowSize2];
		for (int i = 0; i < rowSize2; i++)
			Matrix2[i] = new int[columnSize2];

		///////////////////////////////////////
		/* Manual or random matrixes filling */
		///////////////////////////////////////
		do
		{
			cout << "Выберите способ заполнения матриц\n" <<
				"1 - Вручную \n2 - Случайным образом\n";
			cin >> k;
		} while (k < 1 || k > 2);


		switch (k)
		{
		//manual input
		case 1:
			for (int i = 0; i < rowSize1; i++)
				for (int j = 0; j < columnSize1; j++)
					cin >> Matrix1[i][j];

			for (int i = 0; i < rowSize2; i++)
				for (int j = 0; j < columnSize2; j++)
					cin >> Matrix2[i][j];

			//print matrixes
			cout << "\nМатрица 1\n\n";
			for (int i = 0; i < rowSize1; i++)
			{
				for (int j = 0; j < columnSize1; j++)
					cout << Matrix1[i][j] << " ";
				cout << endl;
			}

			cout << "\nМатрица 2\n\n";
			for (int i = 0; i < rowSize2; i++)
			{
				for (int j = 0; j < columnSize2; j++)
					cout << Matrix2[i][j] << " ";
				cout << endl;
			}
			break;
		//random values
		case 2:
			for (int i = 0; i < rowSize1; i++)
				for (int j = 0; j < columnSize1; j++)
					Matrix1[i][j] = rand() % 10;

			for (int i = 0; i < rowSize2; i++)
				for (int j = 0; j < columnSize2; j++)
					Matrix2[i][j] = rand() % 10;

			//print matrixes
			cout << "\nМатрица 1\n\n";
			for (int i = 0; i < rowSize1; i++)
			{
				for (int j = 0; j < columnSize1; j++)
					cout << Matrix1[i][j] << " ";
				cout << endl;
			}

			cout << "\nМатрица 2\n\n";
			for (int i = 0; i < rowSize2; i++)
			{
				for (int j = 0; j < columnSize2; j++)
					cout << Matrix2[i][j] << " ";
				cout << endl;
			}
			break;
		}

		///////////////////////////////////
		/* Computation part:             */
		/* -slicing each matrix into 4   */
		/* -computing P-matrixes         */
		/* -computing Result-matrix      */
		/* -cropping Result-matrix       */
		///////////////////////////////////
		int extendedDim = 2;

		while (extendedDim < rowSize1 || extendedDim < rowSize2 || extendedDim < columnSize1 || extendedDim < columnSize2)
			extendedDim *= 2;

		matrixSlicer slicedMatrix1(Matrix1, rowSize1, columnSize1, extendedDim);  //slicing first
		matrixSlicer slicedMatrix2(Matrix2, rowSize2, columnSize2, extendedDim);  //and second matrixes into 4


		cout << "Приведенные матрицы\n";

		cout << "\nМатрица 1\n\n";
		for (int i = 0; i < extendedDim; i++)
		{
			for (int j = 0; j < extendedDim; j++)
				cout << slicedMatrix1.get(i, j) << " ";
			cout << endl;
		}

		cout << "\nМатрица 2\n\n";
		for (int i = 0; i < extendedDim; i++)
		{
			for (int j = 0; j < extendedDim; j++)
				cout << slicedMatrix2.get(i, j) << " ";
			cout << endl;
		}

		// P-matrixes -> extended Result-matrix -> cropped Result-matrix
		pMatrix pMatrixes(slicedMatrix1, slicedMatrix2, extendedDim);
		int** extendedResultMatrix = pMatrixes.computeResult();
		croppedMatrix croppedResultMatrix(extendedResultMatrix, extendedDim);


		//print Result-matrix
		cout << "\nРезультирующая матрица\n\n";
		for (int i = 0; i < croppedResultMatrix.rowSize; i++)
		{
			for (int j = 0; j < croppedResultMatrix.columnSize; j++)
				cout << croppedResultMatrix.resultMatrix[i][j] << " ";
			cout << endl;
		}


		system("pause");

		////////////////////
		/* Memory freeing */
		////////////////////
		for (int i = 0; i < rowSize1; i++)
			delete[] Matrix1[i];
		for (int i = 0; i < rowSize2; i++)
			delete[] Matrix2[i];
		delete[] Matrix1, Matrix2;
	}