grammar fluenc;

program 
	: instruction*
	;

instruction
	: (COMMENT | function | structure | global | ns | use)
	;

use
	: 'use' STRING
	;

ns
	: 'namespace' ID '{' instruction* '}'
	;

structure
	: 'struct' name=ID (':' typeName (',' typeName)*)? '{' field (',' field)* '}'
	| 'struct' name=ID (':' typeName (',' typeName)*)?
	;

global
	: 'global' ID ':' expression
	;

assignment
	: field ':' expression
	;

field
	: typeName? ID (':' expression)?
	;
	
function 
	: attribute='export' typeName name=ID '(' (argument (',' argument)*)? ')' block
	| attribute='import' typeName name=ID '(' (argument (',' argument)*)? ')'
	| 'function' name=ID '(' (argument (',' argument)*)? ')' block
	;

literal
	: INT #int32Literal
	| INT'L' #int64Literal
	| BOOL #boolLiteral
	| STRING #stringLiteral
	| INT'u' #uint32Literal
	| CHARACTER #charLiteral
	| INT'u8' #byteLiteral
	| 'nothing' #nothing
	;

with
	: 'with' '{' assignment (',' assignment)* '}'
	;
	
expression
	: ID '(' (expression (',' expression)*)? ')'		#call
	| literal											#constant
	| left=expression op=OP right=expression			#binary
	| ID ('.' ID)* with?								#member
	| typeName '{' assignment? (',' assignment)* '}'	#instantiation
	| 'if' '(' expression ')' block						#conditional
	| '[' expression? (',' expression)* ']'				#array
	| '(' expression ')'								#group
	| '...' expression									#expansion
	| 'let' ID '=' expression							#local
	;

continuation
	: ID '(' (expression (',' expression)*)? ')'
	;

ret
	: 'return' value=expression? ('->' chained=continuation)?
	;

block
	: '{' expression* ret '}'
	;

argument
	: typeName ID #standardArgument
	| '(' argument (',' argument)* ')' #tupleArgument
	;

typeName
	: (ID | '...') #regularType
	| 'function' '(' (typeName (',' typeName)*) ')' #functionType
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
	| '&&'
	| '||'
	| '%'
	;

COMMENT
	: '//' ~[\r\n]* -> skip
	;

STRING
	: '"' ~('"')+ '"'
	;

CHARACTER
	: '\'' ~('\'')+ '\''
	;
	
WS
	: [ \t\r\n;]+ -> skip
	;

ID
	: '::'? CHAR (CHAR | DIGIT | '::')*
	;

fragment CHAR
	: [a-zA-Z_]
	;

fragment DIGIT
	: [0-9]
	;
