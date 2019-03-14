#ifndef color_scheme_hpp_20160908
#define color_scheme_hpp_20160908

#include <map>
#include <string>
#include <takisy/gui/widget/widget.h>

class color_scheme::implement
{
    friend class color_scheme;

public:
    implement(void)
        : host_(nullptr)
    {
        scheme_["theme"]              = color::gray();
        scheme_["background"]         = color::white();
        scheme_["border"]             = color::dim_gray();
        scheme_["text"]               = color::black();
        scheme_["selection"]          = color::dodger_blue();
        scheme_["inactive_text"]      = color::black();
        scheme_["inactive_selection"] = color::dodger_blue(128);
        scheme_["hyperlink"]          = color::blue();
    }

public:
    widget* host(void) const
    {
        return host_;
    }

public:
    void host(widget* host)
    {
        host_ = host;
    }

    color other(const std::string& name)
    {
        if (host_ && host_->disabled())
            return scheme_[name] * 128;
        else
            return scheme_[name];
    }

private:
    widget* host_;
    std::map<std::string, color> scheme_;
};

#endif // color_scheme_hpp_20160908
