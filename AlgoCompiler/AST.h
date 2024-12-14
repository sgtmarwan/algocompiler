#pragma once
#include "Lexer.h"

class INode
{

};

class RootNode : public INode
{

};


RootNode* buildAst(const std::vector<Token>& tokens);