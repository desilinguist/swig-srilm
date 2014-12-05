#!/usr/bin/env perl

# Use the srilm module
use srilm;

# Initialize a trigram LM variable (1 = unigram, 2 = bigrams)
$n = srilm::initLM(5);

# Read 'sample.lm' into the LM variable
srilm::readLM($n, "sample.lm");

# How many n-grams of different order are there ?
print "1. Number of n-grams:\n";
print "   There are " . srilm::howManyNgrams($n, 1) . " unigrams in this LM\n";
print "   There are " . srilm::howManyNgrams($n, 2) . " bigrams in this LM\n";
print "   There are " . srilm::howManyNgrams($n, 3) . " trigrams in this LM\n";
print "   There are " . srilm::howManyNgrams($n, 4) . " 4-grams in this LM\n";
print "   There are " . srilm::howManyNgrams($n, 5) . " 5-grams in this LM\n\n";

# Query the LM for some n-gram log probabilities.
# Note that a SRI language model uses backoff smoothing, so if an n-gram is
# not present in the LM, it will compute it using a smoothed lower-order
# n-gram distribution.
print "2. N-gram log probabilities:\n";
$p1 = srilm::getUnigramProb($n, "good");
print "   p('good') = " . $p1 . "\n";

$p2 = srilm::getBigramProb($n, "of the");
print "   p('of the') = " . $p2 . "\n";

$p3 = srilm::getBigramProb($n, "nitin madnani");
print "   p('nitin madnani') = " . $p3 . "\n";

$p4 = srilm::getTrigramProb($n, "there are some");
print "   p('there are some') = " . $p4 . "\n";

# generic n-gram probability function
$p5 = srilm::getNgramProb($n, "do more about your", 4);
print "   p('do more about your') = " . $p5 . "\n";

$p6 = srilm::getNgramProb($n, "or whatever has yet to", 5);
print "   p('or whatever has yet to') = " . $p6 . "\n\n";

# Query the LM to get the final log probability for an entire sentence.
# Note that this is  different from a n-gram probability because
  # (1) For a sentence, SRILM appends <s> and </s> to its beginning
  #     and the end respectively
  # (2) The log prob of a probability is the sum of all individual
  #     n-gram log probabilities

print "3. Sentence log probabilities and perplexities:\n";

$sprob = srilm::getSentenceProb($n, "there are some good", 4);
print "   p('there are some good') = " . $sprob . "\n";

$sprob = srilm::getSentencePpl($n,"there are some good", 4);
print "   ppl('there are some good') = " . $sprob . "\n\n";

# number of OOVs in a sentence
print "4. OOVs:\n";
$noov = srilm::numOOVs($n, "there are some foobar", 4);
print "   nOOVs('there are some foobar') = " . $noov . "\n\n";

# Query the LM to get the total log probability for the file named 'corpus'
print "5. Corpus log probabilities and perplexities:\n";
$corpus = 'test.txt';
$corpus_prob = srilm::getCorpusProb($n, $corpus);
print "   Logprob for the file " . $corpus . " = " . $corpus_prob . "\n";

# Query the LM to get the perplexity for the file named 'corpus'
$corpus_ppl = srilm::getCorpusPpl($n, $corpus);
print "   Perplexity for the file " . $corpus . " = " . $corpus_ppl . "\n";

# Free LM variable
srilm::deleteLM($n);
