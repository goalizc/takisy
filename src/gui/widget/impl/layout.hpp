#ifndef layout_hpp_20160113
#define layout_hpp_20160113

class layout::implement
{
    friend class layout;
    friend class horizontal_layout;
    friend class vertical_layout;
    friend class grid_layout;

public:
    implement(layout* _this)
        : this_(_this), readapting_(false)
        , alignment_(aCenter), margin_({0, 0, 0, 0}), spacing_(0)
    {}

public:
    Alignment get_alignment(widget* widget) const
    {
        if (widget->exists_attribute("alignment"))
            return widget->attribute<Alignment>("alignment");

        return alignment_;
    }

    std::vector<widget*> visible_children(void) const
    {
        std::vector<widget*> children = this_->children();

        children.erase(std::remove_if(children.begin(), children.end(),
                                      [](widget* child) -> bool
                                      {
                                         return !child->visible();
                                      }),
                       children.end());

        return children;
    }

    void readapt(void)
    {
        if (readapting_)
            return;

        struct david
        {
            bool& readapting;
        public:
            david(bool& readapting) : readapting(readapting)
                        { readapting = true; }
           ~david(void) { readapting = false; }
        } __david__(readapting_);

        this_->readapt();
    }

private:
    layout*       this_;
    bool          readapting_;
    Alignment     alignment_;
    struct margin margin_;
    unsigned int  spacing_;
};

#endif // layout_hpp_20160113
