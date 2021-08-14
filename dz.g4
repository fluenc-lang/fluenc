grammar dz;

program 
	: (function | structure)*
	;

structure
	: 'struct' ID '{' field+ '}'
	;

field
	: typeName ID
	;
	
function 
	: attribute=ID? typeName name=ID '(' (argument (',' argument)*)? ')' block?
	;

literal
	: INT #int32Literal
	| INT'L' #int64Literal
	| BOOL #boolLiteral
	;
	
expression
	: ID '(' (expression (',' expression)*)? ')'	#call
	| literal										#constant
	| left=expression op=OP right=expression		#binary
	| ID											#member
	;

ret
	: 'return' value=expression? ('->' chained=expression)?
	;

conditional
	: 'if' '(' expression ')' block
	;

block
	: '{' conditional* ret '}'
	;

argument
	: typeName ID
	;

typeName
	: ID '...'?
	;
	
INT
	: '-'?[0-9]+
	;

BOOL
	: 'true'
	| 'false'
	;

OP
	: '<'
	| '>'
	| '=='
	| '!='
	| '*'
	| '/'
	| '+'
	| '-'
	| '<='
	| '>='
	;
	
WS
	: [ \t\r\n;]+-> skip
	;

ID
	: CHAR (CHAR | DIGIT)*
	;

fragment CHAR
	: [a-zA-Z_]
	;

fragment DIGIT
	: [0-9]
	;
