#include <map>
#include <takisy/core/sys.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/widget/button.h>
#include <takisy/gui/widget/combo.h>

class combo::implement
{
    friend class combo;

    class combolist : public list
    {
    public:
        combolist(const std::vector<std::wstring>& items)
            : list(items)
        {}

    public:
        void onEndPaint(graphics graphics, Rect rect) override
        {
            rectf cr = client_rect();

            graphics.draw_rectangle(cr.inflate(-0.5), color_scheme().border());
        }

        bool onKeyPress(unsigned int chr) override
        {
            if (chr == 13)
                onItemClickedHandle(current(), sys::btnLeft);
            else
                return list::onKeyPress(chr);

            return true;
        }

        bool onMouseMove(Point point) override
        {
            int index = hittest(point);

            if (index != -1)
            {
                clear_selection();
                select(index);
            }

            return true;
        }
    };

    class popup : public button
    {
        void onPaint(graphics graphics, Rect rect) override
        {
            vertices vertices;

            vertices.append(relative(0.25, 0.4));
            vertices.append(relative(0.75, 0.4));
            vertices.append(relative(0.50, 0.6));

            graphics.fill_path(vertices, color_scheme().theme());
        }

    private:
        inline pointf relative(double x, double y) const
        {
            return pointf(x * width(), y * height());
        }
    };

public:
    implement(const std::vector<std::wstring>& items)
        : list_(items)
    {
        text_.word_wrap(false);
        text_.multiline(false);
        text_.vertical_scroll().hide();
        text_.horizontal_scroll().hide();
        text_.show();

        popup_.show();
    }

    ~implement(void)
    {
        list_.window().close();
    }

private:
    text_edit text_;
    popup     popup_;
    combolist list_;
};

combo::combo(void)
    : combo(std::vector<std::wstring>())
{}

combo::combo(const std::vector<std::string>& items)
    : combo(items, sys::default_codec())
{}

combo::combo(const std::vector<std::string>& items, const std::string& codec)
    : combo([&items, &codec](void) -> std::vector<std::wstring>
    {
        std::vector<std::wstring> result;

        for (const std::string& item : items)
            result.push_back(stralgo::decode(item, codec));

        return result;
    }())
{}

combo::combo(const std::vector<std::wstring>& items)
    : impl_(new implement(items))
{
    impl_->list_.upper_height(128);
    impl_->list_.onItemClicked(
        [this](class list* self, unsigned int index, sys::Button button)
        {
            if (button == sys::btnLeft)
            {
                select(index);
                self->window().close();
            }
        });
    impl_->list_.onItemSelected(
        [this](class list* self, unsigned int index)
        {
            self->current(index);
        });
    impl_->popup_.onClick(
        [this](button* self)
        {
            class color_scheme cs = color_scheme();
            int x = screen_x(), y = screen_y() + height();

            if (y + (int)impl_->list_.height() > GetSystemMetrics(SM_CYSCREEN))
                y = screen_y() - impl_->list_.height();

            impl_->list_.hide();
            impl_->list_.size(width(), impl_->list_.optimal().height);
            impl_->list_.xy(x, y);
            impl_->list_.as_window(window::wsDropShadow | window::wsEvanescent |
                                   window::wsNoActivate | window::wsPopup |
                                   window::wsTopmost);
            impl_->list_.focus(true);
            impl_->list_.color_scheme(&cs);
            impl_->list_.show();

            handler::sptr handler = forefather()->window().event().onDestroy(
                [this](void*)
                {
                    impl_->list_.window().close();
                });
            impl_->list_.window().event().onDestroy(
                [this, handler](void*)
                {
                    window().event().onDestroyRemove(handler);
                });
        });

    add(&impl_->text_);
    add(&impl_->popup_);
}

combo::~combo(void)
{
    delete impl_;
}

std::wstring combo::selected(void) const
{
    unsigned int index = selected_index();

    if (index < impl_->list_.count())
        return impl_->list_.item(index);

    return L"";
}

unsigned int combo::selected_index(void) const
{
    std::set<unsigned int> selected = impl_->list_.selected();
    if (selected.empty())
        return impl_->list_.count();

    unsigned int index = *selected.begin();
    if (index >= impl_->list_.count()
        || impl_->list_.item(index) != impl_->text_.text())
        return impl_->list_.count();

    return *selected.begin();
}

text_edit& combo::text(void)
{
    return impl_->text_;
}

const text_edit& combo::text(void) const
{
    return impl_->text_;
}

class list& combo::list(void)
{
    return impl_->list_;
}

const class list& combo::list(void) const
{
    return impl_->list_;
}

void combo::select(unsigned int index)
{
    if (index < impl_->list_.count())
    {
        impl_->list_.clear_selection();
        impl_->list_.select(index);
        impl_->text_.text(impl_->list_.item(index));
        onSelectionChangedHandle();
    }
}

void combo::onSize(void)
{
    impl_->popup_.upper_width(height());

    horizontal_layout::onSize();
}

void combo::onEndPaint(graphics graphics, Rect)
{
    color selcolor = color_scheme().selection();
    rectf rect = client_rect();

    if (impl_->text_.readonly())
        for (double i = 0.5; i < height(); i += 1)
            graphics.draw_line(0, i, width(), i,
                               selcolor * (i * 96.0 / height() + 32));

    graphics.draw_rectangle(rect.inflate(-0.5), color_scheme().border());
}

bool combo::onMouseMove(Point point)
{
    return true;
}
