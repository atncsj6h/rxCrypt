#! /usr/bin/env rexx
/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Copyright Enrico Sorichetti 2018 - 2019
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
*/

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
COUNT       = 0
KEY         = 8809e7dd3a959ee5d8dbb13f501f2274
IV          = e5c0bb535d7d54572ad06d170a0e58ae
PLAINTEXT   = 1fd4ee65603e6130cfc2a82ab3d56c24
CIPHERTEXT  = b127a5b4c4692d87483db0c3b0d11e64
*/

parse arg test

say
say "Using" test
dir = ""
tk  = 0

encrypts = 0
decrypts = 0

detail = 1

do while ( lines(test) > 0 )

    if  ( tk = 5 & dir \= "" ) then do
        /*
            RexxRoutine4( RexxObjectPtr, rxcbc, RexxStringObject, Action ,
                RexxStringObject, In, RexxStringObject, Vector, RexxStringObject, Key )
        */
        interpret dir"s +=1"
        ret = ofb( in, key, iv )
        if  ( ret = exp ) then ,
            success +=1
        else do
            failure +=1
            say "test number" right(ts,2) "," dir
            say "key       "  ">>>"c2x(key)"<<<"
            say "IV        "  ">>>"c2x(IV)"<<<"
            say "in        "  ">>>"c2x(in)"<<<"
            say "expected  "  ">>>"c2x(exp)"<<<"
            say "returned  "  ">>>"c2x(ret)"<<<"
        end
        tk  = 0
    end

    line = linein(test)

    if  translate(word(line,1)) = "[ENCRYPT]" then do
        dir  = "encrypt"
        success = 0
        failure = 0
        iterate
    end

    if  translate(word(line,1)) = "[DECRYPT]" then do
        dir  = "decrypt"
        success = 0
        failure = 0
        iterate
    end

    if  translate(word(line,1)) = "COUNT"      then do
        ts  = word(line,3)
        tk  +=1; iterate
    end
    if  translate(word(line,1)) = "KEY"        then do
        key = x2c(word(line,3))
        tk  +=1; iterate
    end
    if  translate(word(line,1)) = "IV"         then do
        IV  = x2c(word(line,3))
        tk  +=1; iterate
    end
    if  translate(word(line,1)) = "PLAINTEXT"  then do
        if  dir = "encrypt" then ,
            in  = x2c(word(line,3))
        else ,
            exp = x2c(word(line,3))
        tk  +=1; iterate
    end
    if  translate(word(line,1)) = "CIPHERTEXT" then do
        if  dir = "encrypt" then ,
            exp = x2c(word(line,3))
        else ,
            in  = x2c(word(line,3))
        tk  +=1; iterate
    end

    if  line \= "" then ,
        say line

end

if  ( tk = 5 & dir \= "" ) then do
    /*
        RexxRoutine4( RexxObjectPtr, rxcbc, RexxStringObject, Action ,
            RexxStringObject, In, RexxStringObject, Vector, RexxStringObject, Key )
    */
    interpret dir"s +=1"
    ret = ofb( in, key, iv)
    if  ( ret = exp ) then ,
        success +=1
    else do
        failure +=1
        say "test number" right(ts,2) "," dir
        say "key       "  ">>>"c2x(key)"<<<"
        say "IV        "  ">>>"c2x(IV)"<<<"
        say "in        "  ">>>"c2x(in)"<<<"
        say "expected  "  ">>>"c2x(exp)"<<<"
        say "returned  "  ">>>"c2x(ret)"<<<"
    end
    tk  = 0
end

say "encrypts   " encrypts
say "  success  " success
say "  failure  " failure

say "decrypts   " decrypts
say "  success  " success
say "  failure  " failure

exit

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/

::requires  "rxcrypt" LIBRARY
