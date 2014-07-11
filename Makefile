SRILM_LIBS=/home/speech/Resources/Tools/srilm/lib/i686-m64
SRILM_INC=/home/speech/Resources/Tools/srilm/include
PYTHON_INC=/opt/python/2.7/include/python2.7
PERL_INC=/usr/lib64/perl5/CORE

python: clean _srilm.so

_srilm.so: srilm.o srilm_python_wrap.o
	g++ -shared $^ -loolm -ldstruct -lmisc -lz -L$(SRILM_LIBS) -o $@

srilm_python_wrap.o: srilm_python_wrap.c
	g++ -c -fpic $< -I/usr/local/include/ -I$(SRILM_INC) -I$(PYTHON_INC)

srilm_python_wrap.c: srilm_python.i
	swig -python $<

perl: clean srilm.so

srilm.so: srilm.o srilm_perl_wrap.o
	g++ -shared $^ -loolm -ldstruct -lmisc -lz -L$(SRILM_LIBS) -o $@

srilm_perl_wrap.o: srilm_perl_wrap.c
	g++ -c -fpic $< -I/usr/local/include/ -I$(SRILM_INC) -I$(PERL_INC)

srilm_perl_wrap.c: srilm_perl.i
	swig -perl $<

srilm.o: srilm.c
	g++ -c -fpic $< -I/usr/local/include/ -I$(SRILM_INC) -I$(PYTHON_INC)

clean:
	\rm -fr srilm.o srilm_*_wrap.* *.so srilm.py* srilm.pm
