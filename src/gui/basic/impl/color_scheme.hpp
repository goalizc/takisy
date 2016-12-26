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
        scheme_["inactive_selection"] = color::dodger_blue(85);
        scheme_["hyperlink"]          = color::blue();
    }

public:
    inline void host(widget* host)
    {
        host_ = host;
    }

    inline color other(const std::string& name, unsigned char alpha)
    {
        if (host_ && host_->disabled())
            return scheme_[name] * alpha;
        else
            return scheme_[name];
    }

private:
    widget* host_;
    std::map<std::string, color> scheme_;
};

#endif // color_scheme_hpp_20160908
