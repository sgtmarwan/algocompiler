#include "Lexer.h"
#include <algorithm>
#include <stdexcept>
#include "Syntax.h"
#include <unordered_map>
#include <utility>


static const std::wstring keywords[] = { algoKw, beginKw, endKw, varKw, ifKw, thenKw, elseKw, endifKw, constKw };
static const std::wstring operators[] = { intDivOp, modOp };

static const std::pair<std::wstring, Token::Type> kwsPairs[] = { {algoKw, Token::Type::algoKw},
	{beginKw, Token::Type::beginKw},
	{endKw, Token::Type::endKw},
	{varKw, Token::Type::varKw},
	{ifKw, Token::Type::ifKw},
	{thenKw, Token::Type::thenKw},
	{elseKw, Token::Type::elseKw},
	{endifKw, Token::Type::endifKw},
	{constKw, Token::Type::constKw} };
static const std::unordered_map<std::wstring, Token::Type> kwToTypeMap(opsPairs, opsPairs + sizeof(opsPairs) / sizeof(std::pair<std::wstring, Token::Type>));

static const std::pair<std::wstring, Token::Type> opsPairs[] = { {intDivOp, Token::Type::intDivOp}, {modOp, Token::Type::modOp} };
static const std::unordered_map<std::wstring, Token::Type> opToTypeMap(opsPairs, opsPairs + sizeof(opsPairs) / sizeof(std::pair<std::wstring, Token::Type>));

static Token::Type isKeyword(const std::wstring_view str)
{
	constexpr size_t size = sizeof(keywords) / sizeof(std::wstring);
	const std::wstring* kw = std::find(keywords, keywords + size, str);
	if (kw != keywords + size)
	{
		return kwToTypeMap.at(*kw);
	}
	else
	{
		return Token::Type::none;
	}
}
static Token::Type isOp(const std::wstring_view str)
{
	constexpr size_t size = sizeof(operators) / sizeof(std::wstring);
	const std::wstring* op = std::find(operators, operators + size, str);
	if (op != operators + size)
	{
		return opToTypeMap.at(*op);
	}
	else
		return Token::Type::none;
}

std::vector<Token> Lex(const std::wstring_view src)
{
	std::vector<Token> tokens;

	std::wstring currentTok;

	enum class TokenType
	{
		real, integer, idnt, alphnumidx, none
	} tt = TokenType::none, ltt = TokenType::none;

	for (size_t i = 0; i < src.size(); ++i)
	{
		wchar_t chr = src[i];
		switch (chr)
		{
		case L'+':
			tokens.emplace_back(Token::Type::plusOp);
			break;
		case L'-':
			tokens.emplace_back(Token::Type::minusOp);
			break;
		case L'/':
			tokens.emplace_back(Token::Type::divOp);
			break;
		case L'*':
			tokens.emplace_back(Token::Type::multOp);
			break;
		case L'%':
			tokens.emplace_back(Token::Type::modOp);
			break;
		case L'(':
			tokens.emplace_back(Token::Type::oP);
			break;
		case L')':
			tokens.emplace_back(Token::Type::cP);
			break;
		case L'<':
			if (src[i + 1] == L'-')
			{
				tokens.push_back(Token::Type::assign);
				++i;
			}
			break;
		case L';':
			tokens.emplace_back(Token::Type::semicol);
			break;
		case L'.':
			if (tt == TokenType::integer)
			{
				tt = TokenType::real;
			}
			break;
		case L'\"':
			++i;
			chr = src[i];
			while (chr != '\"')
			{
				currentTok.push_back(chr);
				++i;
				chr = src[i];
			}
			tokens.emplace_back(currentTok, Token::Type::string);
			currentTok.clear();
			break;
		default:
			if (iswalnum(chr) || chr == L'_')
			{
				currentTok.push_back(chr);
			}
			if (iswalpha(chr) || chr == L'_')
			{
				tt = TokenType::idnt;
			}
			if (iswdigit(chr))
			{
				if (tt != TokenType::real && tt != TokenType::idnt)
					tt = TokenType::integer;
			}
			break;
		}
		if ((size_t)tt < (size_t)TokenType::alphnumidx)
		{
			if (!iswalnum(src[i + 1]))
			{
				switch (tt)
				{
				case TokenType::real:
					if (!iswdigit(currentTok[currentTok.size() - 1]))
						currentTok.push_back(L'0');
					tokens.emplace_back(std::stod(currentTok), Token::Type::real);
					break;
				case TokenType::integer:
					tokens.emplace_back(std::stoll(currentTok), Token::Type::integer);
					break;
				case TokenType::idnt:
					Token::Type t = isKeyword(currentTok);
					if (t < Token::Type::kwEnd && t > Token::Type::kwStart)
					{
						tokens.emplace_back(currentTok, t);
					}
					else
					{
						t = isOp(currentTok);
						if (t < Token::Type::opEnd && t > Token::Type::opStart)
						{
							tokens.emplace_back(currentTok, t);
						}
						else
						{
							tokens.emplace_back(currentTok, Token::Type::idnt);
						}
					}
					break;
				case TokenType::alphnumidx:
					break;
				case TokenType::none:
					break;
				default:
					break;
				}
				currentTok.clear();
				tt = TokenType::none;
			}
		}
	}
	return tokens;
}
