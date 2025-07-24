Program            = FunctionDecl

FunctionDecl       = Type Identifier "(" ")" CompoundStmt

Type               = "int"

CompoundStmt       = "{" Statement* "}"

Statement          = ReturnStmt

ReturnStmt         = "return" Expression ";"

Expression         = AdditiveExpression

PrimaryExpression  = IntergerLiteral

AdditiveExpression = PrimaryExpression (("+" | "-") PrimaryExpression)*