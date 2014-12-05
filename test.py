#!/usr/bin/env python

from __future__ import print_function

# Use the srilm module
from srilm import *

# Initialize a trigram LM variable (1 = unigram, 2 = bigram and so on)
n = initLM(5)

# Read 'sample.lm' into the LM variable
readLM(n, "sample.lm")

# How many n-grams of different order are there ?
print("1. Number of n-grams:")
print("   There are {} unigrams in this LM".format(howManyNgrams(n, 1)))
print("   There are {} bigrams in this LM".format(howManyNgrams(n, 2)))
print("   There are {} trigrams in this LM".format(howManyNgrams(n, 3)))
print("   There are {} 4-grams in this LM".format(howManyNgrams(n, 4)))
print("   There are {} 5-grams in this LM".format(howManyNgrams(n, 5)))
print()

# Query the LM for some n-gram log probabilities.
# Note that a SRI language model uses backoff smoothing, so if an n-gram is
# not present in the LM, it will compute it using a smoothed lower-order
# n-gram distribution.
print("2. N-gram log probabilities:")

p1 = getUnigramProb(n, 'good')
print("   p('good') = {}".format(p1))

p2 = getBigramProb(n, 'of the')
print("   p('of the') = {}".format(p2))

p3 = getBigramProb(n, 'nitin madnani')
print("   p('nitin madnani') = {}".format(p3))

p4 = getTrigramProb(n, 'there are some')
print("   p('there are some') = {}".format(p4))

# generic n-gram probability function
p5 = getNgramProb(n, 'do more about your', 4)
print("   p('do more about your') = {}".format(p5))

p6 = getNgramProb(n, 'or whatever has yet to', 5)
print("   p('or whatever has yet to') = {}".format(p6))

print()

# Query the LM to get the final log probability for an entire sentence.
# Note that this is  different from a n-gram probability because
  # (1) For a sentence, SRILM appends <s> and </s> to its beginning
  #     and the end respectively
  # (2) The log prob of a probability is the sum of all individual
  #     n-gram log probabilities
print("3. Sentence log probabilities and perplexities:")
sprob = getSentenceProb(n,'there are some good',4)
print("   p('there are some good') = {}".format(sprob))

# the perplexity
sppl = getSentencePpl(n,'there are some good', 4)
print("   ppl('there are some good') = {}".format(sppl))
print()

# number of OOVs in a sentence
print("4. OOvs:")
noov = numOOVs(n, 'there are some foobar', 4)
print("   nOOVs('there are some foobar') = {}".format(noov))
print()

# Query the LM to get the total log probability for the file named 'corpus'
print("5. Corpus log probabilties and perplexities:")
corpus = 'test.txt'
corpus_prob = getCorpusProb(n, corpus)
print("   Logprob for the file {} = {}".format(corpus, corpus_prob))

# Query the LM to get the perplexity for the file named 'corpus'
corpus_ppl = getCorpusPpl(n, corpus);
print("   Perplexity for the file {} = {}".format(corpus, corpus_ppl))

# Free LM variable
deleteLM(n);
