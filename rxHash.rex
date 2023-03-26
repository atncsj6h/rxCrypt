#! /usr/bin/env rexx
/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Copyright Enrico Sorichetti 2018 - 2019
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
*/

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Len = 0
Msg = 00
MD = e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
*/
parse arg file
test = filespec( "name", file )
FN      = test~lower~left(6)
if  FN~left(4) = "sha1" then ,
    FN = "sha1"

say
say "Using     " file
say "Test      " test
say "Function  " FN

say

tk  = 0

seqn = 0
pass = 0
fail = 0

do while ( lines(file) > 0 )

    if  ( tk = 3  ) then do

        seqn +=1

        if  ( len = 0 ) then ,
            in = ""

        interpret "ret = "FN"(in)"
        if  ( ret = exp ) then ,
            pass += 1
        else do
            say "test      "  seqn
            say "in        "  ">>>"c2x(in)"<<<"
            say "expected  "  ">>>"c2x(exp)"<<<"
            say "returned  "  ">>>"c2x(ret)"<<<"
            fail +=1
        end
        tk  = 0

    end

    line = linein(file)

    if  translate(word(line,1)) = "LEN"      then do
        len = word(line,3)
        tk  +=1; iterate
    end
    if  translate(word(line,1)) = "MSG"        then do
        in = x2c(word(line,3))
        tk  +=1; iterate
    end
    if  translate(word(line,1)) = "MD"  then do
        exp = x2c(word(line,3))
        tk  +=1; iterate
    end

    if  line \= "" then ,
        say line

end

if  ( tk = 3  ) then do

    seqn +=1

    if  ( len = 0 ) then ,
        in = ""

    interpret "ret = "FN"(in)"
    if  ( ret = exp ) then ,
        pass += 1
    else do
        say "test      "  seqn
        say "in        "  ">>>"c2x(in)"<<<"
        say "expected  "  ">>>"c2x(exp)"<<<"
        say "returned  "  ">>>"c2x(ret)"<<<"
        fail +=1
    end
    tk  = 0
end

say
say "test count " seqn
say "pass       " pass
say "fail       " fail

exit

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/

::requires  "rxcrypt" LIBRARY
