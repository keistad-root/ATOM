#ifndef __TMATRIX2D__
#define __TMATRIX2D__

#include "Headers.h"

template <typename numT>
class TMatrix2D;

template<typename numT>
std::ostream& operator<< (std::ostream& os, const TMatrix2D<numT>& matrix);

template <typename numT>
class TMatrix2D {
private:
    std::vector<std::vector<numT>> mMatrix;
    int mNRow, mNColumn;
public:
    TMatrix2D();
    TMatrix2D(int nRow, int nColumn);
    TMatrix2D(const std::vector<std::vector<numT>>& matrix);

    void setMatrix(const std::vector<std::vector<numT>>& matrix);
    void setElement(const int iRow, const int iColumn, const numT element);
    const numT getElement(const int iRow, const int iColumn) const;
    const int getNRow() const;
    const int getNColumn() const;
    const std::pair<int, int> getDimension() const;

    template<typename numT2>
    bool isSameDimension(const TMatrix2D<numT2>& matrix);
    template<typename numT2>
    bool isSame(const TMatrix2D<numT2>& matrix);
    template<typename numT2>
    bool hasXSymmetry(const TMatrix2D<numT2>& matrix);
    template<typename numT2>
    bool hasYSymmetry(const TMatrix2D<numT2>& matrix);
    template<typename numT2>
    bool hasXYSymmetry(const TMatrix2D<numT2>& matrix);
    template<typename numT2>
    bool hasRSymmetry(const TMatrix2D<numT2>& matrix);
    template<typename numT2>
    bool hasRXSymmetry(const TMatrix2D<numT2>& matrix);
    template<typename numT2>
    bool hasRYSymmetry(const TMatrix2D<numT2>& matrix);
    template<typename numT2>
    bool hasRXYSymmetry(const TMatrix2D<numT2>& matrix);
    template<typename numT2>
    bool hasHomeomorphism(const TMatrix2D<numT2>& matrix);

    friend std::ostream& operator<<<>(std::ostream& os, const TMatrix2D<numT>& matrix);
    template<typename numT2>
    TMatrix2D& operator+=(const TMatrix2D<numT2>& matrix);
    template<typename numT2>
    TMatrix2D& operator+(const TMatrix2D<numT2>& matrix);
    template<typename numT2>
    TMatrix2D& operator-=(const TMatrix2D<numT2>& matrix);
    template<typename numT2>
    TMatrix2D& operator-(const TMatrix2D<numT2>& matrix);
    template<typename numT2>
    TMatrix2D& operator*=(numT2 scalar);
    template<typename numT2>
    TMatrix2D& operator*(numT2 scalar);
    template<typename numT2>
    TMatrix2D& operator*=(const TMatrix2D<numT2>& matrix);
    template<typename numT2>
    TMatrix2D& operator*(const TMatrix2D<numT2>& matrix);
};


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
template<typename numT>
TMatrix2D<numT>::TMatrix2D() {}

template<typename numT>
TMatrix2D<numT>::TMatrix2D(int nRow, int nColumn) : mNRow(nRow), mNColumn(nColumn) {
    for (int iRow = 0; iRow < nRow; iRow++) {
        std::vector<numT> temp;
        for (int iColumn = 0; iColumn < nColumn; iColumn++) {
            temp.push_back(0);
        }
        mMatrix.push_back(temp);
    }
}

template<typename numT>
TMatrix2D<numT>::TMatrix2D(const std::vector<std::vector<numT>>& matrix) {
    mNRow = matrix.size();
    mNColumn = matrix[0].size();
    mMatrix.assign(matrix.begin(), matrix.end());
}

template<typename numT>
void TMatrix2D<numT>::setMatrix(const std::vector<std::vector<numT>>& matrix) {
    mNRow = matrix.size();
    mNColumn = matrix[0].size();
    mMatrix.clear();
    mMatrix.assign(matrix.begin(), matrix.end());
}

template<typename numT>
void TMatrix2D<numT>::setElement(const int iRow, const int iColumn, const numT element) {
    mMatrix[iRow][iColumn] = element;
}

template<typename numT>
const numT TMatrix2D<numT>::getElement(const int iRow, const int iColumn) const {
    try {
        if (iRow < 0 || iRow > mNRow) throw iRow;
        if (iColumn < 0 || iColumn > mNColumn) throw iColumn;
        return mMatrix[iRow][iColumn];
    } catch (int iRow) {
        std::cerr << "The row index shoud be positive and smaller than " << mNRow << ". Current: " << iRow << std::endl;
        return mMatrix[0][0];
    }
}

template<typename numT>
const int TMatrix2D<numT>::getNRow() const {
    return mNRow;
}

template<typename numT>
const int TMatrix2D<numT>::getNColumn() const {
    return mNColumn;
}

template<typename numT>
const std::pair<int, int> TMatrix2D<numT>::getDimension() const {
    return {mNRow, mNColumn};
}

template<typename numT> 
template<typename numT2>
bool TMatrix2D<numT>::isSameDimension(const TMatrix2D<numT2>& matrix) {
    if (mNRow == matrix.getNRow() && mNColumn == matrix.getNColumn()) {
        return true;
    } else {
        return false;
    }
}

template<typename numT>
template<typename numT2>
bool TMatrix2D<numT>::isSame(const TMatrix2D<numT2>& matrix) {
    for (int iColumn = 0; iColumn < mNColumn; iColumn++) {
        for (int iRow = 0; iRow < mNRow; iRow++) {
            if (mMatrix[iRow][iColumn] != matrix.getElement(iRow, iColumn)) {
                return false;
            }
        }
    }
    return true;
}

template<typename numT>
template<typename numT2>
bool TMatrix2D<numT>::hasYSymmetry(const TMatrix2D<numT2>& matrix) {
    for (int iColumn = 0; iColumn < mNColumn; iColumn++) {
        for (int iRow = 0; iRow < mNRow; iRow++) {
            if (mMatrix[iRow][iColumn] != matrix.getElement(mNRow - iRow - 1, iColumn)) {
                return false;
            }
        }
    }
    return true;
}

template<typename numT>
template<typename numT2>
bool TMatrix2D<numT>::hasXSymmetry(const TMatrix2D<numT2>& matrix) {
    for (int iRow = 0; iRow < mNRow; iRow++) {
        for (int iColumn = 0; iColumn < mNColumn; iColumn++) {
            if (mMatrix[iRow][iColumn] != matrix.getElement(iRow, mNColumn - iColumn - 1)) {
                return false;
            }
        }
    }
    return true;
}

template<typename numT>
template<typename numT2>
bool TMatrix2D<numT>::hasXYSymmetry(const TMatrix2D<numT2>& matrix) {
    for (int iRow = 0; iRow < mNRow; iRow++) {
        for (int iColumn = 0; iColumn < mNColumn; iColumn++) {
            if (mMatrix[iRow][iColumn] != matrix.getElement(mNRow - iRow - 1, mNColumn - iColumn - 1)) {
                return false;
            }
        }
    }
    return true;
}

template<typename numT>
template<typename numT2>
bool TMatrix2D<numT>::hasRSymmetry(const TMatrix2D<numT2>& matrix) {
    for (int iRow = 0; iRow < mNRow; iRow++) {
        for (int iColumn = 0; iColumn < mNColumn; iColumn++) {
            if (mMatrix[iRow][iColumn] != matrix.getElement(mNColumn - iColumn - 1, iRow)) {
                return false;
            }
        }
    }
    return true;
}

template<typename numT>
template<typename numT2>
bool TMatrix2D<numT>::hasRXSymmetry(const TMatrix2D<numT2>& matrix) {
    for (int iRow = 0; iRow < mNRow; iRow++) {
        for (int iColumn = 0; iColumn < mNColumn; iColumn++) {
            if (mMatrix[iRow][iColumn] != matrix.getElement(iColumn, iRow)) {
                return false;
            }
        }
    }
    return true;
}

template<typename numT>
template<typename numT2>
bool TMatrix2D<numT>::hasRYSymmetry(const TMatrix2D<numT2>& matrix) {
    for (int iRow = 0; iRow < mNRow; iRow++) {
        for (int iColumn = 0; iColumn < mNColumn; iColumn++) {
            if (mMatrix[iRow][iColumn] != matrix.getElement(mNColumn - iColumn - 1, mNRow - iRow - 1)) {
                return false;
            }
        }
    }
    return true;
}

template<typename numT>
template<typename numT2>
bool TMatrix2D<numT>::hasRXYSymmetry(const TMatrix2D<numT2>& matrix) {
    for (int iRow = 0; iRow < mNRow; iRow++) {
        for (int iColumn = 0; iColumn < mNColumn; iColumn++) {
            if (mMatrix[iRow][iColumn] != matrix.getElement(iColumn, mNRow - iRow - 1)) {
                return false;
            }
        }
    }
    return true;
}

template<typename numT>
template<typename numT2>
bool TMatrix2D<numT>::hasHomeomorphism(const TMatrix2D<numT2>& matrix) {
    if (mNRow == mNColumn && mNRow == matrix.getNRow() && mNColumn == matrix.getNColumn()) {
        if (isSame(matrix)) {
            return true;
        } else if (hasXSymmetry(matrix)) {
            return true;
        } else if (hasYSymmetry(matrix)) {
            return true;
        } else if (hasXYSymmetry(matrix)) {
            return true;
        } else if (hasRSymmetry(matrix)) {
            return true;
        } else if (hasRXSymmetry(matrix)) {
            return true;
        } else if (hasRYSymmetry(matrix)) {
            return true;
        } else if (hasRXYSymmetry(matrix)) {
            return true;
        } else {
            return false;
        }
    } else if (mNRow == matrix.getNRow() && mNColumn == matrix.getNColumn()) {
        if (isSame(matrix)) {
            return true;
        } else if (hasXSymmetry(matrix)) {
            return true;
        } else if (hasYSymmetry(matrix)) {
            return true;
        } else if (hasXYSymmetry(matrix)) {
            return true;
        } else {
            return false;
        }
    } else if (mNRow == matrix.getNColumn() && mNColumn == matrix.getNRow()) {
        if (hasRSymmetry(matrix)) {
            return true;
        } else if (hasRXSymmetry(matrix)) {
            return true;
        } else if (hasRYSymmetry(matrix)) {
            return true;
        } else if (hasRXYSymmetry(matrix)) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

template<typename numT>
std::ostream& operator<<(std::ostream& os, const TMatrix2D<numT>& matrix) {
    for (const std::vector<numT>& aRow : matrix.mMatrix) {
        for (const numT& element : aRow) {
            os << element << " ";
        }
        os << std::endl;
    }
    os << "    Dimension: " << matrix.mNRow << "X" << matrix.mNColumn;
    return os;
}

template<typename numT> 
template<typename numT2>
TMatrix2D<numT>& TMatrix2D<numT>::operator+=(const TMatrix2D<numT2>& matrix) {
    try {
        if(!isSameDimension(matrix)) throw matrix;
        for (int iRow = 0; iRow < mNRow; iRow++) {
            for (int iColumn = 0; iColumn < mNColumn; iColumn++) {
                mMatrix[iRow][iColumn] += matrix.getElement(iRow, iColumn);
            }
        }
    } catch (TMatrix2D<numT> matrix) {
        std::cerr << "The dimension should be same for plus operator. (" << mNRow << "X" << mNColumn << " != " << matrix.mNRow << "X" << matrix.mNColumn << ")" << std::endl;
    }
    return *this;
}

template<typename numT> 
template<typename numT2>
TMatrix2D<numT>& TMatrix2D<numT>::operator+(const TMatrix2D<numT2>& matrix) {
    try {
        if(!isSameDimension(matrix)) throw matrix;
        for (int iRow = 0; iRow < mNRow; iRow++) {
            for (int iColumn = 0; iColumn < mNColumn; iColumn++) {
                mMatrix[iRow][iColumn] += matrix.getElement(iRow, iColumn);
            }
        }
    } catch (TMatrix2D<numT> matrix) {
        std::cerr << "The dimension should be same for plus operator. (" << mNRow << "X" << mNColumn << " != " << matrix.mNRow << "X" << matrix.mNColumn << ")" << std::endl;
    }
    return *this;
}

template<typename numT>
template<typename numT2>
TMatrix2D<numT>& TMatrix2D<numT>::operator-=(const TMatrix2D<numT2>& matrix) {
    try {
        if(!isSameDimension(matrix)) throw matrix;
        for (int iRow = 0; iRow < mNRow; iRow++) {
            for (int iColumn = 0; iColumn < mNColumn; iColumn++) {
                mMatrix[iRow][iColumn] -= matrix.getElement(iRow, iColumn);
            }
        }
    } catch (TMatrix2D<numT> matrix) {
        std::cerr << "The dimension should be same for minus operator. (" << mNRow << "X" << mNColumn << " != " << matrix.mNRow << "X" << matrix.mNColumn << ")" << std::endl;
    }
    return *this;
}

template<typename numT>
template<typename numT2>
TMatrix2D<numT>& TMatrix2D<numT>::operator-(const TMatrix2D<numT2>& matrix) {
    try {
        if(!isSameDimension(matrix)) throw matrix;
        for (int iRow = 0; iRow < mNRow; iRow++) {
            for (int iColumn = 0; iColumn < mNColumn; iColumn++) {
                mMatrix[iRow][iColumn] -= matrix.getElement(iRow, iColumn);
            }
        }
    } catch (TMatrix2D<numT> matrix) {
        std::cerr << "The dimension should be same for minus operator. (" << mNRow << "X" << mNColumn << " != " << matrix.mNRow << "X" << matrix.mNColumn << ")" << std::endl;
    }
    return *this;
}

template<typename numT>
template<typename numT2>
TMatrix2D<numT>& TMatrix2D<numT>::operator*=(const numT2 scalar) {
    for (int iRow = 0; iRow < mNRow; iRow++) {
        for (int iColumn = 0; iColumn < mNColumn; iColumn++) {
            mMatrix[iRow][iColumn] *= scalar;
        }
    }
    return *this;
}

template<typename numT>
template<typename numT2>
TMatrix2D<numT>& TMatrix2D<numT>::operator*(const numT2 scalar) {
    for (int iRow = 0; iRow < mNRow; iRow++) {
        for (int iColumn = 0; iColumn < mNColumn; iColumn++) {
            mMatrix[iRow][iColumn] *= scalar;
        }
    }
    return *this;
}

template<typename numT>
template<typename numT2>
TMatrix2D<numT>& TMatrix2D<numT>::operator*=(const TMatrix2D<numT2>& matrix) {
    try {
        if(mNRow != matrix.getNColumn()) throw matrix;

        TMatrix2D<numT> newMatrix(mNRow, matrix.getNColumn());
        for (int iRow = 0; iRow < mNRow; iRow++) {
            for (int iColumn = 0; iColumn < matrix.getNColumn(); iColumn++) {
                for (int iOperand = 0; iOperand < mNColumn; iOperand++) {
                    newMatrix.setElement(iRow, iColumn, newMatrix.getElement(iRow, iColumn) + mMatrix[iRow][iOperand] * matrix.getElement(iOperand, iColumn));
                }
            }
        }
        *this = newMatrix;
    } catch (TMatrix2D<numT> matrix) {
        std::cerr << "The row dimension should be same with column dimension of operand for multiple operator. (" << mNRow << "X" << mNColumn << " != " << matrix.mNRow << "X" << matrix.mNColumn << ")" << std::endl;
    }
    return *this;
}

template<typename numT>
template<typename numT2>
TMatrix2D<numT>& TMatrix2D<numT>::operator*(const TMatrix2D<numT2>& matrix) {
    try {
        if(mNRow != matrix.getNColumn()) throw matrix;

        TMatrix2D<numT> newMatrix(mNRow, matrix.getNColumn());
        for (int iRow = 0; iRow < mNRow; iRow++) {
            for (int iColumn = 0; iColumn < matrix.getNColumn(); iColumn++) {
                for (int iOperand = 0; iOperand < mNColumn; iOperand++) {
                    newMatrix.setElement(iRow, iColumn, newMatrix.getElement(iRow, iColumn) + mMatrix[iRow][iOperand] * matrix.getElement(iOperand, iColumn));
                }
            }
        }
        *this = newMatrix;
    } catch (TMatrix2D<numT> matrix) {
        std::cerr << "The row dimension should be same with column dimension of operand for multiple operator. (" << mNRow << "X" << mNColumn << " != " << matrix.mNRow << "X" << matrix.mNColumn << ")" << std::endl;
    }
    return *this;
}




#endif