#ifndef layout_hpp_20160113
#define layout_hpp_20160113

class layout::implement
{
    friend class widget;
    friend class layout;
    friend class vertical_layout;

public:
    class spacer final : public widget
    {};

public:
    implement(layout* _this)
        : this_(_this), alignment_(layout::aCenter)
        , margin_({0, 0, 0, 0}), spacing_(0), recalculating_(false)
    {}

public:
    layout::Alignment get_alignment(widget* widget) const
    {
        if (widget->exists_attribute("alignment"))
            return widget->attribute<layout::Alignment>("alignment");

        return alignment_;
    }

    void recalculate(void);

private:
    layout*              this_;
    bool                 vertical_;
    layout::Alignment    alignment_;
    layout::Margin       margin_;
    unsigned int         spacing_;
    std::vector<spacer*> spacers_;
    bool                 recalculating_;
};

#endif // layout_hpp_20160113
