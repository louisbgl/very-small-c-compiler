Program                  = FunctionDecl

FunctionDecl             = Type Identifier "(" ")" CompoundStmt

Type                     = "int"

Identifier               = [a-zA-Z_][a-zA-Z0-9_]*

CompoundStmt             = "{" Statement* "}"

Statement                = ";" | ReturnStmt | VariableDeclaration | Assignment | IfStmt

ReturnStmt               = "return" Expression ";"

VariableDeclaration      = Type Identifier ("=" Expression)? ";"

Assignment               = Identifier "=" Expression ";"

IfStmt                  = "if" "(" Expression ")" CompoundStmt

Expression               = AdditiveExpression

AdditiveExpression       = MultiplicativeExpression (("+" | "-") MultiplicativeExpression)*

MultiplicativeExpression = PrimaryExpression (("*" | "/") PrimaryExpression)*

PrimaryExpression        = IntegerLiteral | Identifier | "(" Expression ")"