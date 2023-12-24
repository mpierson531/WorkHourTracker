//
// Created by Micah on 12/15/2023.
//

#ifndef BESTBUYTRACKER_TOKEN_HPP
#define BESTBUYTRACKER_TOKEN_HPP

#include <string>
using std::string;

#include "../gutils/ENUM_CLASS.hpp"
#include "../gutils/gutils.hpp"

namespace tracker {
    struct Token {
        ENUM_CLASS(Type, uint8_t, Comma, Hyphen, DollarSign, Number, LBrace, RBrace, LParen, RParen, Colon, FSlash, Word, End, Unknown)
        PUBLIC
        [[nodiscard]] constexpr string toString() const {
            switch ((uint8_t) *this) {
                case Comma: return "Comma";
                case Hyphen: return "Hyphen";
                case DollarSign: return "DollarSign";
                case Number: return "Number";
                case LBrace: return "LBrace";
                case RBrace: return "RBrace";
                case LParen: return "LParen";
                case RParen: return "RParen";
                case Colon: return "Colon";
                case FSlash: return "FSlash";
                case Word: return "Word";
                case End: return "End";

                default: return "Unknown";
            }
        }
        END

        string value;
        Token::Type type;

        constexpr Token(const string& value, const Token::Type type) : value(value), type(type) { }

        constexpr Token() : value(string{}), type(Type::Unknown) {}

        [[nodiscard]] constexpr string toString() const {
            return "Token { value = \"" + value + "\", type = " + type.toString() + " }";
        }
    };
}

#endif //BESTBUYTRACKER_TOKEN_HPP