//
// Created by Micah on 12/17/2023.
//

#ifndef BESTBUYTRACKER_TOKENIZER_HPP
#define BESTBUYTRACKER_TOKENIZER_HPP

#include "../gutils/Array.hpp"
#include "../gutils/Result.hpp"
using gutils::Result, gutils::Array;

#include "Token.hpp"

namespace tracker {
    struct Tokenizer {
    private:
        constexpr Token::Type getTokenType(const string& source) {
            if (source.empty() || source == "\0") {
                return Token::Type::End;
            }

            if (source == ",") {
                return Token::Type::Comma;
            }

            if (source == "-") {
                return Token::Type::Hyphen;
            }

            if (source == "$") {
                return Token::Type::DollarSign;
            }

            if (isdigit(source[0])) {
                return Token::Type::Number;
            }

            if (source == "{") {
                return Token::Type::LBrace;
            }

            if (source == "}") {
                return Token::Type::RBrace;
            }

            if (source == "(") {
                return Token::Type::LParen;
            }

            if (source == ")") {
                return Token::Type::RParen;
            }

            if (source == ":") {
                return Token::Type::Colon;
            }

            if (source == "/") {
                return Token::Type::FSlash;
            }

            if (isalpha(source[0])) {
                return Token::Type::Word;
            }

            return Token::Type::Unknown;
        }

    public:
        typedef vector<Token> TokenList;
        typedef vector<string> ErrorList;

        [[nodiscard]] Result<Array<Token>, Array<string>> tokenize(const string& source) {
            string sanitizedSource;

            for (const char c: source) {
                if (isspace(c) || isblank(c))
                    continue;

                sanitizedSource.push_back(c);
            }

            const size_t length = sanitizedSource.length();
            TokenList tokens;
            ErrorList errors;

            const auto emplaceToken = [&](const Token::Type type, const string& value) {
                tokens.emplace_back(value, type);
            };

            int index = 0;

            while (index != length) {
                const Token::Type type = getTokenType(string{sanitizedSource[index]});
                string value;

                switch ((uint8_t) type) {
                    case Token::Type::Comma:
                        value = ",";
                        break;
                    case Token::Type::Hyphen:
                        value = "-";
                        break;
                    case Token::Type::DollarSign:
                        value = "$";
                        break;

                    case Token::Type::Number:
                        while (index != length && isdigit(sanitizedSource[index])) {
                            value.push_back(sanitizedSource[index++]);
                        }

                        emplaceToken(type, value);
                        continue;

                    case Token::Type::LBrace:
                        value = "{";
                        break;
                    case Token::Type::RBrace:
                        value = "}";
                        break;
                    case Token::Type::LParen:
                        value = "(";
                        break;
                    case Token::Type::RParen:
                        value = ")";
                        break;
                    case Token::Type::Colon:
                        value = ":";
                        break;
                    case Token::Type::FSlash:
                        value = "/";
                        break;
                    case Token::Type::Word:
                        while (index != length && isalpha(sanitizedSource[index])) {
                            value.push_back(sanitizedSource[index++]);
                        }

                        emplaceToken(type, value);
                        continue;

                    default:
                        errors.push_back(" Unknown token: " + string{sanitizedSource[index]});
                        break;
                }

                index++;

                emplaceToken(type, value);
            }

            emplaceToken(Token::Type::End, "\0");

            if (!errors.empty()) {
                return Result<Array<Token>, Array<string>>::errOf(errors);
            }

            return Result<Array<Token>, Array<string>>::okOf(tokens);
        }
    };
}

#endif //BESTBUYTRACKER_TOKENIZER_HPP