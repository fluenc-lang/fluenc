grammar dz;

program 
	: function+ 
	;
	
function 
	: typeName ID '(' (argument (',' argument)*)? ')' block
	;
	
expression
	: ID '(' (expression (',' expression)*)? ')'	#call
	| INT											#constant
	| left=expression op=OP right=expression		#binary
	| ID											#member
	;

assignment
	: 'var' ID '=' expression
	;

ret
	: 'return' expression
	;

conditional
	: 'if' '(' expression ')' block
	;

statement
	: assignment
	| conditional
	;

block
	: '{' statement* ret '}'
	;

argument
	: typeName ID
	;

typeName
	: ID
	;
	
INT
	: [0-9]+ 
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
    :   CHAR (CHAR | DIGIT)*
    ;

fragment
CHAR
    : [a-zA-Z_]
    ;

fragment
DIGIT
    : [0-9]
    ;
