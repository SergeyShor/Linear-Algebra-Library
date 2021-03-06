#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace LinAlg
{
    template <typename T>
    bool areEqual(T value1, T value2);

    template <typename T>
    class Matrix
    {
    public:
        Matrix();
        Matrix(std::size_t rows, std::size_t cols);
        Matrix(std::size_t rows, std::size_t cols, T value);
        Matrix(std::size_t rows, std::size_t cols, const std::vector<T>& v);
        Matrix(std::initializer_list< std::initializer_list<T> > il);
        Matrix(const Matrix<T>& other) = default;
        Matrix(Matrix<T>&& other) noexcept;
        ~Matrix() = default;

        T& operator()(std::size_t row, std::size_t col);
        const T& operator()(std::size_t row, std::size_t col) const;

        Matrix<T>& operator= (const Matrix<T>& other) = default;
        Matrix<T>& operator= (Matrix<T>&& other) noexcept;

        Matrix<T> operator* (T value);
        Matrix<T> operator/ (T value);

        Matrix<T>& operator*= (T value);
        Matrix<T>& operator/= (T value);

        Matrix<T> operator- ();

        Matrix<T> operator+ (const Matrix<T>& other);
        Matrix<T> operator- (const Matrix<T>& other);
        Matrix<T> operator* (const Matrix<T>& other);
        Matrix<T> operator/ (const Matrix<T>& other);

        Matrix<T>& operator+= (const Matrix<T>& other);
        Matrix<T>& operator-= (const Matrix<T>& other);
        Matrix<T>& operator*= (const Matrix<T>& other);
        Matrix<T>& operator/= (const Matrix<T>& other);

        std::size_t rows() const { return _rows; }
        std::size_t cols() const { return _cols; }
        std::size_t vector_size() const { return _matrix.size(); }
        std::size_t max_rows() { return _matrix.max_size(); };
        std::size_t max_cols() { return max_rows() / _rows; };

        bool square() const { return _rows == _cols; }
        bool zero() const { return *this == Matrix<T>(_rows, _cols); };

        T& at(std::size_t row, std::size_t col);
        const T& at(std::size_t row, std::size_t col) const;

        void set_identity();
        void set_zero();
        void set_diag(const std::vector<T>& v);
        void set_diag(std::initializer_list<T> il);
        void set_row(std::size_t row, T value);
        void set_row(std::size_t row, const std::vector<T>& v);
        void set_row(std::size_t row, std::initializer_list<T> il);
        void set_col(std::size_t col, T value);
        void set_col(std::size_t col, const std::vector<T>& v);
        void set_col(std::size_t col, std::initializer_list<T> il);

        std::vector<T> get_row(std::size_t row) const;
        std::vector<T> get_col(std::size_t col) const;

        void transpose();
        void pow(int power);
        void swap_row(std::size_t lhsRow, std::size_t rhsRow);
        void swap_col(std::size_t lhsCol, std::size_t rhsCol);
        void mult_row(std::size_t row, T value);
        void mult_col(std::size_t col, T value);
        void add_row(std::size_t lhsRow, std::size_t rhsRow, T value);
        void add_col(std::size_t lhsCol, std::size_t rhsCol, T value);
        T cofactor(std::size_t row, std::size_t col);
        T determinant();
        Matrix<T> minor(std::size_t row, std::size_t col);
        Matrix<T> adjoint();
        Matrix<T> inverse();

        template <typename U>
        friend bool operator== (const Matrix<U>& lhs, const Matrix<U>& rhs);

        template <typename U>
        friend Matrix<U> operator* (U value, const Matrix<U>& other);

    private:
        std::size_t _rows;
        std::size_t _cols;
        std::vector<T> _matrix;

        std::size_t check_rows_arg(std::size_t rows);
        std::size_t check_cols_arg(std::size_t cols);
        std::size_t check_template_arg(std::size_t size);
    };

    template <typename T>
    inline bool areEqual(T value1, T value2)
    {
        if (std::numeric_limits<T>::is_iec559) {
            return std::fabs(value1 - value2) <= (std::max(std::fabs(value1), std::fabs(value2)) * std::numeric_limits<T>::epsilon());
        } else {
            return value1 == value2;
        }
    }

    template <typename U>
    inline bool operator== (const Matrix<U>& lhs, const Matrix<U>& rhs)
    {
        if (&lhs == &rhs) { return true; };
        if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()) { return false; }

        for (auto i = 0; i < lhs.rows(); ++i) {
            for (auto j = 0; j < lhs.cols(); ++j) {
                if (!areEqual(lhs.at(i, j), rhs.at(i, j))) { return false; }
            }
        }
        return true;
    }

    template <typename U>
    inline Matrix<U> operator* (U value, const Matrix<U>& other)
    {
        Matrix<U> tempMatrix(other);
        return tempMatrix * value;
    }

}

template <typename T>
inline LinAlg::Matrix<T>::Matrix()
    :  _rows(0), _cols(0), _matrix(check_template_arg(0))
{
}

template <typename T>
inline LinAlg::Matrix<T>::Matrix(std::size_t rows, std::size_t cols)
    : _rows(check_rows_arg(rows)), _cols(check_cols_arg(cols)), _matrix(check_template_arg(rows * cols))
{
}

template <typename T>
inline LinAlg::Matrix<T>::Matrix(std::size_t rows, std::size_t cols, T value)
    : _rows(check_rows_arg(rows)), _cols(check_cols_arg(cols)), _matrix(check_template_arg(rows * cols))
{
    _matrix.assign(vector_size(), value);
}

template <typename T>
inline LinAlg::Matrix<T>::Matrix(std::size_t rows, std::size_t cols, const std::vector<T>& v)
    : _rows(check_rows_arg(rows)), _cols(check_cols_arg(cols)), _matrix(check_template_arg(rows * cols))
{
    if (v.size() != _matrix.size()) { throw std::invalid_argument("invalid vector argument size"); }

    _matrix.assign(v.begin(), v.end());
}

template <typename T>
inline LinAlg::Matrix<T>::Matrix(std::initializer_list< std::initializer_list<T> > il)
    : _rows(il.size()), _cols(il.begin()->size())
{
    if (!std::is_arithmetic<T>::value) { throw std::invalid_argument("invalid Matrix template argument"); }

    for (auto i = il.begin(); i != il.end(); ++i) {
        if (_cols != i->size()) { throw std::invalid_argument("invalid nested initializer list size"); }

        _matrix.insert(_matrix.end(), i->begin(), i->end());
    }
}

template <typename T>
inline LinAlg::Matrix<T>::Matrix(Matrix&& other) noexcept
    : _rows(other._rows), _cols(other._cols), _matrix(std::move(other._matrix))
{
    other._rows = 0;
    other._cols = 0;
}

template <typename T>
inline T& LinAlg::Matrix<T>::operator() (std::size_t row, std::size_t col)
{
    return _matrix[row * _cols + col];
}

template <typename T>
inline const T& LinAlg::Matrix<T>::operator() (std::size_t row, std::size_t col) const
{
    return _matrix[row * _cols + col];
}

template <typename T>
inline LinAlg::Matrix<T>& LinAlg::Matrix<T>::operator= (Matrix<T>&& other) noexcept
{
    if (this != &other) {
        _rows = other._rows;
        _cols = other._cols;
        _matrix = std::move(other._matrix);

        other._rows = 0;
        other._cols = 0;
    }
    return *this;
}

template <typename T>
inline LinAlg::Matrix<T> LinAlg::Matrix<T>::operator* (T value)
{
    LinAlg::Matrix<T> temp(*this);
    for (T& element : temp._matrix) { element *= value; }
    return temp;
}

template <typename T>
inline LinAlg::Matrix<T> LinAlg::Matrix<T>::operator/ (T value)
{
    if (value == T()) { throw std::invalid_argument("Matrix division by zero"); }
    LinAlg::Matrix<T> temp(*this);
    for (T& element : temp._matrix) { element /= value; }
    return temp;
}

template <typename T>
inline LinAlg::Matrix<T>& LinAlg::Matrix<T>::operator*= (T value)
{
    for (T& element : _matrix) { element *= value; }
    return *this;
}

template <typename T>
inline LinAlg::Matrix<T>& LinAlg::Matrix<T>::operator/= (T value)
{
    if (value == T()) { throw std::invalid_argument("Matrix division by zero"); }
    for (T& element : _matrix) { element /= value; }
    return *this;
}

template <typename T>
inline LinAlg::Matrix<T> LinAlg::Matrix<T>::operator- ()
{
    return (*this) * -1;
}

template <typename T>
inline LinAlg::Matrix<T> LinAlg::Matrix<T>::operator+ (const Matrix<T>& other)
{
    if (_rows != other._rows || _cols != other._cols) { throw std::invalid_argument("invalid Matrix argument size"); }

    LinAlg::Matrix<T> resultMatrix(*this);
    for (auto i = 0; i < resultMatrix.rows(); ++i) {
        for (auto j = 0; j < resultMatrix.cols(); ++j) {
            resultMatrix.at(i, j) += other.at(i, j);
        }
    }
    return resultMatrix;
}

template <typename T>
inline LinAlg::Matrix<T> LinAlg::Matrix<T>::operator- (const Matrix<T>& other)
{
    LinAlg::Matrix<T> tempMatrix(other);
    LinAlg::Matrix<T> resultMatrix(*this + -tempMatrix);
    return resultMatrix;
}

template <typename T>
inline LinAlg::Matrix<T> LinAlg::Matrix<T>::operator* (const Matrix<T>& other)
{
    if (_cols != other._rows) { throw std::invalid_argument("invalid Matrix argument size"); }
    LinAlg::Matrix<T> resultMatrix(_rows, other._cols);
    for (auto i = 0; i < resultMatrix.rows(); ++i) {
        for (auto j = 0; j < resultMatrix.cols(); ++j) {
            for (auto k = 0; k < _cols; ++k) {
                resultMatrix.at(i, j) += at(i, k) * other.at(k, j);
            }
        }
    }
    return resultMatrix;
}

template <typename T>
inline LinAlg::Matrix<T> LinAlg::Matrix<T>::operator/ (const Matrix<T>& other)
{
    LinAlg::Matrix<T> tempMatrix(other);
    LinAlg::Matrix<T> resultMatrix = *this * tempMatrix.inverse();
    return resultMatrix;
}

template <typename T>
inline LinAlg::Matrix<T>& LinAlg::Matrix<T>::operator+= (const Matrix<T>& other)
{
    *this = *this + other;
    return *this;
}

template <typename T>
inline LinAlg::Matrix<T>& LinAlg::Matrix<T>::operator-= (const Matrix<T>& other)
{
    *this = *this - other;
    return *this;
}

template <typename T>
inline LinAlg::Matrix<T>& LinAlg::Matrix<T>::operator*= (const Matrix<T>& other)
{
    *this = *this * other;
    return *this;
}

template <typename T>
inline LinAlg::Matrix<T>& LinAlg::Matrix<T>::operator/= (const Matrix<T>& other)
{
    *this = *this / other;
    return *this;
}

template <typename T>
inline T& LinAlg::Matrix<T>::at(std::size_t row, std::size_t col)
{
    if (row < 0 || col < 0 || _rows < row || _cols < col) { throw std::out_of_range("invalid Matrix subscripts"); }
    return _matrix[row * _cols + col];
}

template <typename T>
inline const T& LinAlg::Matrix<T>::at(std::size_t row, std::size_t col) const
{
    if (row < 0 || col < 0 || _rows < row || _cols < col) { throw std::out_of_range("invalid Matrix subscripts"); }
    return _matrix[row * _cols + col];
}

template <typename T>
inline void LinAlg::Matrix<T>::set_identity()
{
    if (!square()) { throw std::invalid_argument("square Matrix required"); }

    std::vector<T> identityVector(vector_size());
    for (auto i = 0; i < identityVector.size(); i += _rows + 1) {
        identityVector[i] = 1;
    }
    *this = LinAlg::Matrix<T>(_rows, _cols, identityVector);
}

template <typename T>
inline void LinAlg::Matrix<T>::set_zero()
{
    *this = LinAlg::Matrix<T>(_rows, _cols);
}

template <typename T>
inline void LinAlg::Matrix<T>::set_diag(const std::vector<T>& v)
{
    if (!square()) { throw std::invalid_argument("square Matrix required"); }
    if (v.size() != _rows) { throw std::invalid_argument("invalid vector argument size"); }

    set_zero();
    for (auto i = 0, j = 0; i < _matrix.size(); i += _rows + 1, ++j) {
        _matrix[i] = v[j];
    }
}

template <typename T>
inline void LinAlg::Matrix<T>::set_diag(std::initializer_list<T> il)
{
    if (!square()) { throw std::invalid_argument("square Matrix required"); }
    if (il.size() != _rows) { throw std::invalid_argument("invalid initializer list argument size"); }

    set_zero();
    auto element = il.begin();
    for (auto i = 0; i < _matrix.size(); i += _rows + 1, ++element) {
        _matrix[i] = *element;
    }
}

template <typename T>
inline void LinAlg::Matrix<T>::set_row(std::size_t row, T value)
{
    if (row < 0 || row >= _rows) { throw std::out_of_range("invalid Matrix row subscript"); }

    for (auto i = row * _cols; i < (row + 1) * _cols; ++i) {
        _matrix[i] = value;
    }
}

template <typename T>
inline void LinAlg::Matrix<T>::set_row(std::size_t row, const std::vector<T>& v)
{
    if (v.size() != _cols) { throw std::invalid_argument("invalid vector argument size"); }
    if (row < 0 || row >= _rows) { throw std::out_of_range("invalid Matrix row subscript"); }

    for (std::size_t i = row * _cols, j = 0; j < _cols; ++i, ++j) {
        _matrix[i] = v[j];
    }
}

template <typename T>
inline void LinAlg::Matrix<T>::set_row(std::size_t row, std::initializer_list<T> il)
{
    if (il.size() != _cols) { throw std::invalid_argument("invalid initializer list argument size"); }
    if (row < 0 || row >= _rows) { throw std::out_of_range("invalid Matrix row subscript"); }

    auto element = il.begin();
    for (auto i = row * _cols; i < (row + 1) * _cols; ++i, ++element) {
        _matrix[i] = *element;
    }
}

template <typename T>
inline void LinAlg::Matrix<T>::set_col(std::size_t col, T value)
{
    if (col < 0 || col >= _cols) { throw std::out_of_range("invalid Matrix column subscript"); }

    for (auto i = col; i < (_rows - 1) * _cols + col + 1; i += _cols) {
        _matrix[i] = value;
    }
}

template <typename T>
inline void LinAlg::Matrix<T>::set_col(std::size_t col, const std::vector<T>& v)
{
    if (v.size() != _rows) { throw std::invalid_argument("invalid vector argument size"); }
    if (col < 0 || col >= _cols) { throw std::out_of_range("invalid Matrix column subscript"); }

    for (std::size_t i = col, j = 0; j < _rows; i += _cols, ++j) {
        _matrix[i] = v[j];
    }
}

template <typename T>
inline void LinAlg::Matrix<T>::set_col(std::size_t col, std::initializer_list<T> il)
{
    if (il.size() != _rows) { throw std::invalid_argument("invalid initializer list argument size"); }
    if (col < 0 || col >= _cols) { throw std::out_of_range("invalid Matrix column subscript"); }
    
    auto element = il.begin();
    for (auto i = col; i < (_rows - 1) * _cols + col + 1; i += _cols, ++element) {
        _matrix[i] = *element;
    }
}

template <typename T>
inline std::vector<T> LinAlg::Matrix<T>::get_row(std::size_t row) const
{
    if (row < 0 || row >= _rows) { throw std::out_of_range("invalid Matrix row subscript"); }

    std::vector<T> rowVector(_cols);
    for (std::size_t i = row * _cols, j = 0; j < _cols; ++i, ++j) {
        rowVector[j] = _matrix[i];
    }
    return rowVector;
}

template <typename T>
inline std::vector<T> LinAlg::Matrix<T>::get_col(std::size_t col) const
{
    if (col < 0 || col >= _cols) { throw std::out_of_range("invalid Matrix column subscript"); }

    std::vector<T> colVector(_rows);
    for (std::size_t i = col, j = 0; j < _rows; i += _cols, ++j) {
        colVector[j] = _matrix[i];
    }
    return colVector;
}

template <typename T>
inline void LinAlg::Matrix<T>::transpose()
{
    std::vector<T> tempVector(vector_size());
    std::swap(_rows, _cols);
    int counter = 0;
    for (auto i = 0; i < _rows; ++i) {
        for (auto j = 0; j < _cols; ++j) {
            tempVector[counter++] = _matrix[i + j * _rows];
        }
    }
    _matrix.assign(tempVector.begin(), tempVector.end());
}

template <typename T>
inline void LinAlg::Matrix<T>::pow(int power)
{
    if (!square()) { throw std::invalid_argument("square Matrix required"); }

    if (power == 0) {
        set_identity();
    } else if(power > 0) {
        LinAlg::Matrix<T> resultMatrix(*this);
        for (int i = 1; i < power; ++i) {
            resultMatrix *= *this;
        }
        *this = resultMatrix;
    } else if(power < 0) {
        LinAlg::Matrix<T> resultMatrix(inverse());
        for (int i = 1; i < std::abs(power); ++i) {
            resultMatrix *= inverse();
        }
        *this = resultMatrix;
    }
}

template <typename T>
inline void LinAlg::Matrix<T>::swap_row(std::size_t lhsRow, std::size_t rhsRow)
{
    if ( (lhsRow < 0 || lhsRow >= _rows) || (rhsRow < 0 || rhsRow >= _rows) ) { throw std::out_of_range("invalid Matrix row subscript"); }

    if (lhsRow != rhsRow) {
        for (auto i = lhsRow * _cols, j = rhsRow * _cols; i < (lhsRow + 1) * _cols; ++i, ++j) {
            std::swap(_matrix[i], _matrix[j]);
        }
    }
}

template <typename T>
inline void LinAlg::Matrix<T>::swap_col(std::size_t lhsCol, std::size_t rhsCol)
{
    if ( (lhsCol < 0 || lhsCol >= _cols) || (rhsCol < 0 || rhsCol >= _cols) ) { throw std::out_of_range("invalid Matrix column subscript"); }

    if (lhsCol != rhsCol) {
        for (auto i = lhsCol, j = rhsCol; i < (_rows - 1) * _cols + lhsCol + 1; i += _cols, j += _cols) {
            std::swap(_matrix[i], _matrix[j]);
        }
    }
}

template <typename T>
inline void LinAlg::Matrix<T>::mult_row(std::size_t row, T value)
{
    if (row < 0 || row >= _rows) { throw std::out_of_range("invalid Matrix row subscript"); }

    for (auto i = row * _cols; i < (row + 1) * _cols; ++i) {
        _matrix[i] *= value;
    }
}

template <typename T>
inline void LinAlg::Matrix<T>::mult_col(std::size_t col, T value)
{
    if (col < 0 || col >= _cols) { throw std::out_of_range("invalid Matrix column subscript"); }

    for (auto i = col; i < (_rows - 1) * _cols + col + 1; i += _cols) {
        _matrix[i] *= value;
    }
}

template <typename T>
inline void LinAlg::Matrix<T>::add_row(std::size_t lhsRow, std::size_t rhsRow, T value)
{
    if ( (lhsRow < 0 || lhsRow >= _rows) || (rhsRow < 0 || rhsRow >= _rows) ) { throw std::out_of_range("invalid Matrix row subscript"); }

    if (value != 0) {
        if (lhsRow == rhsRow) {
            mult_row(lhsRow, value + 1);
        } else {
            for (auto i = lhsRow * _cols, j = rhsRow * _cols; i < (lhsRow + 1) * _cols; ++i, ++j) {
                _matrix[i] += value * _matrix[j];
            }
        }
    }
}

template <typename T>
inline void LinAlg::Matrix<T>::add_col(std::size_t lhsCol, std::size_t rhsCol, T value)
{
    if ( (lhsCol < 0 || lhsCol >= _cols) || (rhsCol < 0 || rhsCol >= _cols) ) { throw std::out_of_range("invalid Matrix column subscript"); }

    if (value != 0) {
        if (lhsCol == rhsCol) {
            mult_col(lhsCol, value + 1);
        } else {
            for (auto i = lhsCol, j = rhsCol; i < (_rows - 1) * _cols + lhsCol + 1; i += _cols, j += _cols) {
                _matrix[i] += value * _matrix[j];
            }
        }
    }
}

template <typename T>
inline T LinAlg::Matrix<T>::cofactor(std::size_t row, std::size_t col)
{
    return std::pow(-1, row + col) * minor(row, col).determinant();
}

template <typename T>
inline T LinAlg::Matrix<T>::determinant()
{
    if (!square()) { throw std::invalid_argument("square Matrix required"); }

    if (_rows == 1) { 
        return at(0, 0); 
    } else if (_rows == 2) { 
        return at(0, 0) * at(1, 1) - at(0, 1) * at(1, 0); 
    } else {
        T totalDeterminant = 0;
        for (auto i = 0; i < _cols; ++i) {
            totalDeterminant += _matrix[i] * cofactor(0, i);
        }
        return totalDeterminant;
    }
}

template <typename T>
inline LinAlg::Matrix<T> LinAlg::Matrix<T>::minor(std::size_t row, std::size_t col)
{
    if (!square()) { throw std::invalid_argument("square Matrix required"); }
    if (row < 0 || row >= _rows) { throw std::out_of_range("invalid Matrix row subscript"); }
    if (col < 0 || col >= _cols) { throw std::out_of_range("invalid Matrix column subscript"); }

    std::vector<T> tempVector;
    for (auto i = 0; i < _rows; ++i) {
        if (i != row) {
            for (auto j = 0; j < _cols; ++j) {
                if (j != col) { tempVector.push_back(at(i, j)); }
            }
        }
    }
    return LinAlg::Matrix<T>(_rows - 1, _cols - 1, tempVector);
}

template <typename T>
inline LinAlg::Matrix<T> LinAlg::Matrix<T>::adjoint()
{
    if (!square()) { throw std::invalid_argument("square Matrix required"); }

    if (_rows == 1) {
        return LinAlg::Matrix<T>({ { 1 } });
    } else if (_rows == 2) {
        return LinAlg::Matrix<T>({ { at(1, 1), -at(0, 1) }, { -at(1, 0), at(0, 0) } });
    } else {
        LinAlg::Matrix<T> adjointMatrix(_rows, _cols);
        for (auto i = 0; i < _rows; ++i) {
            for (auto j = 0; j < _cols; ++j) {
                adjointMatrix.at(i, j) = cofactor(i, j);
            }
        }
        adjointMatrix.transpose();
        return adjointMatrix;
    }
}

template <typename T>
inline LinAlg::Matrix<T> LinAlg::Matrix<T>::inverse()
{
    if (!square()) { throw std::invalid_argument("square Matrix required"); }
    if (determinant() == 0) { throw std::runtime_error("null determinant"); }

    LinAlg::Matrix<T> inverseMatrix = adjoint() / determinant();
    return inverseMatrix;
}

template <typename T>
inline std::size_t LinAlg::Matrix<T>::check_rows_arg(std::size_t rows)
{
    if (rows < max_rows()) { 
        return rows; 
    } else { 
        throw std::invalid_argument("invalid Matrix rows argument");
    }
}

template <typename T>
inline std::size_t LinAlg::Matrix<T>::check_cols_arg(std::size_t cols)
{
    if (_rows != 0) {
        if (cols < max_cols() && cols > 0) {
            return cols; 
        } else { 
            throw std::invalid_argument("invalid Matrix cols argument");
        }
    } else if (cols == 0) {
        return cols;
    } else {
        throw std::invalid_argument("invalid Matrix cols argument");
    }
}

template <typename T>
inline std::size_t LinAlg::Matrix<T>::check_template_arg(std::size_t size)
{
    if (std::is_arithmetic<T>::value && !std::is_const<T>::value) {
        return size;
    } else {
        throw std::invalid_argument("invalid Matrix template argument");
    }
}

#endif // MATRIX_HPP
