#pragma once

#include <citygml/object.h>

namespace citygml {
    class TransformationMatrix : public Object
    {
    public:
        TransformationMatrix();


        TransformationMatrix(double* matrix);


        double* getMatrix();
        double* getTransposedMatrix();


    protected:
        double m_matrix[16];
        double m_transposedMatrix[16];
    };
}
