#include "Prob.h"
#include "Ngram.h"
#include "Vocab.h"

#include "srilm.h"
#include <cstdio>
#include <cstring>
#include <cmath>

Vocab *swig_srilm_vocab;
const float BIGNEG = -99;

// Initialize the ngram model
Ngram* initLM(int order) {
  swig_srilm_vocab = new Vocab;
  return new Ngram(*swig_srilm_vocab, order);
}

// Delete the ngram model
void deleteLM(Ngram* ngram) {
  delete swig_srilm_vocab;
  delete ngram;
}

// Get index for given string
unsigned getIndexForWord(const char *s) {
  unsigned ans;
  ans = swig_srilm_vocab->addWord((VocabString)s);
  if(ans == Vocab_None) {
    printf("Trying to get index for Vocab_None.\n");
  }
  return ans;
}

// Get the word for a given index
const char* getWordForIndex(unsigned i) {
  return swig_srilm_vocab->getWord((VocabIndex)i);
}

// Read in an LM file into the model
int readLM(Ngram* ngram, const char* filename) {
    File file(filename, "r");
    if(!file) {
        fprintf(stderr,"Error:: Could not open file %s\n", filename);
        return 0;
    }
    else
        return ngram->read(file, 0);
}

// Get the ngram probability of the given string
/*float getNgramProb(Ngram* ngram, const char* ngramstr, unsigned order) {
    const char* words[10];
    unsigned int indices[order];
    int numparsed, histsize, i, j;
    char* scp;
    float ans, adder;

    // Duplicate string so that we don't mess up the original
    scp = strdupa(ngramstr);

    // Parse the given string into words
    numparsed = Vocab::parseWords(scp, (VocabString *)words, 10);
    if(numparsed != order) {
        fprintf(stderr, "Error: Given order %d not correct.\n", order);
        return 0;
    }

    // Get indices for the words obtained above, if you don't find them, then add them
    // to the vocabulary and then get the indices.
    swig_srilm_vocab->addWords((VocabString *)words, (VocabIndex *)indices, order);
    ans = 0;

    for(i=(order-1); i>=0; i--) {

        histsize = i;

        if(histsize == 0) {
            unsigned int hist[1] = {Vocab_None};
            if(indices[i] == swig_srilm_vocab->ssIndex()) {
                adder = LogP_One;
            }
            else {
                adder = getWordProb(ngram, indices[i], hist, cap);
                if(adder == LogP_Zero)
                    return BIGNEG;
            }
        }
        else {
            unsigned int hist[histsize+1];
            for(j=0; j<histsize; j++)
                hist[j] = indices[i-j-1];
            hist[histsize] = Vocab_None;
            adder = getWordProb(ngram, indices[i], hist, cap);
            if(adder == LogP_Zero)
                return BIGNEG;
        }
        ans += adder;
    }
    ans /= log10(exp(1.0));
    return ans;
}
*/

// Get word probability
float getWordProb(Ngram* ngram, unsigned w, unsigned* context) {
    return (float)ngram->wordProb(w, context);
}

// Get unigram probability
float getUnigramProb(Ngram* ngram, const char* word) {
    unsigned index;
    float ans;

    // fill the history array the empty token
    unsigned hist[1] = {Vocab_None};

    // get the index for this word
    index = getIndexForWord(word);

    // Compute word probability
    ans = getWordProb(ngram, index, hist);

    // If the probability is zero, return the constant representing
    // log(0).
    if(ans == LogP_Zero)
        return BIGNEG;

    return ans;
}

// Get bigram probability
float getBigramProb(Ngram* ngram, const char* ngramstr) {
    const char* words[2];
    unsigned indices[2];
    unsigned numparsed;
    char* scp;
    float ans;

    // Create a copy of the input string to be safe
    scp = strdupa(ngramstr);

    // Parse the bigram into the words
    numparsed = Vocab::parseWords(scp, (VocabString *)words, 2);
    if(numparsed != 2) {
        fprintf(stderr, "Error: Given ngram is not a bigram.\n");
        return -1;
    }

    // Add the words to the vocabulary
    swig_srilm_vocab->addWords((VocabString *)words, (VocabIndex *)indices, 2);

    // Fill the history array
    unsigned hist[2] = {indices[0], Vocab_None};

    // Compute the bigram probability
    ans = getWordProb(ngram, indices[1], hist);

    // Return the representation of log(0) if needed
    if(ans == LogP_Zero)
        return BIGNEG;

    return ans;
}

// Get trigram probability
float getTrigramProb(Ngram* ngram, const char* ngramstr) {
    const char* words[6];
    unsigned indices[3];
    unsigned numparsed;
    char* scp;
    float ans;

    // Duplicate
    scp = strdupa(ngramstr);

    numparsed = Vocab::parseWords(scp, (VocabString *)words, 6);
    if(numparsed != 3) {
        fprintf(stderr, "Error: Given ngram is not a trigram.\n");
        return 0;
    }

    swig_srilm_vocab->addWords((VocabString *)words, (VocabIndex *)indices, 3);

    unsigned hist[3] = {indices[1], indices[0], Vocab_None};

    ans = getWordProb(ngram, indices[2], hist);

    if(ans == LogP_Zero)
        return BIGNEG;

    return ans;
}

// probability and perplexity at the sentence level
unsigned sentenceStats(Ngram* ngram, const char* sentence, unsigned length, TextStats &stats) {
    float ans;
    const char* words[100];
    unsigned indices[2];
    unsigned numparsed;
    char* scp;

    // Create a copy of the input string to be safe
    scp = strdupa(sentence);

    // Parse the bigram into the words
    numparsed = Vocab::parseWords(scp, (VocabString *)words, 100);
    if(numparsed != length) {
        fprintf(stderr, "Error: Number of words in sentence does not match given length.\n");
        return 1;
    }
    else {
        ngram->sentenceProb(words, stats);
        return 0;
    }
}


float getSentenceProb(Ngram* ngram, const char* sentence, unsigned length) {
    TextStats stats;
    float ans;

    if(!sentenceStats(ngram, sentence, length, stats)) {
        if (stats.prob == LogP_Zero) {
            ans = BIGNEG;
        }
        else {
            ans = stats.prob;
        }
    }
    else {
        ans = -1.0;
    }

    return ans;
}


float getSentencePpl(Ngram* ngram, const char* sentence, unsigned length) {
    float ans;
    TextStats stats;

    if(!sentenceStats(ngram, sentence, length, stats)) {
        int denom = stats.numWords - stats.numOOVs - stats.zeroProbs + stats.numSentences;
        if (denom > 0) {
            ans = LogPtoPPL(stats.prob / denom);
        }
        else {
            ans = -1.0;
        }
    }
    else {
        ans = -1.0;
    }

    return ans;
}

// how many OOVs in the sentence
int numOOVs(Ngram* ngram, const char* sentence, unsigned length) {
    float ans;
    TextStats stats;

    if(!sentenceStats(ngram, sentence, length, stats)) {
        ans = stats.numOOVs;
    }
    else {
        ans = -1;
    }

    return ans;
}

// probability and perplexity at the corpus level
unsigned corpusStats(Ngram* ngram, const char* filename, TextStats &stats) {
    File corpus(filename, "r");

    if(!corpus) {
        fprintf(stderr,"Error:: Could not open file %s\n", filename);
        return 1;
    }
    else {
        ngram->pplFile(corpus, stats, 0);
        return 0;
    }
}


float getCorpusProb(Ngram* ngram, const char* filename) {
    TextStats stats;
    float ans;
    if(!corpusStats(ngram, filename, stats))
        ans = stats.prob;
    else
        ans = -1.0;

    return ans;
}

float getCorpusPpl(Ngram* ngram, const char* filename) {
    TextStats stats;
    float ans;

    if(!corpusStats(ngram, filename, stats)) {
        int denom = stats.numWords - stats.numOOVs - stats.zeroProbs + stats.numSentences;
        if (denom > 0) {
            ans = LogPtoPPL(stats.prob / denom);
        }
        else {
            ans = -1.0;
        }
        return ans;
    }
}


// How many ngrams are in the model
int howManyNgrams(Ngram* ngram, unsigned order) {
  return ngram->numNgrams(order);
}


