#ifndef text_hpp_20151208
#define text_hpp_20151208

#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <takisy/core/handler.h>
#include <takisy/cgl/basic/point.h>
#include <takisy/cgl/basic/size.h>
#include <takisy/cgl/basic/rect.h>
#include <takisy/cgl/font/font.h>
#include <takisy/cgl/font/tfont_simple.h>
#include <takisy/cgl/brush.h>
#include <takisy/cgl/graphics.h>
#include <takisy/gui/basic/define.h>

class text {
    static const unsigned int xxdo_limit = 16;

    struct action {
        enum { atInsert, atErase, } type;
        unsigned int offset;
        std::wstring content;
    };

    struct isx {
        typedef int (*isx_f)(wint_t);
        static int isvname(wint_t c)
            { return ::iswalpha(c) || ::iswdigit(c) || c == '_'; }
        static int isfalse(wint_t) { return false; }
        static isx_f get_isx(int c) {
            if (isvname(c)) return isvname;
            else if (::iswblank(c)) return ::iswblank;
            else if (::iswpunct(c)) return ::iswpunct;
            else return isfalse;
        }
    };

    class caret {
    public:
        DECLARE_HANDLER(onCaretChanged);
        DECLARE_HANDLER(onSelected, bool /* yes */);
        DECLARE_HANDLER(onSelectionChanged);

    public:
        caret(long s, long e) : s_(s), e_(e) {
            onSelected(
                [this](caret* self, bool) {
                    onSelectionChangedHandle();
                });
        }

    public:
        long s             (void) const { return s_; }
        long e             (void) const { return e_; }
        long l /* lower */ (void) const { return std::min(s_, e_); }
        long u /* upper */ (void) const { return std::max(s_, e_); }

    public:
        void s(long s) {
            if (s_ != s) {
                long olds = s_; s_ = s;
                if (olds == -1 || olds == e_) {
                    if (s_ != -1 && s_ != e_)
                        onSelectedHandle(true);
                } else {
                    if (s_ == -1 || s_ == e_)
                        onSelectedHandle(false);
                    else
                        onSelectionChangedHandle();
                }
            }
        }

        void e(long e) {
            if (e_ != e) {
                long olde = e_; e_ = e;
                onCaretChangedHandle();
                if (s_ == olde)
                    onSelectedHandle(true);
                else if (s_ == e_)
                    onSelectedHandle(false);
                else if (s_ != -1)
                    onSelectionChangedHandle();
            }
        }

    private:
        long s_, e_;
    };

public:
    DECLARE_HANDLER(onCaretPointChanged);
    DECLARE_HANDLER(onCaretChanged);
    DECLARE_HANDLER(onTypewriting, std::wstring& /* str */);
    DECLARE_HANDLER(onErased, const std::wstring& /* str */);
    DECLARE_HANDLER(onContentChanged);
    DECLARE_HANDLER(onSelectionChanged);
    DECLARE_HANDLER(onCopyAvailable, bool /* yes */);
    DECLARE_HANDLER(onUndoAvailable, bool /* yes */);
    DECLARE_HANDLER(onRedoAvailable, bool /* yes */);

public:
    struct line {
        unsigned int words, width;
    };

    struct line_info {
        unsigned int p, l;
        struct range { unsigned int offset, words; } prange, lrange;
    };

public:
    text(void)
        : text(std::wstring())
    {}

    text(const std::wstring& content)
        : caret_(-1, content.size())
        , content_(content), max_length_(~0u)
        , world_(0, 0), view_(0, 0, ~0u >> 1, ~0u >> 1), margin_({0})
        , indent_(0), alignment_(aLeftTop)
        , multiline_(true), word_wrap_(true)
        , line_spacing_(0), word_spacing_(0)
        , font_(tfont_simple::get()), actptr_(0)
    {
        caret_.onCaretChanged(
            [this](class caret* self) {
                onCaretChangedHandle();
            });
        caret_.onSelected(
            [this](class caret* self, bool yes) {
                onCopyAvailableHandle(yes);
            });
        caret_.onSelectionChanged(
            [this](class caret* self) {
                onSelectionChangedHandle();
            });

        recalculate();
    }

    text(const text& text) : caret_(-1, 0) {
        operator=(text);
    }

    text& operator=(const text& text) {
        if (this != &text) {
            caret_        = text.caret_;
            content_      = text.content_;
            max_length_   = text.max_length_;
            world_        = text.world_;
            view_         = text.view_;
            margin_       = text.margin_;
            indent_       = text.indent_;
            alignment_    = text.alignment_;
            multiline_    = text.multiline_;
            word_wrap_    = text.word_wrap_;
            line_spacing_ = text.line_spacing_;
            word_spacing_ = text.word_spacing_;
            font_         = text.font_;
            paragraphs_   = text.paragraphs_;
            actptr_       = text.actptr_;
            actions_      = text.actions_;
        }

        return *this;
    }

public:
    const std::wstring& content(void) const {
        return content_;
    }

    bool selected(void) const {
        return caret_.s() >= 0 && caret_.s() != caret_.e();
    }

    std::wstring selected_content(void) const {
        if (selected())
            return content_.substr(caret_.l(), caret_.u() - caret_.l());
        return std::wstring();
    }

    std::pair<int, int> selection(void) const {
        if (selected())
            return std::pair<int, int>(caret_.l(), caret_.u());
        else
            return std::pair<int, int>(caret_.e(), caret_.e());
    }

    unsigned int max_length(void) const {
        return max_length_;
    }

    unsigned int caret(void) const {
        return caret_.e();
    }

    size world(void) const {
        return world_;
    }

    unsigned int width(void) const {
        return world_.width;
    }

    unsigned int height(void) const {
        return world_.height;
    }

    point offset(void) const {
        return view_.left_top();
    }

    size view(void) const {
        return view_.size();
    }

    Margin margin(void) const {
        return margin_;
    }

    unsigned int indent(void) const {
        return indent_;
    }

    unsigned int alignment(void) const {
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

public:
    void content(const std::wstring& content) {
        clear_actions();
        content_ = content;
        caret_.s(-1);
        caret_.e(content_.size());
        recalculate();
        readapt();
        onContentChangedHandle();
    }

    void max_length(unsigned int max_length) {
        max_length_ = max_length;
    }

    void offset(int left, int top) {
        if (word_wrap_)
            left = 0;
        view_ = view_.move(left, top);
        onCaretPointChangedHandle();
    }

    void offset_left(int left) {
        offset(left, view_.top);
    }

    void offset_top(int top) {
        offset(view_.left, top);
    }

    void view(unsigned int width, unsigned int height) {
        view_.height(height);
        if (view_.width() != (int)width) {
            view_.width(width);
            if (word_wrap_)
                recalculate();
        }
    }

    void margin(int ltrb) {
        margin(ltrb, ltrb, ltrb, ltrb);
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

    void alignment(unsigned int alignment) {
        alignment_ = alignment;
    }

    void font(const class font* font) {
        if (!font)
            font = tfont_simple::get();
        if (font_ == font)
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

public:
    bool typewrite(wchar_t chr) {
        return typewrite(std::wstring(&chr, 1));
    }

    bool typewrite(std::wstring str) {
        onTypewritingHandle(str);
        if (!multiline_)
            str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
        if (str.empty())
            return false;
        return action_insert(str.c_str(), str.size());
    }

    void erase(int offset = 0) {
        if (caret_.s() == caret_.e())
            caret_.s(-1);
        if (caret_.s() >= 0) {
            long offset = caret_.l(), count = caret_.u() - caret_.l();
            caret_.s(-1);
            action_erase(offset, count);
        } else if (offset < 0) {
            if (offset < -caret_.e())
                offset = -caret_.e();
            if (offset < 0)
                action_erase(caret_.e() + offset, -offset);
        } else if (offset > 0) {
            if (offset > static_cast<long>(content_.size()) - caret_.e())
                offset = static_cast<long>(content_.size()) - caret_.e();
            if (offset != 0)
                action_erase(caret_.e(), offset);
        }
    }

    void move(int caret, bool shift = false) {
        int fixcaret = caret;
        if (caret < 0)
            fixcaret = 0;
        else if (caret > static_cast<int>(content_.size()))
            fixcaret = content_.size();
        if (caret_.s() >= 0) {
            if (shift)
                caret_.e(fixcaret);
            else {
                caret_.e(caret > caret_.e() ? caret_.u() : caret_.l());
                caret_.s(-1);
            }
        } else {
            if (shift)
                caret_.s(caret_.e());
            caret_.e(fixcaret);
        }
        readapt();
    }

    void move_similar(bool negative, bool shift = false) {
        int caret = caret_.e();
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
            if (caret_.s() < 0)
                caret_.s(caret_.e());
        } else
            caret_.s(-1);
        caret_.e(caret);
        readapt();
    }

    void select(int start, int end) {
        if (start < 0)
            start = -1;
        if (end < 0)
            end = 0;
        else if (end > (long)content_.size())
            end = content_.size();
        if (start == end)
            start = -1;
        caret_.e(end);
        caret_.s(start);
        readapt();
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
        caret_.s(-1);
    }

    unsigned int hittest(point point) const {
        point.y -= margin_.top;
        if (point.y < 0)
            return 0;
        else
        if (point.y >= line_count() * line_height())
            return content_.size();
        else {
            struct line_info li = line_info(point.y / line_height());
            int x = margin_.left + line_offset(paragraphs_[li.p][li.l]);
            unsigned int begin = li.lrange.offset;
            unsigned int end   = li.lrange.offset + li.lrange.words;

            if (li.l == 0)
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

    unsigned int caret_line(void) const {
        return caret_line(caret_.e());
    }

    unsigned int caret_line(unsigned int caret) const {
        unsigned int index = 0;
        for (const std::vector<line>& lines : paragraphs_) {
            for (const line& line : lines) {
                if (caret <= line.words) return index;
                else caret -= line.words, ++index;
            }
            --caret;
        }
        return index;
    }

    point caret_point(void) const {
        return caret_point(caret_.e());
    }

    point caret_point(unsigned int caret) const {
        unsigned int line_index = caret_line(caret);
        struct line_info li = line_info(line_index);
        int x = margin_.left + line_offset(paragraphs_[li.p][li.l]);
        if (li.l == 0)
            x += indent_;
        for (unsigned int i = li.lrange.offset; i < caret; ++i)
            x += font_->get_bitmap(content_[i])->advance + word_spacing_;
        return point(x, line_index * line_height() + margin_.top);
    }

    bool outside_caret(void) const {
        point cp = caret_point();
        return view_.outside(cp.x, cp.y)
            && view_.outside(cp.x, cp.y + font_->height());
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

    unsigned int paragraph_words(unsigned int index) const {
        return std::accumulate(
                paragraphs_[index].begin(), paragraphs_[index].end(), 0,
                [](unsigned int a, const line& b) -> unsigned int {
                    return a + b.words;
                });
    }

    struct line_info line_info(unsigned int line) const {
        struct line_info li = {0};
        for (li.p = 0; li.p < paragraphs_.size(); ++li.p) {
            li.prange.words = paragraph_words(li.p);
            if (line < paragraphs_[li.p].size()) {
                for (unsigned int i = 0; i < line; ++i)
                    li.lrange.offset += paragraphs_[li.p][i].words;
                li.lrange.words = paragraphs_[li.p][line].words;
                li.l = line;
                return li;
            } else
                line -= paragraphs_[li.p].size();
            li.prange.offset = li.lrange.offset += li.prange.words + 1;
        }
        return li;
    }

    template <bool First=true>
    void draw(graphics& graphics, rect area, brush_sptr selbr, brush& wrtbr) {
        if (First && !selbr)
            return draw<false>(graphics, area, nullptr, wrtbr);

        unsigned int sel_end = caret_.u();
        unsigned int sel_start = caret_.s() >= 0 ? caret_.l() : sel_end;
        unsigned int caret = 0;
        int line_height = this->line_height();
        int xx = margin_.left - view_.left, y = margin_.top - view_.top;

        for (const std::vector<line>& lines : paragraphs_) {
            int x = xx + indent_;
            for (const line& line : lines) {
                if (y >= area.bottom)
                    goto END;
                if (y + (int)line_height > area.top) {
                    unsigned int lhoremheight = line_height;
                    if (caret + line.words >= sel_end)
                        lhoremheight = font_->emheight();
                    x += line_offset(line);
                    for (unsigned int i = caret; i < caret + line.words; ++i) {
                        if (x >= area.right)
                            continue;
                        const bitmap* bitmap = font_->get_bitmap(content_[i]);
                        if (x + (int)bitmap->advance > area.left) {
                            if (First) {
                                if (sel_start <= i && i < sel_end)
                                    graphics.fill_rectangle(x, y,
                                            x + bitmap->advance + word_spacing_,
                                            y + lhoremheight, *selbr);
                            } else
                                bitmap->render(graphics, x, y,
                                               area.left,
                                               area.top,
                                               area.right,
                                               area.bottom, wrtbr);
                        }
                        x += bitmap->advance + word_spacing_;
                    }
                }
                x = xx, y += line_height;
                caret += line.words;
            }
            if (First && sel_start <= caret && caret < sel_end) {
                int x = xx + lines.back().width, yy = y - line_height;
                if (lines.size() == 1)
                    x += indent_;
                graphics.fill_rectangle(x, yy,
                                        x  + (int)font_->emheight() / 2,
                                        yy + line_height, *selbr);
            }
            caret++;
        }

    END:
        if (First)
            draw<false>(graphics, area, selbr, wrtbr);
    }

    bool undo(void) {
        if (actptr_ == 0)
            return false;
        const action& action = actions_[--actptr_];
        if (action.type == action::atInsert) {
            content_.erase(action.offset, action.content.size());
            caret_.s(-1); caret_.e(action.offset);
            recalculate_erase(action.offset, action.content.size());
            onContentChangedHandle();
        } else if (action.type == action::atErase) {
            content_.insert(action.offset, action.content);
            caret_.s(action.offset);
            caret_.e(action.offset + action.content.size());
            recalculate_typewrite(action.offset, action.content.size());
            onContentChangedHandle();
        }
        if (actptr_ == 0)
            onUndoAvailableHandle(false);
        if (actptr_ == actions_.size() - 1)
            onRedoAvailableHandle(true);
        return true;
    }

    bool redo(void) {
        if (actptr_ == actions_.size())
            return false;
        const action& action = actions_[actptr_++];
        if (action.type == action::atInsert) {
            content_.insert(action.offset, action.content);
            caret_.s(action.offset);
            caret_.e(action.offset + action.content.size());
            recalculate_typewrite(action.offset, action.content.size());
            onContentChangedHandle();
        } else if (action.type == action::atErase) {
            content_.erase(action.offset, action.content.size());
            caret_.s(-1); caret_.e(action.offset);
            recalculate_erase(action.offset, action.content.size());
            onContentChangedHandle();
        }
        if (actptr_ == actions_.size())
            onRedoAvailableHandle(false);
        if (actptr_ == 1)
            onUndoAvailableHandle(true);
        return true;
    }

private:
    int margin_hsum(void) const {
        return margin_.left + margin_.right;
    }

    int line_offset(const line& line) const {
        int width = view_.width() - margin_hsum();

        switch (alignment_ & aHorizontal)
        {
        default:
        case aLeft:   return  0;
        case aCenter: return (width - (int)line.width) / 2;
        case aRight:  return  width - (int)line.width;
        }
    }

    void push_line(std::vector<line>& paragraph,
                   unsigned int words, unsigned int width) {
        if (words != 0)
            width -= word_spacing_;
        paragraph.push_back(line {words, width});
    }

    void clear_actions(void) {
        bool undo = actptr_ > 0, redo = actptr_ < actions_.size();
        actions_.clear(), actptr_ = 0;
        if (undo) onUndoAvailableHandle(false);
        if (redo) onRedoAvailableHandle(false);
    }

    void push_action(const action& action) {
        actions_.insert(actions_.begin() + actptr_++, action);
        if (actptr_ == 1)
            onUndoAvailableHandle(true);
        if (actptr_ != actions_.size()) {
            actions_.resize(actptr_);
            onRedoAvailableHandle(false);
        }
    }

    bool action_insert(const wchar_t* str, unsigned int count) {
        erase();
        if (content_.size() + count > max_length_) {
            int c = (int)max_length_ - (int)content_.size();
            if (c <= 0) return false;
            else count = c;
        }
        int index = actptr_ - 1;
        if (index < 0
            || actions_[index].content.size() > xxdo_limit
            || actions_[index].type != action::atInsert
            || actions_[index].offset + actions_[index].content.size()
                    != static_cast<unsigned long>(caret_.e()))
        {
            action action;
            action.type    = action::atInsert;
            action.offset  = caret_.e();
            action.content = std::wstring(str, count);
            push_action(action);
        }
        else
            actions_[index].content.append(str, count);
        content_.insert(caret_.e(), std::wstring(str, count));
        caret_.e(caret_.e() + count);
        recalculate_typewrite(caret_.e() - count, count);
        readapt();
        onContentChangedHandle();
        return true;
    }

    void action_erase(unsigned int offset, unsigned int count) {
        bool dealed = false;
        std::wstring str = content_.substr(offset, count);
        int index = actptr_ - 1;
        if (index >= 0
            && actions_[index].type == action::atErase
            && actions_[index].content.size() < xxdo_limit) {
            action& act = actions_[index];
            if (offset + count == act.offset) {
                act.offset   = offset;
                act.content  = str + act.content;
                dealed       = true;
            } else if (act.offset == offset) {
                act.content += str;
                dealed       = true;
            }
        }
        if (!dealed) {
            action action;
            action.type    = action::atErase;
            action.offset  = offset;
            action.content = str;
            push_action(action);
        }
        content_.erase(offset, count);
        caret_.e(offset);
        recalculate_erase(offset, str.size());
        readapt();
        onContentChangedHandle();
        onErasedHandle(str);
    }

    void calculate(std::vector<std::vector<line>>& paragraphs,
                   unsigned int start,
                   unsigned int stop) {
        int right = word_wrap_ ? (view_.right - margin_hsum()) : (~0u >> 1);
        int x     = indent_;

        paragraphs.clear();
        paragraphs.push_back(std::vector<line>());

        for (unsigned int i = start ; i < stop; ++i) {
            if (content_[i] != '\n' || !multiline_) {
                int advance = font_->get_bitmap(content_[i])->advance;
                if (x + advance > right) {
                    push_line(paragraphs.back(), i - start, x);
                    start = i;
                    x = advance + word_spacing_;
                } else
                    x += advance + word_spacing_;
            } else {
                push_line(paragraphs.back(), i - start, x);
                paragraphs.push_back(std::vector<line>());
                start = i + 1;
                x = indent_;
            }
        }
        push_line(paragraphs.back(), stop - start, x);
    }

    void refresh_world_size(void) {
        if (word_wrap_)
            world_.width = view_.width();
        else {
            unsigned int max_width = 0;
            for (std::vector<line>& lines : paragraphs_) {
                for (line& line : lines) {
                    if (max_width < line.width)
                        max_width = line.width;
                }
            }
            world_.width = max_width + margin_hsum();
        }
        unsigned int
            count = line_count(),
            height = margin_.top + count * line_height() + margin_.bottom;
        if (count != 0)
            height -= line_height() - font_->emheight();
        world_.height = height;
    }

    void recalculate(void) {
        calculate(paragraphs_, 0, content_.size());
        refresh_world_size();
    }

    void recalculate_typewrite(unsigned int offset, unsigned int count) {
        struct line_info li = line_info(caret_line(offset));
        std::vector<std::vector<line>> paragraphs;
        calculate(paragraphs, li.prange.offset,
                  li.prange.offset + li.prange.words + count);
        paragraphs_.erase (paragraphs_.begin() + li.p);
        paragraphs_.insert(paragraphs_.begin() + li.p,
                           paragraphs .begin(), paragraphs.end());
        refresh_world_size();
    }

    void recalculate_erase(unsigned int offset, unsigned int count) {
        struct line_info lis = line_info(caret_line(offset));
        struct line_info lie = line_info(caret_line(offset + count));
        std::vector<std::vector<line>> paragraphs;
        calculate(paragraphs, lis.prange.offset,
                  lie.prange.offset + lie.prange.words - count);
        paragraphs_.erase (paragraphs_.begin() + lis.p,
                           paragraphs_.begin() + lie.p + 1);
        paragraphs_.insert(paragraphs_.begin() + lis.p,
                           paragraphs .begin(), paragraphs.end());
        refresh_world_size();
    }

    void readapt() {
        point cp = caret_point(), cpoffset(0, 0);
        rect  cv = view_;

        cv.left   += margin_.left;
        cv.top    += margin_.top;
        cv.right  -= margin_.right;
        cv.bottom -= margin_.bottom + font_->emheight();

        if (cp.x < cv.left)   cpoffset.x = cp.x - cv.left;  else
        if (cp.x > cv.right)  cpoffset.x = cp.x - cv.right;
        if (cp.y < cv.top)    cpoffset.y = cp.y - cv.top;   else
        if (cp.y > cv.bottom) cpoffset.y = cp.y - cv.bottom;

        offset(view_.left + cpoffset.x, view_.top + cpoffset.y);
    }

private:
    class caret caret_;
    std::wstring content_;
    unsigned int max_length_;
    size world_;
    rect view_;
    Margin margin_;
    unsigned int indent_;
    unsigned int alignment_;
    bool multiline_, word_wrap_;
    unsigned int line_spacing_, word_spacing_;
    const class font* font_;
    std::vector<std::vector<line>> paragraphs_;
    unsigned int actptr_;
    std::vector<action> actions_;
};

#endif // text_hpp_20151208
