Program            = FunctionDecl

FunctionDecl       = Type Identifier "(" ")" CompoundStmt

Type               = "int"

CompoundStmt       = "{" Statement* "}"

Statement          = ReturnStmt

ReturnStmt         = "return" Expression ";"

Expression         = PrimaryExpression | AdditiveExpression

PrimaryExpression  = NumberLiteral | Identifier

AdditiveExpression = PrimaryExpression ("+" | "-") PrimaryExpression