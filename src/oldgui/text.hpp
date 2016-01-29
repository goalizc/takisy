class Text {
    struct Action {
        enum Type { atInsert, atErase, } type;
        unsigned int offset;
        std::wstring content;
    };

public:
    Text(const std::wstring& text)
        : text_(text), caret_({-1, (long int)text_.size()})
        , area_(0, 0, 0, 0), canvas_(1e8, 1e8)
        , font_(tfont_simple::get())
        , multiline_(false)
        , word_wrap_(false)
        , brush1_(makeColorBrushSP(Color::argb(0xff318dfd)))
        , brush2_(makeColorBrushSP(Color::argb(0xff000000)))
        , action_index_(-1)
    {
        reconfigure();
    }

public:
    const std::wstring& text(void) const {
        return text_;
    }

    std::wstring selected_text(void) const {
        if (caret_.s >= 0 && caret_.s != caret_.e)
            return text_.substr(caret_.lower(), caret_.upper() - caret_.lower());
        else
            return L"";
    }

    unsigned int caret(void) const {
        return caret_.e;
    }

    Point offset(void) const {
        return area_.left_top();
    }

    unsigned int width(void) const {
        return area_.width();
    }

    unsigned int height(void) const {
        return area_.height();
    }

    Size canvas(void) const {
        return canvas_;
    }

    const class font* font(void) const {
        return font_;
    }

    bool multiline(void) const {
        return multiline_;
    }

    bool word_wrap(void) const {
        return word_wrap_;
    }

    std::shared_ptr<Brush> brush1(void) const {
        return brush1_;
    }

    std::shared_ptr<Brush> brush2(void) const {
        return brush2_;
    }

public:
    void text(const std::wstring& text) {
        if (!text_.empty()) {
            select(0, text_.size());
            erase();
        }
        typewrite(text);
    }

    void offset(int left, int top) {
        if (left == area_.left && top == area_.top)
            return;
        area_ = area_.move(left, top);
    }

    void offset_left(int left) {
        offset(left, area_.top);
    }

    void offset_top(int top) {
        offset(area_.left, top);
    }

    void resize(unsigned int width, unsigned int height) {
        canvas_.width  = width;
        canvas_.height = height;
        if (word_wrap_)
            reconfigure();
    }

    void font(const Font* font) {
        font_ = font;
        reconfigure();
    }

    void multiline(bool multiline) {
        if (multiline == multiline_)
            return;
        multiline_ = multiline;
        reconfigure();
    }

    void word_wrap(bool word_wrap) {
        if (word_wrap == word_wrap_)
            return;
        word_wrap_ = word_wrap;
        reconfigure();
    }

    void brush1(const std::shared_ptr<Brush>& brush1) {
        if (brush1 == brush1_)
            return;
        brush1_ = brush1;
    }

    void brush2(const std::shared_ptr<Brush>& brush2) {
        if (brush2 == brush2_)
            return;
        brush2_ = brush2;
    }

public:
    void typewrite(wchar_t chr) {
        erase();
        if (chr == '\n' && !multiline_)
            return;
        action_insert(caret_.e++, &chr, 1);
        reconfigure();
        adapt();
    }

    void typewrite(const std::wstring& str) {
        erase();
        if (str.empty())
            return;
        action_insert(caret_.e, str.c_str(), str.size());
        caret_.e += str.size();
        reconfigure();
        adapt();
    }

    void erase(int offset = 0) {
        if (caret_.s == caret_.e)
            caret_.s = -1;

        if (caret_.s >= 0) {
            action_erase(caret_.lower(), caret_.upper() - caret_.lower());
            caret_.e = caret_.lower();
            caret_.s = -1;
        } else if (offset < 0) {
            if (offset < -caret_.e)
                offset = -caret_.e;
            if (offset >= 0)
                return;
            action_erase(caret_.e += offset, -offset);
        } else if (offset == 0) {
            return;
        } else if (offset > 0) {
            if (caret_.e == static_cast<long>(text_.size()))
                return;
            action_erase(caret_.e, offset);
        }

        reconfigure();
        adapt();
    }

    void move(int caret_e, bool shift, bool ctrl=false) {
        int fix_e = caret_e;

        if (caret_e < 0)
            fix_e = 0;
        else if (caret_e > static_cast<int>(text_.size()))
            fix_e = text_.size();

        if (caret_.s >= 0) {
            if (shift)
                caret_.e = fix_e;
            else {
                caret_.e = caret_e > caret_.e ? caret_.upper() : caret_.lower();
                caret_.s = -1;
            }
        } else {
            if (shift)
                caret_.s = caret_.e;
            caret_.e = fix_e;
        }

        adapt();
    }

    void select(int start, int end) {
        caret_.s = start;
        caret_.e = end;

        if (caret_.s < 0)
            caret_.s = -1;
        if (caret_.e < 0)
            caret_.e = 0;
        else if (caret_.e > static_cast<long>(text_.size()))
            caret_.e = text_.size();
        if (caret_.s == caret_.e)
            caret_.s = -1;
    }

    void select_similar(int caret) {
        bool mode = stralgo::iswspace(text_[caret]);
        int i = caret - 1, j = caret + 1, text_size = text_.size();
        while (i >= 0 && stralgo::iswspace(text_[i]) == mode) --i;
        while (j < text_size && stralgo::iswspace(text_[j]) == mode) ++j;
        select(i + 1, j);
    }

    void cancel_select(void) {
        select(-1, caret_.e);
    }

    unsigned int hit_test(Point point) const {
        if (point.y < 0)
            return 0;
        else
        if (point.y >= line_count() * font_->height())
            return text_.size();
        else {
            std::pair<unsigned int, unsigned int>
                range = line_range(point.y / font_->height());
            int x = 0;

            for (unsigned int i = range.first; i < range.second; ++i) {
                int advance = font_->get_bitmap(text_[i])->advance;
                if (x + advance / 2 > point.x)
                    return i;
                x += advance;
            }

            return range.second;
        }
    }

    Point caret_point(void) const {
        unsigned int index = line_index(caret_.e);
        std::pair<unsigned int, unsigned int> range = line_range(index);
        int x = 0;

        for (long i = range.first; i < caret_.e; ++i)
            x += font_->get_bitmap(text_[i])->advance;

        return Point(x, index * font_->height());
    }

    unsigned int line_count(void) const {
        unsigned int line_count = 0;
        for (const std::vector<unsigned int>& lines : paragraphs_)
            line_count += lines.size();
        return line_count;
    }

    unsigned int line_index(unsigned int caret) const {
        unsigned int line_offset = 0, line_index = 0;
        for (const std::vector<unsigned int>& lines : paragraphs_) {
            for (unsigned int words : lines)
                if (line_offset + words >= caret)
                    return line_index;
                else
                    line_offset += words, ++line_index;
            ++line_offset;
        }
        return line_index;
    }

    std::pair<unsigned int, unsigned int> line_range(unsigned int line) const {
        unsigned int line_offset = 0, line_index = 0;
        for (const std::vector<unsigned int>& lines : paragraphs_) {
            for (unsigned int words : lines)
                if (line_index == line)
                    return std::make_pair(line_offset, line_offset + words);
                else
                    line_offset += words, ++line_index;
            ++line_offset;
        }
        return std::make_pair(0, 0);
    }

    std::pair<unsigned int, unsigned int>
        paragraph_range(unsigned int caret) const {
        unsigned int offset = 0;
        for (const std::vector<unsigned int>& paragraph : paragraphs_) {
            unsigned int words = std::accumulate(paragraph.begin(),
                                                 paragraph.end(), 0);
            if (offset + words > caret)
                return std::make_pair(offset, offset + words);
            else
                offset += words + 1;
        }
        return std::make_pair(0, 0);
    }

    void draw(Graphics& graphics, Point origin, Rect paint_area) {
        unsigned int sel_end = caret_.upper();
        unsigned int sel_start = caret_.s >= 0 ? caret_.lower() : sel_end;
        unsigned int font_height = font_->height();
        unsigned int caret = 0;
        int y = area_.top + origin.y;

        for (const std::vector<unsigned int>& lines : paragraphs_) {
            for (unsigned int words : lines) {
                if (y >= paint_area.bottom)
                    return;
                if (y + (int)font_height > paint_area.top) {
                    int x = area_.left + origin.x;
                    for (unsigned int wi = caret, i = 0; i < words; ++wi, ++i) {
                        if (x >= paint_area.right)
                            continue;
                        const bitmap* bitmap = font_->get_bitmap(text_[wi]);
                        int advance = bitmap->advance;
                        if (x + advance > paint_area.left) {
                            if (sel_start <= wi && wi < sel_end)
                                graphics.fillRectangle(x, y,
                                                       x + advance,
                                                       y + font_height,
                                                       *brush1_);
                            bitmap->render(graphics, x, y, *brush2_);
                        }
                        x += advance;
                    }
                }
                caret += words;
                y += font_height;
            }
            caret++;
        }
    }

    bool undo(void) {
        if (action_index_ == -1)
            return false;
        const Action& action = actions_[action_index_--];
        if (action.type == Action::Type::atInsert) {
            text_.erase(action.offset, action.content.size());
            select(-1, action.offset);
        } else if (action.type == Action::Type::atErase) {
            text_.insert(action.offset, action.content);
            select(action.offset, action.offset + action.content.size());
        }
        reconfigure();
        return true;
    }

    bool redo(void) {
        if (action_index_ >= (int)actions_.size() - 1)
            return false;
        const Action& action = actions_[++action_index_];
        if (action.type == Action::Type::atInsert) {
            text_.insert(action.offset, action.content);
            select(action.offset, action.offset + action.content.size());
        } else if (action.type == Action::Type::atErase) {
            text_.erase(action.offset, action.content.size());
            select(-1, action.offset);
        }
        reconfigure();
        return true;
    }

private:
    inline void push_action(const Action& action) {
        if (action_index_ + 1 < (int)actions_.size())
            actions_.resize(action_index_ + 1);
        actions_.push_back(action);
        action_index_++;
    }

    void action_insert(unsigned int offset,
                       const wchar_t* str, unsigned int count) {
        Action action;
        action.type    = Action::Type::atInsert;
        action.offset  = offset;
        action.content = std::wstring(str, count);
        push_action(action);
        text_.insert(offset, str, count);
    }

    void action_erase(unsigned int offset, unsigned int count) {
        Action action;
        action.type    = Action::Type::atErase;
        action.offset  = offset;
        action.content = text_.substr(offset, count);
        push_action(action);
        text_.erase(offset, count);
    }

    void reconfigure(void) {
        int right = word_wrap_ ? canvas_.width : (1 << 30);
        int origin_x = word_wrap_ ? 0 : area_.left;
        unsigned int line_start = 0;
        int x = origin_x;

        area_.right = x;
        paragraphs_.clear();
        paragraphs_.push_back(std::vector<unsigned int>());

        for (unsigned int i = 0; i < text_.size(); ++i) {
            if (text_[i] == '\n') {
                if (area_.right < x)
                    area_.right = x;
                paragraphs_.back().push_back(i - line_start);
                paragraphs_.push_back(std::vector<unsigned int>());
                line_start = i + 1;
                x = origin_x;
            } else {
                int advance = font_->get_bitmap(text_[i])->advance;
                if (x + advance > right) {
                    if (area_.right < x)
                        area_.right = x;
                    paragraphs_.back().push_back(i - line_start);
                    line_start = i;
                    x = origin_x + advance;
                }
                else
                    x += advance;
            }
        }

        paragraphs_.back().push_back(text_.size() - line_start);

        if (area_.right < x)
            area_.right = x;
        area_.bottom = area_.top + line_count() * font_->height();
    }

    void adapt(void) {
        Point offset = area_.left_top();
        if (area_.right < canvas_.width) {
            offset.x += canvas_.width - area_.right;
            if (offset.x > 0)
                offset.x = 0;
        }
        if (area_.bottom < canvas_.height) {
            offset.y += canvas_.height - area_.bottom;
            if (offset.y > 0)
                offset.y = 0;
        }

        Point caret = caret_point() + offset;
        if (caret.x < 0)
            offset.x -= caret.x;
        else if (caret.x > canvas_.width)
            offset.x -= caret.x - canvas_.width;
        if (caret.y < 0)
            offset.y -= caret.y;
        else if (caret.y > canvas_.height - font_->emheight())
            offset.y -= caret.y - canvas_.height + font_->emheight();

        area_ = area_.offset(offset - area_.left_top());
    }

private:
    std::wstring text_;
    struct Caret {
        long s, e;
    public:
        inline long lower(void) const { return s < e ? s : e; }
        inline long upper(void) const { return s > e ? s : e; }
    } caret_;

    Rect area_;
    Size canvas_;
    const Font* font_;
    bool multiline_, word_wrap_;
    std::shared_ptr<Brush> brush1_, brush2_;

    std::vector<std::vector<unsigned int>> paragraphs_;

    int action_index_;
    std::vector<Action> actions_;
};
