#include "citygml/transformmatrix.h"

namespace citygml {

    TransformationMatrix::TransformationMatrix() : Object("")
    {
       m_matrix = m_transposedMatrix = { 1.0, 0.0, 0.0, 0.0,
                                       0.0, 1.0, 0.0, 0.0,
                                       0.0, 0.0, 1.0, 0.0,
               0.0, 0.0, 0.0, 1.0};
    }

    TransformationMatrix::TransformationMatrix(double* matrix) : Object("")
    {
        for (size_t i = 0; i < 16; ++i) {
            m_matrix[i] = matrix[i];
        }

        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                m_transposedMatrix[i+j*4] = m_matrix[j+i*4];
            }
        }
    }

    double* TransformationMatrix::getMatrix()
    {
        return m_matrix;
    }

    double*TransformationMatrix::getTransposedMatrix()
    {
        return m_transposedMatrix;
    }

}
