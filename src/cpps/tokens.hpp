#pragma once

#include "cpps/token.hpp"

#include <span>
#include <vector>

namespace CPPS {

class Tokens
{
public:
    constexpr Tokens() = default;
    constexpr Tokens(std::vector<Token>&& tokens);

    constexpr Tokens(Tokens&&) = default;
    constexpr Tokens& operator=(Tokens&&) = default;

    constexpr Tokens(const Tokens&) = delete;
    constexpr Tokens& operator=(const Tokens&) = delete;

    [[nodiscard]] constexpr std::size_t lines() const;

    [[nodiscard]] constexpr bool empty() const;
    [[nodiscard]] constexpr std::size_t size() const;
    [[nodiscard]] constexpr const Token& at(std::size_t index) const;
    [[nodiscard]] constexpr auto begin() const;
    [[nodiscard]] constexpr auto end() const;

    [[nodiscard]] constexpr bool exists(std::size_t lineIndex) const;
    [[nodiscard]] constexpr std::size_t size(std::size_t lineIndex) const;
    [[nodiscard]] constexpr std::span<Token> on(std::size_t lineIndex) const;
    [[nodiscard]] constexpr const Token& at(std::size_t lineIndex, std::size_t index) const;

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

constexpr std::size_t Tokens::lines() const
{
    return _sparse.size();
}

constexpr bool Tokens::empty() const
{
    return _tokens.empty();
}

constexpr std::size_t Tokens::size() const
{
    return _tokens.size();
}

constexpr const Token& Tokens::at(std::size_t index) const
{
    assert(index < _tokens.size());
    return _tokens[index];
}

constexpr auto Tokens::begin() const
{
    return _tokens.begin();
}

constexpr auto Tokens::end() const
{
    return _tokens.end();
}

constexpr bool Tokens::exists(std::size_t lineIndex) const
{
    return lineIndex < _sparse.size() && _sparse[lineIndex] != InvalidIndex;
}

constexpr std::size_t Tokens::size(std::size_t lineIndex) const
{
    return on(lineIndex).size();
}

constexpr std::span<Token> Tokens::on(std::size_t lineIndex) const
{
    assert(lineIndex < _sparse.size());
    return _sparse[lineIndex] != InvalidIndex ? _dense[_sparse[lineIndex]] : std::span<Token>{};
}

constexpr const Token& Tokens::at(std::size_t lineIndex, std::size_t index) const
{
    const std::span span = on(lineIndex);
    assert(index < span.size());
    return span[index];
}

} // namespace CPPS
