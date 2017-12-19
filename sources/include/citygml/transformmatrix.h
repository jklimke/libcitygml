#pragma once

#include <citygml/object.h>
#include <memory>

namespace citygml {
    class LIBCITYGML_EXPORT TransformationMatrix : public Object
    {
    public:
        TransformationMatrix();


        TransformationMatrix(double* matrix);


        const double* getMatrix() const;
        const double* getTransposedMatrix() const;

        ~TransformationMatrix();


    protected:
        double m_matrix[16];
        double m_transposedMatrix[16];
    };
}
