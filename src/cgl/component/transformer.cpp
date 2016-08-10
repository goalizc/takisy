#include <takisy/core/math.h>
#include <takisy/cgl/path/vertices_wrapper.h>
#include <takisy/cgl/path/vertex_set.h>
#include <takisy/cgl/component/transformer.h>

class transformer::implement
{
    friend class transformer;

public:
    void matrix_multiply(double matrix[3][3])
    {
        double matrix_copy[3][3] = {
            {matrix_[0][0], matrix_[0][1], matrix_[0][2],},
            {matrix_[1][0], matrix_[1][1], matrix_[1][2],},
            {matrix_[2][0], matrix_[2][1], matrix_[2][2],},
        };

        for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            matrix_[i][j] = matrix[i][0] * matrix_copy[0][j]
                          + matrix[i][1] * matrix_copy[1][j]
                          + matrix[i][2] * matrix_copy[2][j];
    }

    void inverse(void)
    {
        double expand_matrix[3][6] = {
            {matrix_[0][0], matrix_[0][1], matrix_[0][2], 1, 0, 0},
            {matrix_[1][0], matrix_[1][1], matrix_[1][2], 0, 1, 0},
            {matrix_[2][0], matrix_[2][1], matrix_[2][2], 0, 0, 1},
        };

        for (int i = 0; i < 3; ++i)
        {
            double base_element = expand_matrix[i][i];

            for (int j = 0; j < 6; ++j)
                expand_matrix[i][j] /= base_element;

            for (int j = 0; j < 3; ++j)
            {
                if (j == i)
                    continue;

                double factor = expand_matrix[j][i];

                for (int k = 0; k < 6; ++k)
                    expand_matrix[j][k] -= expand_matrix[i][k] * factor;
            }
        }

        for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            matrix_[i][j] = expand_matrix[i][j + 3];
    }

    inline path::vertex_type transform(path::vertex_type::axis_type x,
                                       path::vertex_type::axis_type y) const
    {
        return path::vertex_type {
            .x = x * matrix_[0][0] + y * matrix_[0][1] + matrix_[0][2],
            .y = x * matrix_[1][0] + y * matrix_[1][1] + matrix_[1][2],
        };
    }

    vertex_set* transform(const path& path, vertex_set* vertex_set) const
    {
        for (unsigned int i = 0; i < path.size(); ++i)
        {
            const path::vertex_type& vertex = path.fetch_vertex(i);

            vertex_set->append(transform(vertex.x, vertex.y));
        }

        return vertex_set;
    }

private:
    double matrix_[3][3] = {{1, 0, 0},
                            {0, 1, 0},
                            {0, 0, 1}};
    vertex_set transformed_path_;
    paths      transformed_paths_;
};

transformer::transformer(void)
    : impl_(new implement)
{}

transformer::transformer(const transformer& _transformer)
    : transformer()
{
    operator=(_transformer);
}

transformer::~transformer(void)
{
    delete impl_;
}

transformer& transformer::operator=(const transformer& transformer)
{
    if (this != &transformer)
        for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            impl_->matrix_[i][j] = transformer.impl_->matrix_[i][j];

    return *this;
}

transformer& transformer::reset(void)
{
    register double (*matrix)[3] = impl_->matrix_;

    matrix[0][0] = 1; matrix[0][1] = 0; matrix[0][2] = 0;
    matrix[1][0] = 0; matrix[1][1] = 1; matrix[1][2] = 0;
    matrix[2][0] = 0; matrix[2][1] = 0; matrix[2][2] = 1;

    return *this;
}

transformer& transformer::offset(const path::vertex_type& _offset)
{
    return offset(_offset.x, _offset.y);
}

transformer& transformer::offset(double offset_x, double offset_y)
{
    double matrix[3][3] = {{1, 0, offset_x},
                           {0, 1, offset_y},
                           {0, 0, 1       }};

    impl_->matrix_multiply(matrix);

    return *this;
}

transformer& transformer::scale(double _scale)
{
    return scale(_scale, _scale);
}

transformer& transformer::scale(double scale_x, double scale_y)
{
    double matrix[3][3] = {{scale_x, 0,       0},
                           {0,       scale_y, 0},
                           {0,       0,       1}};

    impl_->matrix_multiply(matrix);

    return *this;
}

transformer& transformer::scale(const path::vertex_type& center, double _scale)
{
    return scale(center, _scale, _scale);
}

transformer& transformer::scale(const path::vertex_type& center,
                                double scale_x,
                                double scale_y)
{
    return scale(center.x, center.y, scale_x, scale_y);
}

transformer& transformer::scale(double x, double y, double _scale)
{
    return scale(x, y, _scale, _scale);
}

transformer& transformer::scale(double x, double y,
                                double scale_x,
                                double scale_y)
{
    return offset(-x, -y).scale(scale_x, scale_y).offset(x, y);
}

transformer& transformer::rotate(double angle)
{
    double matrix[3][3] = {{math::cos(angle), -math::sin(angle), 0},
                           {math::sin(angle),  math::cos(angle), 0},
                           {0,                 0,                1}};

    impl_->matrix_multiply(matrix);

    return *this;
}

transformer& transformer::rotate(const path::vertex_type& center, double angle)
{
    return rotate(center.x, center.y, angle);
}

transformer& transformer::rotate(double x, double y, double angle)
{
    return offset(-x, -y).rotate(angle).offset(x, y);
}

transformer& transformer::reflect(const path::vertex_type& axis)
{
    return reflect(axis.x, axis.y);
}

transformer& transformer::reflect(double x, double y)
{
    path::vertex_type unit = path::vertex_type::polar(math::atan2(x, y), 1);

    x = unit.x;
    y = unit.y;

    double matrix[3][3] = {{2 * x * x - 1, 2 * x * y,     0},
                           {2 * x * y,     2 * y * y - 1, 0},
                           {0,             0,             1}};

    impl_->matrix_multiply(matrix);

    return *this;
}

transformer& transformer::shear(double shear_x, double shear_y)
{
    double matrix[3][3] = {{1,       shear_x, 0},
                           {shear_y, 1,       0},
                           {0,       0,       1}};

    impl_->matrix_multiply(matrix);

    return *this;
}

transformer& transformer::shear(const path::vertex_type& center,
                                double shear_x,
                                double shear_y)
{
    return shear(center.x, center.y, shear_x, shear_y);
}

transformer& transformer::shear(double x, double y,
                                double shear_x,
                                double shear_y)
{
    return offset(-x, -y).shear(shear_x, shear_y).offset(x, y);
}

transformer& transformer::inverse(void)
{
    impl_->inverse();

    return *this;
}

transformer transformer::inversed(void) const
{
    return transformer(*this).inverse();
}

path::vertex_type transformer::transform(const path::vertex_type& point) const
{
    return transform(point.x, point.y);
}

path::vertex_type transformer::transform(
    const path::vertex_type::axis_type x,
    const path::vertex_type::axis_type y) const
{
    return impl_->transform(x, y);
}

const path& transformer::transform(const path& path) const
{
    impl_->transformed_path_.clear();

    return *impl_->transform(path, &impl_->transformed_path_);
}

const path& transformer::transform(const path::vertex_type* vertices,
                                   unsigned int size) const
{
    return transform(vertices_wrapper(vertices, size));
}

const paths& transformer::transform(const paths& paths) const
{
    impl_->transformed_paths_.clear();

    for (unsigned int i = 0; i < paths.size(); ++i)
        impl_->transformed_paths_.append(
                impl_->transform(*paths.fetch_path(i), new vertex_set));

    return impl_->transformed_paths_;
}

const paths& transformer::transform(const path* paths, unsigned int size) const
{
    impl_->transformed_paths_.clear();

    for (unsigned int i = 0; i < size; ++i)
        impl_->transformed_paths_.append(
                impl_->transform(paths[i], new vertex_set));

    return impl_->transformed_paths_;
}

