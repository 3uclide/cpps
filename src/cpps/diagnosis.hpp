#pragma once

#include "cpps/source-location.hpp"

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace cpps {

class Diagnosis
{
public:
    struct Entry
    {
        std::string message;
        std::optional<std::string> fixMessage;
        std::optional<SourceLocation> location;
    };

    constexpr Diagnosis() = default;

    constexpr void error(std::string message);
    constexpr void error(std::string message, SourceLine line);
    constexpr void error(std::string message, SourceLocation location);
    constexpr void error(std::string message, std::string fixMessage);
    constexpr void error(std::string message, std::string fixMessage, SourceLine line);
    constexpr void error(std::string message, std::string fixMessage, SourceLocation location);

    constexpr void warning(std::string message);
    constexpr void warning(std::string message, SourceLine line);
    constexpr void warning(std::string message, SourceLocation location);
    constexpr void warning(std::string message, std::string fixMessage);
    constexpr void warning(std::string message, std::string fixMessage, SourceLine line);
    constexpr void warning(std::string message, std::string fixMessage, SourceLocation location);

    [[nodiscard]] constexpr std::span<const Entry> getErrors() const;
    [[nodiscard]] constexpr std::span<const Entry> getWarnings() const;

private:
    std::vector<Entry> _errors;
    std::vector<Entry> _warnings;
};

constexpr void Diagnosis::error(std::string message)
{
    _errors.push_back(Entry{.message = std::move(message), .fixMessage = std::nullopt, .location = std::nullopt});
}

constexpr void Diagnosis::error(std::string message, SourceLine line)
{
    _errors.push_back(Entry{.message = std::move(message), .fixMessage = std::nullopt, .location = SourceLocation{.line = line, .column = InvalidSourceColumn}});
}

constexpr void Diagnosis::error(std::string message, SourceLocation location)
{
    _errors.push_back(Entry{.message = std::move(message), .fixMessage = std::nullopt, .location = location});
}

constexpr void Diagnosis::error(std::string message, std::string fixMessage)
{
    _errors.push_back(Entry{.message = std::move(message), .fixMessage = std::move(fixMessage), .location = std::nullopt});
}

constexpr void Diagnosis::error(std::string message, std::string fixMessage, SourceLine line)
{
    _errors.push_back(Entry{.message = std::move(message), .fixMessage = std::move(fixMessage), .location = SourceLocation{.line = line, .column = InvalidSourceColumn}});
}

constexpr void Diagnosis::error(std::string message, std::string fixMessage, SourceLocation location)
{
    _errors.push_back(Entry{.message = std::move(message), .fixMessage = std::move(fixMessage), .location = location});
}

constexpr void Diagnosis::warning(std::string message)
{
    _warnings.push_back(Entry{.message = std::move(message), .fixMessage = std::nullopt, .location = std::nullopt});
}

constexpr void Diagnosis::warning(std::string message, SourceLine line)
{
    _warnings.push_back(Entry{.message = std::move(message), .fixMessage = std::nullopt, .location = SourceLocation{.line = line, .column = InvalidSourceColumn}});
}

constexpr void Diagnosis::warning(std::string message, SourceLocation location)
{
    _warnings.push_back(Entry{.message = std::move(message), .fixMessage = std::nullopt, .location = location});
}

constexpr void Diagnosis::warning(std::string message, std::string fixMessage)
{
    _warnings.push_back(Entry{.message = std::move(message), .fixMessage = std::move(fixMessage), .location = std::nullopt});
}

constexpr void Diagnosis::warning(std::string message, std::string fixMessage, SourceLine line)
{
    _warnings.push_back(Entry{.message = std::move(message), .fixMessage = std::move(fixMessage), .location = SourceLocation{.line = line, .column = InvalidSourceColumn}});
}

constexpr void Diagnosis::warning(std::string message, std::string fixMessage, SourceLocation location)
{
    _warnings.push_back(Entry{.message = std::move(message), .fixMessage = std::move(fixMessage), .location = location});
}

constexpr std::span<const Diagnosis::Entry> Diagnosis::getErrors() const
{
    return _errors;
}

constexpr std::span<const Diagnosis::Entry> Diagnosis::getWarnings() const
{
    return _warnings;
}

} // namespace cpps
