A syntax parser with the following grammar rules:

P::= S

S::= V:=E | read (V) | write(V) | while C do S od | S;S

C::= E< E | E> E | E= E | E<>E | E<= E | E>= E

E::= T| E+T| E-T

T::= F| T*F| T/F

F::= (E)| N| V

V::= a | b | ... | y | z | aV| bV| ... | yV| zV

N::= 0 | 1 | ... | 8 | 9 | 0N| 1N| ... | 8N| 9N

Error codes on Linux: echo $? -return echo code

Note: If a user input file contains multiple syntax errors,
your solution is only required to find and report the 1st syntax error

1) Your solution must print out “DanC Parser :: R<#>” as the first line of output with <#> being replaced by
your R#. The double colon “::” is required for correct grading of your submission.
2) If the provided user file is free of syntax errors:
a. Your solution must print out “Syntax Validated” as the last line of output.
b. Your solution must return with an exit code of 0.
3) If the provided user file contains syntax errors:
a. Your solution must print out “Error encounter: The next lexeme was <lexeme> and the next
token was <token>”
i. Where <lexeme> is the lexeme that caused the problem (examples: “x”, “<>”)
ii. Where <token> is the uppercase name of the token (examples: “IDENT”, “UNKNOWN”)
b. Your solution must return with an exit code of 1.
4) If the user did not provide a file as input:
a. Your solution must display an appropriate error message.
b. Your solution must return with an exit code of 2.
5) If the user did provide a file as input but the file does not exist:
a. Your solution must display an appropriate error message.
b. Your solution must return with an exit code of 3.