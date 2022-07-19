#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace LinAlg
{
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

        std::size_t rows() const { return _rows; }
        std::size_t cols() const { return _cols; }
        std::size_t vector_size() const { return _matrix.size(); }
        std::size_t max_rows() { return _matrix.max_size(); };
        std::size_t max_cols() { return max_rows() / _rows; };

        T& at(std::size_t row, std::size_t col);
        const T& at(std::size_t row, std::size_t col) const;

    private:
        std::size_t _rows;
        std::size_t _cols;
        std::vector<T> _matrix;

        std::size_t check_rows_arg(std::size_t rows);
        std::size_t check_cols_arg(std::size_t cols);
        std::size_t check_template_arg(std::size_t size);
    };

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
    _rows = other._rows;
    _cols = other._cols;
    _matrix = std::move(other._matrix);

    other._rows = 0;
    other._cols = 0;

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