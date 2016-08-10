#include <takisy/gui/basic/color_scheme.h>

class color_scheme::implement
{
    friend class color_scheme;

public:
    implement(const color& main, const color& cool, const color& warm)
        : main_(main), cool_(cool), warm_(warm)
    {}

public:
    static color foo(color color, signed char degree)
    {
        if (degree < 0)
            return color.blend(color::black(-degree));
        else
            return color.blend(color::white(+degree));
    }

private:
    color main_, cool_, warm_;
    static color_scheme default_color_scheme_;
};

color_scheme color_scheme::implement::default_color_scheme_
        (color::rgb(0x1abc9c), color::rgb(0x5a6673), color::rgb(0xe96151));

color_scheme::color_scheme(void)
    : color_scheme(default_color_scheme())
{}

color_scheme::color_scheme(const color& main,
                           const color& cool, const color& warm)
    : impl_(new implement(main, cool, warm))
{}

color_scheme::color_scheme(const color_scheme& colorscheme)
    : color_scheme(colorscheme.main(), colorscheme.cool(), colorscheme.warm())
{}

color_scheme::~color_scheme(void)
{
    delete impl_;
}

color_scheme& color_scheme::operator=(const color_scheme& color_scheme)
{
    if (this != &color_scheme)
    {
        impl_->main_ = color_scheme.impl_->main_;
        impl_->cool_ = color_scheme.impl_->cool_;
        impl_->warm_ = color_scheme.impl_->warm_;
    }

    return *this;
}

color color_scheme::main(void) const
{
    return impl_->main_;
}

color color_scheme::main(signed char degree) const
{
    return implement::foo(impl_->main_, degree);
}

color color_scheme::cool(void) const
{
    return impl_->cool_;
}

color color_scheme::cool(signed char degree) const
{
    return implement::foo(impl_->cool_, degree);
}

color color_scheme::warm(void) const
{
    return impl_->warm_;
}

color color_scheme::warm(signed char degree) const
{
    return implement::foo(impl_->warm_, degree);
}

void color_scheme::main(const color& main)
{
    impl_->main_ = main;
}

void color_scheme::cool(const color& cool)
{
    impl_->cool_ = cool;
}

void color_scheme::warm(const color& warm)
{
    impl_->warm_ = warm;
}

color_scheme& color_scheme::default_color_scheme(void)
{
    return implement::default_color_scheme_;
}

const color_scheme& color_scheme::seven_eleven(void)
{
    static const color_scheme instance
            (color::rgb(0x3f7700), color::rgb(0x2e1bec), color::rgb(0x2182f4));

    return instance;
}
