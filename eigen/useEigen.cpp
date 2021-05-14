#include <iostream>
#include <ctime>

#include <Eigen/Core>  // Eigen core
#include <Eigen/Dense>  //  Algebraic operations of dense matrices (inverse, eigenvalues, etc.)

using namespace std;
using namespace Eigen;

#define MATRIX_SIZE 50

int main() {

    std::cout << "Hello, Eigen!" << std::endl;

    // float matrix row 2 cols 3
    Matrix<float, 2, 3> matrix_23;

    //Three-dimensional vector (Eigen::Matrix<double, 3, 1>)
    Vector3d v_3d;
    // This is the same
    Matrix<float, 3, 1> vd_3d;

    // Matrix3d is essentially Eigen::Matrix<double, 3, 3>
    Matrix3d matrix_33 = Matrix3d::Zero(); // initialized to zero
    // If you are not sure about the size of the matrix, you can use a matrix of
    // dynamic size
    Matrix<double, Dynamic, Dynamic> matrix_dynamic;
    // or simpler
    MatrixXd matrix_x;

    // input data (initialization)
    matrix_23 << 1, 2, 3, 4, 5, 6;
    // output
    cout << "matrix 2x3 from 1 to 6: \n" << matrix_23 << endl;

    //  Use () to access elements in the matrix
    cout << "print matrix 2x3: " << endl;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) cout << matrix_23(i, j) << "\t";
        cout << endl;
    }

    // We can easily multiply a matrix with a vector (but actually still matrices and matrices)
    v_3d << 3, 2, 1;
    vd_3d << 4, 5, 6;

    // In Eigen you can't mix two different types of matrices, like this is
    // wrong Matrix<double, 2, 1> result_wrong_type = matrix_23 ∗ v_3d;
    // It should be explicitly converted
    Matrix<double, 2, 1> result = matrix_23.cast<double>() * v_3d;
    cout << "[1,2,3;4,5,6]*[3,2,1]=" << result.transpose() << endl;

    Matrix<float, 2, 1> result_wrong_type = matrix_23 * vd_3d;
    cout << "[1,2,3;4,5,6]*[4,5,6]: " << result_wrong_type.transpose() << " (wrong)" << endl;

    // Also you can't misjudge the dimensions of the matrix
    // Try canceling the comments below to see what Eigen will report.
    // Eigen::Matrix<double, 2, 3> result_wrong_dimension = matrix_23.cast<double>() * v_3d;

    //  some matrix operations
    // The basic operations are not demonstrated, just use +−∗/ operators.
    matrix_33 = Matrix3d::Random();  // random number matrix
    cout << "random matrix: \n" << matrix_33 << endl;

    // https://en.wikipedia.org/wiki/Transpose
    cout << "transpose: \n" << matrix_33.transpose() << endl;
    cout << "sum: " << matrix_33.sum() << endl;
    cout << "trace: " << matrix_33.trace() << endl;
    cout << "times 10: \n" << 10 * matrix_33 << endl;

    // https://www.mathsisfun.com/algebra/matrix-inverse.html
    cout << "inverse: \n" << matrix_33.inverse() << endl;

    // https://www.mathsisfun.com/algebra/matrix-determinant.html
    cout << "det: " << matrix_33.determinant() << endl;

    // Eigenvalues
    // https://mathworld.wolfram.com/Eigenvalue.html#:~:text=Eigenvalues%20are%20a%20special%20set,144).
    // Real symmetric matrix can guarantee successful diagonalizing
    SelfAdjointEigenSolver<Matrix3d> eigen_solver(matrix_33.transpose() * matrix_33);
    cout << "Eigen values = \n" << eigen_solver.eigenvalues() << endl;
    cout << "Eigen vectors = \n" << eigen_solver.eigenvectors() << endl;

    // Solving equations
    // We solve the equation of matrix_NN ∗ x = v_Nd
    //  The size of N is defined in the previous macro, which is generated by a
    // random number Direct inversion is the most direct, but the amount of inverse operations is large

    Matrix<double, MATRIX_SIZE, MATRIX_SIZE> matrix_NN
            = MatrixXd::Random(MATRIX_SIZE, MATRIX_SIZE);
    matrix_NN = matrix_NN * matrix_NN.transpose();  //  Guarantee semi−positive definite
    Matrix<double, MATRIX_SIZE, 1> v_Nd = MatrixXd::Random(MATRIX_SIZE, 1);

    clock_t time_stt = clock(); // timing
    //  Direct inversion
    Matrix<double, MATRIX_SIZE, 1> x = matrix_NN.inverse() * v_Nd;
    cout << "time of normal inverse is "
         << 1000 * (clock() - time_stt) / (double) CLOCKS_PER_SEC << "ms" << endl;
    cout << "x = " << x.transpose() << endl;

    // Usually solved by matrix decomposition, such as QR decomposition, the speed
    //  will be much faster
    time_stt = clock();
    x = matrix_NN.colPivHouseholderQr().solve(v_Nd);
    cout << "time of Qr decomposition is "
         << 1000 * (clock() - time_stt) / (double) CLOCKS_PER_SEC << "ms" << endl;
    cout << "x = " << x.transpose() << endl;

    // For positive definite matrices, you can also use cholesky decomposition to solve equations.
    time_stt = clock();
    x = matrix_NN.ldlt().solve(v_Nd);
    cout << "time of ldlt decomposition is "
         << 1000 * (clock() - time_stt) / (double) CLOCKS_PER_SEC << "ms" << endl;
    cout << "x = " << x.transpose() << endl;

    return 0;
}
