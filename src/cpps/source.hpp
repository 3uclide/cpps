#pragma once

#include "cpps/source-location.hpp"

#include <assert.h>
#include <range/v3/view.hpp>
#include <string>
#include <vector>

namespace cpps {

class Source
{
public:
    class Line
    {
    public:
        enum class Type
        {
            Cpp,
            Cpps,
            Comment,
            Empty,
            Import,
            Preprocessor
        };

    public:
        Line(std::string text, Type type);

        Type getType() const;

        const std::string& getText() const;

        bool empty() const;
        SourceColumn size() const;

    private:
        friend class Source;

    private:
        std::string _text;
        Type _type;
    };

public:
    bool hasCpp() const;
    bool hasCpps() const;

    bool empty() const;
    SourceLine size() const;

    const Line& operator[](SourceLine line) const;

    void add(std::string text, Line::Type type);

    // set previous adjacent comment/empty line type to cpps
    void cppsify();

private:
    std::vector<Line> _lines;

    bool _hasCpp{false};
    bool _hasCpps{false};
};

inline Source::Line::Line(std::string text, Type type)
    : _text(std::move(text))
    , _type(type)
{
}

inline Source::Line::Type Source::Line::getType() const
{
    return _type;
}

inline const std::string& Source::Line::getText() const
{
    return _text;
}

inline bool Source::Line::empty() const
{
    return _text.empty();
}

inline SourceColumn Source::Line::size() const
{
    return static_cast<SourceColumn>(_text.size());
}

inline bool Source::hasCpp() const
{
    return _hasCpp;
}

inline bool Source::hasCpps() const
{
    return _hasCpps;
}

inline bool Source::empty() const
{
    return _lines.empty();
}

inline SourceLine Source::size() const
{
    return static_cast<SourceLine>(_lines.size());
}

inline const Source::Line& Source::operator[](SourceLine line) const
{
    return _lines[line];
}

inline void Source::add(std::string text, Line::Type type)
{
    _lines.emplace_back(std::move(text), type);

    _hasCpp |= type == Line::Type::Cpp || type == Line::Type::Preprocessor;
    _hasCpps |= type == Line::Type::Cpps;
}

inline void Source::cppsify()
{
    assert(_lines.back().getType() == Line::Type::Cpps);

    for (Line& line : ranges::views::reverse(_lines) | ranges::views::drop(1))
    {
        const Line::Type lineType = line.getType();

        if (lineType != Line::Type::Comment && lineType != Line::Type::Empty)
        {
            break;
        }

        line._type = Line::Type::Cpps;
    }
}

} // namespace cpps
