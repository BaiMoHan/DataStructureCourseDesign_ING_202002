# README

## 1. Introduction

This is my data structure course design in Huazhong University of Science and Technology. I major in Computer Science, but at the same time, I have uploaded the tasks and data of Cyber Science and Engineering, I choose the topic on High-level Programming Language Format Processing. To improve my English, I write this in English but write comments in Chinese.

## 2. Commit

1. Upload the necessary files
2. Initialize both classes
   - class Lexer: use the DFA to identify the words, which include identifiers, keywords, constants, string literals, operators and white space(comments).
   - Parser: print the syntex tree in terms of the tokenlist that is the member of Lexer.
3. Try identifying identifiers
   - If I tried using the two level array, I would received the error SIGSSGN. I can't find the reason, so I used vector in C++.
4. Finish identifying identifiers and keywords
5. Try identifying constants
   - some problems in "print"
6. Delete some comments and finish identifying constants
7. Finish identifying "include file name format"
8. Finish identifying + ++ += and add README
9. Solve the bug of No.8
   - Logically, I made the wrong loops
10. Finish identifying -- -= - ++ += * *= / /= // 
    - I will think the solutions on the comments whose format is "/*    */"
11. Finish identifying , ; %
    - After reading the ',' from the file, the file pointer seems no to move to next char automatically. I did not find the reason, so I have to move the file pointer when necessary.
    - I guess it was my computer system that caused the problem.

