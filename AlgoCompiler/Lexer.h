#pragma once
#include <string>
#include <variant>
#include <vector>


struct Token
{
	enum class Type
	{
		string, chr, integer, real, boolean, keyword, oP, cP, assign, semicol, idnt,
		kwStart, algoKw, beginKw, endKw, varKw, ifKw, thenKw, elseKw, endifKw, constKw, kwEnd,
		opStart, plusOp, minusOp, multOp, intDivOp, divOp, modOp, opEnd, none
	} type;
	std::variant<std::wstring, wchar_t, int64_t, double, bool> value;
	
	template <typename T>
	Token(T v, Type t):
		type(t), value(v)
	{

	}
	Token(Type t):
		type(t)
	{

	}
};

std::vector<Token> Lex(const std::wstring_view src);
