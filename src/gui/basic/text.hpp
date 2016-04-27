#ifndef text_hpp_20151208
#define text_hpp_20151208

#include <tuple>
#include <string>
#include <vector>
#include <algorithm>
#include <takisy/core/handler.h>
#include <takisy/core/algorithm.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/cgl/basic/point.h>
#include <takisy/cgl/basic/size.h>
#include <takisy/cgl/basic/rect.h>
#include <takisy/cgl/font/font.h>
#include <takisy/cgl/font/tfont_simple.h>
#include <takisy/gui/basic/brush.h>
#include <takisy/gui/basic/define.h>
#include <takisy/gui/basic/graphics.h>

class text {
    static const unsigned int xxdo_limit = 16;

    struct action {
        enum Type { atInsert, atErase, } type;
        unsigned int offset;
        std::wstring content;
    };

    struct isx {
        typedef int (*isx_f)(int);
        static int isvname(int c)
            { return ::isalpha(c) || ::isdigit(c) || c == '_'; }
        static int isfalse(int) { return false; }
        static isx_f get_isx(int c) {
            if (isvname(c)) return isvname;
            else if (::isblank(c)) return ::isblank;
            else if (::ispunct(c)) return ::ispunct;
            else return isfalse;
        }
    };

    DECLARE_HANDLER(onCaretPositionChanged);

public:
    struct line {
        int words, width;
    };

public:
    text(void)
        : text(L"")
    {}

    text(const std::wstring& content)
        : content_(content), max_length_(~0u)
        , caret_({-1, (long)content_.size()})
        , world_(0, 0, 0, 0), view_(~0u >> 1, ~0u >> 1)
        , fixed_brush_(true), margin_({0}), indent_(0), alignment_(aLeftTop)
        , multiline_(true), word_wrap_(true)
        , line_spacing_(0), word_spacing_(0)
        , font_(tfont_simple::get())
        , selbrush_(make_color_brush_sptr(color::argb(0xff318dfd)))
        , wrtbrush_(make_color_brush_sptr(color::argb(0xff000000)))
        , actidx_(-1)
    {
        recalculate();
    }

public:
    const std::wstring& content(void) const {
        return content_;
    }

    std::wstring selected_content(void) const {
        if (caret_.s >= 0 && caret_.s != caret_.e)
            return content_.substr(caret_.lower(),
                                   caret_.upper() - caret_.lower());
        return L"";
    }

    unsigned int max_length(void) const {
        return max_length_;
    }

    unsigned int caret(void) const {
        return caret_.e;
    }

    rect world(void) const {
        return world_;
    }

    point offset(void) const {
        return world_.left_top();
    }

    unsigned int width(void) const {
        return world_.width();
    }

    unsigned int height(void) const {
        return world_.height();
    }

    size view(void) const {
        return view_;
    }

    bool fixed_brush(void) const {
        return fixed_brush_;
    }

    Margin margin(void) const {
        return margin_;
    }

    unsigned int indent(void) const {
        return indent_;
    }

    Alignment alignment(void) const {
        return alignment_;
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

    unsigned int line_spacing(void) const {
        return line_spacing_;
    }

    unsigned int word_spacing(void) const {
        return word_spacing_;
    }

    brush_sptr selection_brush(void) const {
        return selbrush_;
    }

    brush_sptr writing_brush(void) const {
        return wrtbrush_;
    }

public:
    void content(const std::wstring& content) {
        if (!content_.empty()) {
            select(0, content_.size());
            erase();
        }
        typewrite(content);
        clear_actions();
    }

    void max_length(unsigned int max_length) {
        max_length_ = max_length;
    }

    void offset(int left, int top) {
        if (word_wrap_)
            left = 0;
        if (!fixed_brush_) {
            selbrush_->offset(-left, -top);
            wrtbrush_->offset(-left, -top);
        }
        world_ = world_.move(left, top);
    }

    void offset_left(int left) {
        offset(left, world_.top);
    }

    void offset_top(int top) {
        offset(world_.left, top);
    }

    void view(unsigned int width, unsigned int height) {
        view_.height = height;
        if (view_.width != (int)width) {
            view_.width  = width;
            if (word_wrap_ || (int)width < world_.width())
                recalculate();
        }
    }

    void fixed_brush(bool fixed) {
        fixed_brush_ = fixed;
        if (fixed) {
            selbrush_->offset(0, 0);
            wrtbrush_->offset(0, 0);
        } else {
            selbrush_->offset(-world_.left, -world_.top);
            wrtbrush_->offset(-world_.left, -world_.top);
        }
    }

    void margin(int _margin) {
        margin(_margin, _margin, _margin, _margin);
    }

    void margin(int left, int top, int right, int bottom) {
        margin_.left   = left;
        margin_.top    = top;
        margin_.right  = right;
        margin_.bottom = bottom;
        recalculate();
    }

    void margin_left(int left) {
        margin(left, margin_.top, margin_.right, margin_.bottom);
    }

    void margin_top(int top) {
        margin(margin_.left, top, margin_.right, margin_.bottom);
    }

    void margin_right(int right) {
        margin(margin_.left, margin_.top, right, margin_.bottom);
    }

    void margin_bottom(int bottom) {
        margin(margin_.left, margin_.top, margin_.right, bottom);
    }

    void indent(unsigned int indent) {
        if (indent == indent_)
            return;
        indent_ = indent;
        recalculate();
    }

    void alignment(Alignment alignment) {
        alignment_ = alignment;
    }

    void font(const class font* font) {
        if (font == font_)
            return;
        font_ = font;
        recalculate();
    }

    void multiline(bool multiline) {
        if (multiline == multiline_)
            return;
        multiline_ = multiline;
        recalculate();
    }

    void word_wrap(bool word_wrap) {
        if (word_wrap == word_wrap_)
            return;
        word_wrap_ = word_wrap;
        if (word_wrap_)
            offset_left(0);
        recalculate();
    }

    void line_spacing(unsigned int line_spacing) {
        if (line_spacing == line_spacing_)
            return;
        line_spacing_ = line_spacing;
    }

    void word_spacing(unsigned int word_spacing) {
        if (word_spacing == word_spacing_)
            return;
        word_spacing_ = word_spacing;
        recalculate();
    }

    void selection_brush(const brush_sptr& selection_brush) {
        if (selection_brush == selbrush_)
            return;
        selbrush_ = selection_brush;
    }

    void writing_brush(const brush_sptr& writing_brush) {
        if (writing_brush == wrtbrush_)
            return;
        wrtbrush_ = writing_brush;
    }

public:
    bool typewrite(wchar_t chr) {
        return typewrite(std::wstring(&chr, 1));
    }

    bool typewrite(std::wstring str) {
        erase();
        if (!multiline_)
            str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
        if (str.empty())
            return false;
        unsigned int count = action_insert(caret_.e, str.c_str(), str.size());
        if (count > 0) {
            caret_.e += count;
            recalculate();
            readapt();
            return true;
        }
        return false;
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
            if (offset < 0)
                action_erase(caret_.e += offset, -offset);
        } else if (offset == 0) {
            return;
        } else if (offset > 0) {
            if (offset > static_cast<long>(content_.size()) - caret_.e)
                offset = static_cast<long>(content_.size()) - caret_.e;
            if (offset != 0)
                action_erase(caret_.e, offset);
        }

        recalculate();
        readapt();
    }

    void move(int caret_e, bool shift = false) {
        int fix_e = caret_e;

        if (caret_e < 0)
            fix_e = 0;
        else if (caret_e > static_cast<int>(content_.size()))
            fix_e = content_.size();

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

        readapt();
    }

    void move_similar(bool negative, bool shift = false) {
        int caret = caret_.e;
        if (negative) {
            if (caret > 0) {
                isx::isx_f isx = isx::get_isx(content_[--caret]);
                while (caret > 0 && isx(content_[caret - 1])) --caret;
            }
        } else {
            int upper_limit = content_.size();
            if (caret < upper_limit) {
                isx::isx_f isx = isx::get_isx(content_[caret++]);
                while (caret < upper_limit && isx(content_[caret])) ++caret;
            }
        }
        if (shift) {
            if (caret_.s < 0)
                caret_.s = caret_.e;
        } else
            caret_.s = -1;
        caret_.e = caret;
    }

    void select(int start, int end) {
        caret_.s = start;
        caret_.e = end;

        if (caret_.s < 0)
            caret_.s = -1;
        if (caret_.e < 0)
            caret_.e = 0;
        else if (caret_.e > static_cast<long>(content_.size()))
            caret_.e = content_.size();
        if (caret_.s == caret_.e)
            caret_.s = -1;
    }

    void select_similar(int caret) {
        int upper_limit = content_.size();
        if (caret < 0) caret = 0;
        if (caret >= upper_limit) caret = upper_limit - 1;
        int start = caret, end = caret + 1;
        isx::isx_f isx = isx::get_isx(content_[caret]);
        if (isx(content_[caret])) {
            while (start > 0 && isx(content_[start - 1])) --start;
            while (end < upper_limit && isx(content_[end])) ++end;
        }
        select(start, end);
    }

    void cancel_select(void) {
        caret_.s = -1;
    }

    unsigned int hittest(point point) const {
        point.y -= margin_.top;
        if (point.y < 0)
            return 0;
        else
        if (point.y >= line_count() * line_height())
            return content_.size();
        else {
            unsigned int index = point.y / line_height();
            std::pair<unsigned int, line> range = line_range(index);
            int x = margin_.left + line_offset(range.second);
            unsigned int begin = range.first;
            unsigned int end   = range.first + range.second.words;

            if (std::get<1>(line_location(index)) == 0)
                x += indent_;
            for (unsigned int i = begin; i < end; ++i) {
                int advance = font_->get_bitmap(content_[i])->advance;
                if (x + advance / 2 > point.x)
                    return i;
                x += advance + word_spacing_;
            }

            return end;
        }
    }

    point caret_point(void) const {
        return caret_point(caret_.e);
    }

    point caret_point(unsigned int caret) const {
        unsigned int index = caret_line(caret);
        std::pair<unsigned int, line> range = line_range(index);
        int x = margin_.left + line_offset(range.second);
        if (std::get<1>(line_location(index)) == 0)
            x += indent_;
        for (unsigned int i = range.first; i < caret; ++i)
            x += font_->get_bitmap(content_[i])->advance + word_spacing_;
        return point(x, index * line_height() + margin_.top);
    }

    unsigned int caret_line(void) const {
        return caret_line(caret_.e);
    }

    unsigned int caret_line(unsigned int caret) const {
        unsigned int offset = 0, index = 0;
        for (const std::vector<line>& lines : paragraphs_) {
            for (const line& line : lines)
                if ((offset += line.words) >= caret) return index;
                else ++index;
            ++offset;
        }
        return index;
    }

    unsigned int caret_paragraph(void) const {
        return caret_paragraph(caret_.e);
    }

    unsigned int caret_paragraph(unsigned int caret) const {
        unsigned int offset = 0;
        for (unsigned int i = 0; i < paragraphs_.size(); ++i)
            if ((offset += paragraph_words(i)) >= caret) return i;
            else ++offset;
        return paragraphs_.size();
    }

    std::tuple<unsigned int, unsigned int, unsigned int>
            caret_location(void) const {
        return caret_location(caret_.e);
    }

    std::tuple<unsigned int, unsigned int, unsigned int>
            caret_location(int caret) const {
        unsigned int paragraph, line;
        for (paragraph = 0; paragraph < paragraphs_.size(); ++paragraph) {
            unsigned int words = paragraph_words(paragraph);
            if (caret - (int)words <= 0) break;
            else caret -= words + 1;
        }
        for (line = 0; line < paragraphs_[paragraph].size(); ++line)
            if (caret - (int)paragraphs_[paragraph][line].words <= 0)
                return std::make_tuple(paragraph, line, caret);
            else
                caret -= paragraphs_[paragraph][line].words;
        return std::make_tuple(paragraphs_.size(), 0, 0);
    }

    double line_height(void) const {
        return font_->height() + line_spacing_;
    }

    unsigned int line_count(void) const {
        return std::accumulate(paragraphs_.begin(), paragraphs_.end(), 0,
                [](unsigned int a, const std::vector<line>& b) -> unsigned int {
                    return a + b.size();
                });
    }

    std::pair<unsigned int, line> line_range(unsigned int index) const {
        unsigned int offset = 0, idx = 0;
        for (const std::vector<line>& lines : paragraphs_) {
            for (const line& line : lines)
                if (idx == index) return std::make_pair(offset, line);
                else offset += line.words, ++idx;
            ++offset;
        }
        return std::make_pair(content_.size(), line {0, 0});
    }

    unsigned int line_paragraph(unsigned int line) const {
        unsigned int offset = 0;
        for (unsigned int i = 0; i < paragraphs_.size(); ++i)
            if ((offset += paragraphs_[i].size()) >= line)
                return i;
        return paragraphs_.size();
    }

    std::tuple<unsigned int, unsigned int> line_location(int index) const {
        for (unsigned int i = 0; i < paragraphs_.size(); ++i)
            if (index - (int)paragraphs_[i].size() < 0)
                return std::make_tuple(i, index);
            else
                index -= paragraphs_[i].size();
        return std::make_tuple(paragraphs_.size(), 0);
    }

    unsigned int paragraph_count(void) const {
        return paragraphs_.size();
    }

    unsigned int paragraph_words(unsigned int index) const {
        return std::accumulate(
                paragraphs_[index].begin(), paragraphs_[index].end(), 0,
                [](unsigned int a, const line& b) -> unsigned int {
                    return a + b.words;
                });
    }

    std::pair<unsigned int, std::vector<line>>
            paragraph_range(unsigned int index) const {
        if (index >= paragraphs_.size())
            return std::make_pair(content_.size(), std::vector<line>());
        unsigned int offset = 0;
        for (unsigned int i = 0; i < index; ++i)
            offset += paragraph_words(i) + 1;
        return std::make_pair(offset, paragraphs_[index]);
    }

    void draw(graphics& graphics) {
        rect drawing_area;
        drawing_area.left   = margin_.left;
        drawing_area.top    = margin_.top;
        drawing_area.right  = view_.width - margin_.right;
        drawing_area.bottom = view_.height - margin_.bottom;
        draw(graphics, drawing_area);
    }

    // TODO: 通过world_.top，可以计算出起始绘制行，从这一行开始绘制
    void draw(graphics& graphics, rect drawing_area, bool first=true) {
        unsigned int sel_end = caret_.upper();
        unsigned int sel_start = caret_.s >= 0 ? caret_.lower() : sel_end;
        unsigned int font_height = line_height();
        unsigned int caret = 0;
        int xx = world_.left + margin_.left, y = world_.top + margin_.top;

        for (const std::vector<line>& lines : paragraphs_) {
            int x = xx + (multiline_ ? indent_ : 0);
            for (const line& line : lines) {
                if (y >= drawing_area.bottom)
                    goto End;
                if (y + (int)font_height > drawing_area.top) {
                    unsigned int line_height = font_height;
                    if (caret + line.words >= sel_end)
                        line_height = font_->emheight();
                    x += line_offset(line);
                    for (unsigned int i = caret; i < caret + line.words; ++i) {
                        if (x >= drawing_area.right)
                            continue;
                        const bitmap* bitmap = font_->get_bitmap(content_[i]);
                        if (x + (int)bitmap->advance > drawing_area.left) {
                            if (first) {
                                if (sel_start <= i && i < sel_end)
                                    graphics.fill_rectangle(x, y,
                                            x + bitmap->advance + word_spacing_,
                                            y + line_height, *selbrush_);
                            } else
                                bitmap->render(graphics, x, y,
                                               drawing_area.left,
                                               drawing_area.top,
                                               drawing_area.right,
                                               drawing_area.bottom, *wrtbrush_);
                        }
                        x += bitmap->advance + word_spacing_;
                    }
                    x = xx;
                }
                caret += line.words;
                y += font_height;
            }
            caret++;
        }

    End:
        if (first)
            draw(graphics, drawing_area, false);
    }

    bool undo(void) {
        if (actidx_ == -1)
            return false;
        const action& action = actions_[actidx_--];
        if (action.type == action::Type::atInsert) {
            content_.erase(action.offset, action.content.size());
            select(-1, action.offset);
        } else if (action.type == action::Type::atErase) {
            content_.insert(action.offset, action.content);
            select(action.offset, action.offset + action.content.size());
        }
        recalculate();
        return true;
    }

    bool redo(void) {
        if (actidx_ >= (int)actions_.size() - 1)
            return false;
        const action& action = actions_[++actidx_];
        if (action.type == action::Type::atInsert) {
            content_.insert(action.offset, action.content);
            select(action.offset, action.offset + action.content.size());
        } else if (action.type == action::Type::atErase) {
            content_.erase(action.offset, action.content.size());
            select(-1, action.offset);
        }
        recalculate();
        return true;
    }

private:
    int line_offset(const line& line) const {
        int width  = algorithm::max(world_.width(), view_.width);
            width -= margin_.left + margin_.right;

        switch (alignment_ & aHorizontal)
        {
        default:
        case aLeft:   return  0;
        case aCenter: return (width - line.width) / 2;
        case aRight:  return  width - line.width;
        }
    }

    void push_line(int words, int width) {
        paragraphs_.back().push_back(line {words, width - (int)word_spacing_});
    }

    void clear_actions(void) {
        actions_.clear();
        actidx_ = actions_.size() - 1;
    }

    void push_action(const action& action) {
        if (actidx_ + 1 < (int)actions_.size())
            actions_.resize(actidx_ + 1);
        actions_.push_back(action);
        actidx_++;
    }

    unsigned int action_insert(unsigned int offset,
                               const wchar_t* str, unsigned int count) {
        if (content_.size() + count > max_length_) {
            int c = (int)max_length_ - (int)content_.size();
            if (c <= 0) return 0;
            else count = c;
        }
        if (actidx_ < 0
            || actions_[actidx_].content.size() > xxdo_limit
            || actions_[actidx_].type != action::Type::atInsert
            || actions_[actidx_].offset + actions_[actidx_].content.size()
                    != offset)
        {
            action action;
            action.type    = action::Type::atInsert;
            action.offset  = offset;
            action.content = std::wstring(str, count);
            push_action(action);
        }
        else
            actions_[actidx_].content.append(str, count);
        content_.insert(offset, str, count);
        return count;
    }

    void action_erase(unsigned int offset, unsigned int count) {
        bool dealed = false;
        if (actidx_ >= 0
            && actions_[actidx_].content.size() < xxdo_limit
            && actions_[actidx_].type == action::Type::atErase) {
            action& act = actions_[actidx_];
            if (offset + count == act.offset) {
                act.offset   = offset;
                act.content  = content_.substr(offset, count) + act.content;
                dealed       = true;
            } else if (act.offset == offset) {
                act.content += content_.substr(offset, count);
                dealed       = true;
            }
        }
        if (!dealed) {
            action action;
            action.type    = action::Type::atErase;
            action.offset  = offset;
            action.content = content_.substr(offset, count);
            push_action(action);
        }
        content_.erase(offset, count);
    }

    void recalculate(void) {
        unsigned int line_start = 0;
        int right = word_wrap_ ? (view_.width - margin_.right) : (~0u >> 1);
        int line_beginning = world_.left + margin_.left;
        int paragraph_beginning = line_beginning + (multiline_ ? indent_ : 0);
        int x = paragraph_beginning;

        world_.right = x;
        paragraphs_.clear();
        paragraphs_.push_back(std::vector<line>());

        for (unsigned int i = 0; i < content_.size(); ++i) {
            if (!multiline_ || content_[i] != '\n') {
                int advance = font_->get_bitmap(content_[i])->advance;
                if (x + advance > right) {
                    if (world_.right < x)
                        world_.right = x;
                    push_line(i - line_start, x - line_beginning);
                    line_start = i;
                    x = line_beginning + advance + word_spacing_;
                }
                else
                    x += advance + word_spacing_;
            } else {
                if (world_.right < x)
                    world_.right = x;
                push_line(i - line_start, x - line_beginning);
                paragraphs_.push_back(std::vector<line>());
                line_start = i + 1;
                x = paragraph_beginning;
            }
        }

        if (world_.right < x)
            world_.right = x;
        push_line(content_.size() - line_start, x - line_beginning);

        world_.right += margin_.right;
        world_.bottom = world_.top + margin_.top
                      + line_count() * line_height() - line_spacing_
                      + margin_.bottom;
    }

    // TODO: 根据所在行的变化，不用重新计算所有文本内容。
    //       但需要考虑更多的变化情况，比如（共8种情况）：
    //          1、单字输入/删除，非换行符
    //          2、单字输入/删除，换行符
    //          3、多字输入/删除，其中不包括换行符 （可使用通用算法）
    //          4、多字输入/删除，其中包括换行符   （可使用通用算法）
    void recalculate(unsigned int caret) {
        ;
    }

    void readapt(void) {
        point offset = world_.left_top();
        if (world_.right < view_.width) {
            offset.x += view_.width - world_.right;
            if (offset.x > 0)
                offset.x = 0;
        }
        if (world_.bottom < view_.height) {
            offset.y += view_.height - world_.bottom;
            if (offset.y > 0)
                offset.y = 0;
        }

        rect mv;
        mv.left   = margin_.left;
        mv.top    = margin_.top;
        mv.right  = view_.width  - margin_.right;
        mv.bottom = view_.height - margin_.bottom - font_->emheight();

        point caret = caret_point() + offset;
             if (caret.x < mv.left)   offset.x += mv.left   - caret.x;
        else if (caret.x > mv.right)  offset.x += mv.right  - caret.x;
             if (caret.y < mv.top)    offset.y += mv.top    - caret.y;
        else if (caret.y > mv.bottom) offset.y += mv.bottom - caret.y;

        this->offset(offset.x, offset.y);

        onCaretPositionChanged();
    }

private:
    std::wstring content_;
    unsigned int max_length_;
    struct {
        long s, e;
    public:
        inline long lower(void) const { return s < e ? s : e; }
        inline long upper(void) const { return s > e ? s : e; }
    } caret_;
    rect world_;
    size view_;
    bool fixed_brush_;
    Margin margin_;
    unsigned int indent_;
    Alignment alignment_;
    bool multiline_, word_wrap_;
    unsigned int line_spacing_, word_spacing_;
    const class font* font_;
    brush_sptr selbrush_, wrtbrush_;
    std::vector<std::vector<line>> paragraphs_;
    int actidx_;
    std::vector<action> actions_;
};

#endif // text_hpp_20151208
