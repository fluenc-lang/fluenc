grammar dz;

program 
	: (function | structure)*
	;

structure
	: 'struct' ID '{' field (',' field)* '}'
	;

assignment
	: field ':' expression
	;

field
	: ID
	;
	
function 
	: attribute=ID? typeName name=ID '(' (argument (',' argument)*)? ')' block?
	;

literal
	: INT #int32Literal
	| INT'L' #int64Literal
	| BOOL #boolLiteral
	| STRING #stringLiteral
	;
	
expression
	: ID '(' (expression (',' expression)*)? ')'	#call
	| literal										#constant
	| left=expression op=OP right=expression		#binary
	| ID ('.' ID)*									#member
	| typeName '{' assignment (',' assignment)* '}'	#instantiation
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

STRING
	: '"' ~('"')+ '"'
	;
	
WS
	: [ \t\r\n;]+ -> skip
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
