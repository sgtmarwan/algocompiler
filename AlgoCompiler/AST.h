#pragma once
#include "Lexer.h"

enum class ASTNodeType
{
	root,
};

class ASTNode
{
public:
	const ASTNodeType type;
public:
	ASTNode(ASTNodeType type):
		type(type)
	{

	}
};

class RootNode : public ASTNode
{
public:
	RootNode():
		ASTNode(ASTNodeType::root)
	{

	}
};


RootNode* buildAst(const std::vector<Token>& tokens);