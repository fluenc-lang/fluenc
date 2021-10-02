grammar dz;

program 
	: (function | structure | global)*
	;

structure
	: 'struct' ID '{' field (',' field)* '}'
	;

global
	: 'global' ID ':' literal
	;

assignment
	: field ':' expression
	;

field
	: ID (':' literal)?
	;
	
function 
	: attribute=ID? typeName name=ID '(' (argument (',' argument)*)? ')' block?
	;

literal
	: INT #int32Literal
	| INT'L' #int64Literal
	| BOOL #boolLiteral
	| STRING #stringLiteral
	| INT'u' #uint32Literal
	;
	
expression
	: ID '(' (expression (',' expression)*)? ')'		#call
	| literal											#constant
	| left=expression op=OP right=expression			#binary
	| ID ('.' ID)*										#member
	| typeName '{' assignment? (',' assignment)* '}'	#instantiation
	| 'if' '(' expression ')' block						#conditional
	;

ret
	: 'return' value=expression? ('->' chained=expression)?
	;

block
	: '{' expression* ret '}'
	;

argument
	: typeName ID
	;

typeName
	: ID '...'?
	;
	
INT
	: '-'?[0-9]+
	| '0x'[0-9A-F]+
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
