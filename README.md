[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/desilinguist/swig-srilm/trend.png)](https://bitdeli.com/free "Bitdeli Badge")

### SWIG-SRILM: A SWIG-based wrapper for an SRILM language model
==========

#### Description
This package contains files to generate Perl and Python wrappers for 
[SRILM](http://www.speech.sri.com/projects/srilm/) language models.

#### Requirements
- GNU make
- Simplified Wrapper & Interface Generator ([SWIG](http://swig.org/)) 
- A local Python and/or Perl installation
- The SRILM toolkit (v1.7.1). If you have an older version of SRILM e.g., the 1.5.x series then you should use the `old_srilm` branch. Note that SRILM should have been compiled as position independent code. You can do that by using the command `MAKE_PIC=yes make` when compiling SRILM.

#### Installation:
1. Modify the following environment variables at the top of the included Makefile:
 - `SRILM_LIBS` : The directory containing the SRILM libraries
 - `SRILM_INC`  : The directory containing the SRILM header files
 - `PYTHON_INC` : The directory containing the python header files
 - `PERL_INC`   : The directory containing the perl header files
2. To create a Python module, run 'make python' in this directory. 
Copy `_srilm.so` and `srilm.py` to your directory where you want to 
use the python module. You can run the included `test.py` script to check 
whether the compiled module works correctly. The output of test.py should be
the following:
```
   There are 32 unigrams in this LM
   There are 42 bigrams in this LM
   There are 1 trigrams in this LM

   p('good') = -1.3822170496
   p('nitin madnani') = -1.23044896126
   p('there are some') = -0.176091298461

   p('there are some good') = -4.79909944534

   ppl('there are some good') = 9.11632728577

   nOOVs('there are some foobar') =  1
  
   Logprob for the file test.txt = -6.77615213394
   Perplexity for the file test.txt = 9.29012870789
```
3. To create a Perl module, run `make perl` in this directory. 
Copy `srilm.so` and `srilm.pm` to the directory of your choice. 
Run the included Perl script 'test.pl' to test whether the compiled module works correctly.
The output should be the same as above.

#### Usage:
Usage is clearly illustrated in files `test.pl` and `test.py`. 


