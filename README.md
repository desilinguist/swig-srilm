[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/desilinguist/swig-srilm/trend.png)](https://bitdeli.com/free "Bitdeli Badge")

## SWIG-SRILM: A SWIG-based wrapper for an SRILM language model
==========

### Description
This package contains files to generate Perl and Python wrappers for 
[SRILM](http://www.speech.sri.com/projects/srilm/) language models.

### Requirements
- GNU make
- Simplified Wrapper & Interface Generator ([SWIG](http://swig.org/)) 
- A local Python and/or Perl installation
- The SRILM toolkit (v1.7.1). If you have an older version of SRILM e.g., the 1.5.x series then you should use the `old_srilm` branch. Note that SRILM should have been compiled as position independent code. You can do that by using the command `MAKE_PIC=yes make` when compiling SRILM.


### Installation:

#### Linux

- Modify the following environment variables at the top of the included Makefile:
 - `SRILM_LIBS` : The directory containing the SRILM libraries
 - `SRILM_INC`  : The directory containing the SRILM header files
 - `PYTHON_INC` : The directory containing the python header files
 - `PERL_INC`   : The directory containing the perl header files

- To create a Python module, run 'make python' in this directory. 
Copy `_srilm.so` and `srilm.py` to your directory where you want to 
use the python module. You can run the included `test.py` script to check 
whether the compiled module works correctly. The output of test.py should be
the following:

```
1. Number of n-grams:
   There are 11868 unigrams in this LM
   There are 59481 bigrams in this LM
   There are 16744 trigrams in this LM
   There are 13787 4-grams in this LM
   There are 12082 5-grams in this LM

2. N-gram log probabilities:
   p('good') = -3.49373698235
   p('of the') = -0.558740794659
   p('nitin madnani') = -99.0
   p('there are some') = -0.985605716705
   p('do more about your') = -0.469523012638
   p('or whatever has yet to') = -0.53226429224

3. Sentence log probabilities and perplexities:
   p('there are some good') = -9.85836982727
   ppl('there are some good') = 93.6858444214

4. OOvs:
   nOOVs('there are some foobar') = 1

5. Corpus log probabilties and perplexities:
   Logprob for the file test.txt = -33.6016654968
   Perplexity for the file test.txt = 94.7476806641
```

- To create a Perl module, run `make perl` in this directory. 
Copy `srilm.so` and `srilm.pm` to the directory of your choice. 
Run the included Perl script 'test.pl' to test whether the compiled module works correctly. The output should be the same as above.

#### Mac OS X

*Note*: This has only been tested on OS X El Capitan and only with the built-in versions of python (2.7.10) and perl (5.18).

- Check out the `macosx` branch.

- Make sure you have compiled the SRILM libraries (`MAKE_PIC=yes make`).

- Go to the directory containing the SRILM header files (`$SRILM/include`), open `File.h` and comment out the line that says `#include "zio.h"`. This is necessary because even though SRILM is supposed to rename the `zopen()` function to `my_zopen()` on OS X since `zlib` is installed by default, it does not seem to work. So, this is a hacky workaround. 

- Modify the following environment variables at the top of `Makefile.osx`:
 - `SRILM_LIBS` : The directory containing the SRILM libraries
 - `SRILM_INC`  : The directory containing the SRILM header files

   **IMPORTANT**: DO NOT change the `PYTHON_INC` and `PERL_INC` variables as they are set to be the default values for OS X El Capitan.

- To compile the python module, run `make -f Makefile.osx python` and to compile the perl module, run `make -f Makefile.osx perl`. Note that the compiled modules will *only* work with the default OS X python and perl interpreters, i.e., `/usr/bin/python` and `/usr/bin/perl`.

- You should be able to run `/usr/bin/python test.py` and `/usr/bin/perl test.pl` to test that the modules work and obtain the same output in the Linux case.

### Usage:
Usage is clearly illustrated in files `test.pl` and `test.py`. 


