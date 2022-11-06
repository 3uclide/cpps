#pragma once

#include "cpps/token.hpp"

#include <span>
#include <vector>

namespace CPPS {

class Tokens
{
public:
    using ConstIterator = std::vector<Token>::const_iterator;

public:
    constexpr Tokens() = default;
    constexpr Tokens(std::vector<Token>&& tokens);

    constexpr Tokens(Tokens&&) = default;
    constexpr Tokens& operator=(Tokens&&) = default;

    constexpr Tokens(const Tokens&) = delete;
    constexpr Tokens& operator=(const Tokens&) = delete;

    [[nodiscard]] constexpr const Token& operator[](std::size_t index) const;

    [[nodiscard]] constexpr bool empty() const;
    [[nodiscard]] constexpr std::size_t size() const;

    [[nodiscard]] constexpr std::size_t getLinesSize() const;
    [[nodiscard]] constexpr std::span<Token> getLine(std::size_t index) const;

    [[nodiscard]] constexpr ConstIterator begin() const;
    [[nodiscard]] constexpr ConstIterator end() const;

private:
    static constexpr std::size_t InvalidIndex{std::numeric_limits<std::size_t>::max()};

    std::vector<Token> _tokens;

    std::vector<std::size_t> _sparse;
    std::vector<std::span<Token>> _dense;
};

constexpr Tokens::Tokens(std::vector<Token>&& tokens)
    : _tokens(std::move(tokens))
{
    _sparse.resize(_tokens.empty() ? 0 : _tokens.back().location.line + 1u, InvalidIndex);

    auto startIt = _tokens.begin();
    auto it = _tokens.begin();

    const auto end = _tokens.end();

    for (; it != end; ++it)
    {
        const Token& startToken = *startIt;
        const Token& token = *it;

        if (token.location.line != startToken.location.line)
        {
            _sparse[startToken.location.line] = _dense.size();

            _dense.emplace_back(startIt, it);

            startIt = it;
        }
    }

    if (startIt != end)
    {
        assert(_sparse[startIt->location.line] == InvalidIndex);

        _sparse[startIt->location.line] = _dense.size();

        _dense.emplace_back(startIt, end);
    }
}

constexpr const Token& Tokens::operator[](std::size_t index) const
{
    return _tokens[index];
}
constexpr bool Tokens::empty() const
{
    return _tokens.empty();
}

constexpr std::size_t Tokens::size() const
{
    return _tokens.size();
}

constexpr std::size_t Tokens::getLinesSize() const
{
    return _sparse.size();
}

constexpr std::span<Token> Tokens::getLine(std::size_t index) const
{
    assert(index < _sparse.size());

    return _sparse[index] != InvalidIndex ? _dense[_sparse[index]] : std::span<Token>{};
}

constexpr Tokens::ConstIterator Tokens::begin() const
{
    return _tokens.begin();
}

constexpr Tokens::ConstIterator Tokens::end() const
{
    return _tokens.end();
}

} // namespace CPPS
