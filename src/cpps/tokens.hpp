#pragma once

#include "cpps/comment.hpp"
#include "cpps/token.hpp"

#include <limits>
#include <span>
#include <utility>
#include <vector>

namespace CPPS {

class Tokens
{
public:
    constexpr Tokens() = default;

    constexpr explicit Tokens(std::vector<Token>&& tokens, std::vector<Comment>&& comments);

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

    [[nodiscard]] constexpr std::span<const Comment> comments() const;

private:
    static constexpr std::size_t InvalidIndex{std::numeric_limits<std::size_t>::max()};

    std::vector<Token> _tokens;
    std::vector<std::size_t> _sparse;
    std::vector<std::span<Token>> _dense;

    std::vector<Comment> _comments;
};

constexpr Tokens::Tokens(std::vector<Token>&& tokens, std::vector<Comment>&& comments)
    : _tokens(std::move(tokens))
    , _comments(std::move(comments))
{
    _sparse.resize(_tokens.empty() ? 0 : _tokens.back().location.line + 1u, InvalidIndex);

    auto beginIt = _tokens.begin();
    auto it = _tokens.begin();

    const auto endIt = _tokens.end();

    for (; it != endIt; ++it)
    {
        const Token& beginToken = *beginIt;
        const Token& token = *it;

        if (token.location.line != beginToken.location.line)
        {
            _sparse[beginToken.location.line] = _dense.size();

            _dense.emplace_back(beginIt, it);

            beginIt = it;
        }
    }

    if (beginIt != endIt)
    {
        assert(_sparse[beginIt->location.line] == InvalidIndex);

        _sparse[beginIt->location.line] = _dense.size();

        _dense.emplace_back(beginIt, endIt);
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

constexpr std::span<const Comment> Tokens::comments() const
{
    return _comments;
}

} // namespace CPPS
