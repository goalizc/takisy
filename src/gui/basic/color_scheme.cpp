#include <takisy/gui/basic/color_scheme.h>
#include "impl/color_scheme.hpp"

color_scheme::color_scheme(void)
    : impl_(new implement)
{}

color_scheme::color_scheme(const color_scheme& colorscheme)
    : color_scheme()
{
    operator=(colorscheme);
}

color_scheme::~color_scheme(void)
{
    delete impl_;
}

color_scheme& color_scheme::operator=(const color_scheme& colorscheme)
{
    if (this != &colorscheme)
        *impl_ = *colorscheme.impl_;

    return *this;
}

color color_scheme::theme(void) const
{
    return other("theme");
}

color color_scheme::background(void) const
{
    return other("background");
}

color color_scheme::border(void) const
{
    return other("border");
}

color color_scheme::text(void) const
{
    return other("text");
}

color color_scheme::selection(void) const
{
    return other("selection");
}

color color_scheme::inactive_text(void) const
{
    return other("inactive_text");
}

color color_scheme::inactive_selection(void) const
{
    return other("inactive_selection");
}

color color_scheme::hyperlink(void) const
{
    return other("hyperlink");
}

color color_scheme::other(const char* name) const
{
    return impl_->other(name);
}

void color_scheme::theme(const color& color)
{
    other("theme", color);
}

void color_scheme::background(const color& color)
{
    other("background", color);
}

void color_scheme::border(const color& color)
{
    other("border", color);
}

void color_scheme::text(const color& color)
{
    other("text", color);
}

void color_scheme::selection(const color& color)
{
    other("selection", color);
}

void color_scheme::inactive_text(const color& color)
{
    other("inactive_text", color);
}

void color_scheme::inactive_selection(const color& color)
{
    other("inactive_selection", color);
}

void color_scheme::hyperlink(const color& color)
{
    other("hyperlink", color);
}

void color_scheme::other(const char* name, const color& color)
{
    impl_->scheme_[name] = color;

    if (impl_->host_ && this == impl_->host_->p_color_scheme())
        impl_->host_->repaint();
}

color_scheme& color_scheme::default_color_scheme(void)
{
    static color_scheme default_color_scheme;

    return default_color_scheme;
}
