Program            = FunctionDecl

FunctionDecl       = Type Identifier "(" ")" CompoundStmt

Type               = "int"

CompoundStmt       = "{" Statement* "}"

Statement          = ReturnStmt

ReturnStmt         = "return" Expression ";"

Expression         = AdditiveExpression

AdditiveExpression = MultiplicativeExpression (("+" | "-") MultiplicativeExpression)*

MultiplicativeExpression = PrimaryExpression (("*" | "/") PrimaryExpression)*

PrimaryExpression  = IntegerLiteral | "(" Expression ")"