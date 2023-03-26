##  README
*   . . .
*   a rexx external function package providing cryptographic functions
*   . . .
* * *

##  COPYRIGHT
*   Copyright (c) 2019, 2023 Enrico Sorichetti
*   Distributed under the Boost Software License, Version 1.0
*   See accompanying file `LICENSE_1_0.txt` or copy at
*   `[http://www.boost.org/LICENSE_1_0.txt](http://www.boost.org/LICENSE_1_0.txt)`
* * *

##  public domain notice ( lib Directory )

*   the content of the lib directory has been extracted and reworked
*   from sources in the public domain,
*   and each member contains the proper notice about it
*   ( work in process to give credit when credit is due )
*
*   the NIST testVectors were downloaded from
*   https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Algorithm-Validation-Program/documents/aes/KAT_AES.zip
*   as far as I know almost anything originating from the US government is not encumbered by any licence
*   and can be redistributed freely
* * *

##  how to get it
*   . . .
*   `git clone https://github.com/atncsj6h/rxcrypt`
*   `git@github.com:atncsj6h/rxcrypt`
*   . . .
* * *

##  usage notes
*   . . .
*   clone anywhere with any name
*   mkdir  'the build directory'
*   cd     'the build directory'
*   no need for a particular naming convention
*
*   run ...
*   `cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX<prefix> <the source dir>`
*   `gmakwe/make`

*   to test against the NIST test vectors
*
*   for f in <the source dir>/testVectors/SHA* ; do rexx rxhash ${f}; done;
*   for f in <the source dir>/testVectors/ECB* ; do rexx rxecb ${f}; done;
*   for f in <the source dir>/testVectors/OFB* ; do rexx rxofb ${f}; done;
*   for f in <the source dir>/testVectors/CBC* ; do rexx rxcbc ${f}; done;
*   for f in <the source dir>/testVectors/ECB* ; do rexx rxecb ${f}; done;
*   . . .
*   work in progress to standardize the reporting of the test suite
* * *

##  tested environments
*   . . .
*   macOS[X] Big Sur
*   tested and working on
*   arm-native intel-native
*
* * *

##  dependencies
*   ...
* * *

##  nice to have
*   . . .
* * *
##  changes
*   . . .
*   removed the CMAKE requirement
*   implemented a very simple makefile
*   . . .
* * *

##  contribution guidelines
*   . . .
*   Comments welcome
*   Suggestions welcome
*   . . .
*   but remember ... My repository, my rules :-)
*   . . .
* * *

##  other
*   [Markdown Information](https://bitbucket.org/tutorials/markdowndemo)
* * *

