//***************************************************************************
//* Copyright (c) 2015 Saint Petersburg State University
//* Copyright (c) 2011-2014 Saint Petersburg Academic University
//* All Rights Reserved
//* See file LICENSE for details.
//***************************************************************************

#pragma once
#include <boost/test/unit_test.hpp>
#include "test_utils.hpp"
#include "longseq.hpp"
#include "basic/sequence/sequence.hpp"
#include "basic/sequence/nucl.hpp"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <string>

namespace cap {

BOOST_AUTO_TEST_SUITE(long_seq_tests)

typedef unsigned long long ull;
LSeq GenLSeq(unsigned length, const string &nucl_str, size_t pos = 0) {
  return LSeq(length, Sequence(nucl_str), pos);
}
LSeq GenLSeq(unsigned length, const LSeq &other, size_t pos = 0) {
  return LSeq(length, Sequence(other.str()), pos);
}

BOOST_AUTO_TEST_CASE( PolynomialHashTest ) {
  string s = "QWDKJUHQSDJNBASDUHASDKAJSBDASHDUIQWNJDKNQJWIDUIQDIUASHDKJANSDUQHWUIDUIFBWEIGFIHWESFH";
  vector <int> sizes = {5, 10, 15};
  vector <int> start_positions = {0, 5, 15};
  vector <int> check_positions = {20, 30, 50};

  typedef MultiPolynomialHash<2, uint64_t> Hash;

  for (auto sz_it = sizes.begin(); sz_it != sizes.end(); ++sz_it)
    for (auto st_it = start_positions.begin(); st_it != start_positions.end(); ++st_it)
      for (auto en_it = check_positions.begin(); en_it != check_positions.end(); ++en_it) {
        Hash h1(*sz_it), h2(*sz_it);
        for (int i = 0; i < *sz_it; ++i) {
          h1.Update(s[*st_it + i]);
          h2.Update(s[*en_it + i]);
        }
        for (int i = *st_it; i < *en_it; ++i) {
          h1.Update(s[i + *sz_it], s[i]);
        }
        BOOST_CHECK_EQUAL(h1, h2);
      }
}

BOOST_AUTO_TEST_CASE( TestLSeqSelector ) {
    BOOST_CHECK_EQUAL('G', nucl(GenLSeq(10, "ACGTACGTAC")[2]));
    BOOST_CHECK_EQUAL('G', nucl(GenLSeq(60, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC")[2]));
    BOOST_CHECK_EQUAL('G', nucl(GenLSeq(60, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC")[16]));
    BOOST_CHECK_EQUAL('T', nucl(GenLSeq(60, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC")[17]));
    BOOST_CHECK_EQUAL('A', nucl(GenLSeq(60, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC")[18]));
    BOOST_CHECK_EQUAL('C', nucl(GenLSeq(60, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC")[19]));

    BOOST_CHECK_EQUAL('C', nucl(GenLSeq(64, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACCT")[15]));
    BOOST_CHECK_EQUAL('G', nucl(GenLSeq(64, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACCT")[16]));
    BOOST_CHECK_EQUAL('C', nucl(GenLSeq(64, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACCT")[31]));
    BOOST_CHECK_EQUAL('G', nucl(GenLSeq(64, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACCT")[32]));
    BOOST_CHECK_EQUAL('T', nucl(GenLSeq(64, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACCT")[63]));

    BOOST_CHECK_EQUAL('C', nucl(GenLSeq(32, "ACGTACGTACACGTACGTACACGTACGTACAC")[15]));
    BOOST_CHECK_EQUAL('G', nucl(GenLSeq(32, "ACGTACGTACACGTACGTACACGTACGTACAC")[16]));
    BOOST_CHECK_EQUAL('C', nucl(GenLSeq(32, "ACGTACGTACACGTACGTACACGTACGTACAC")[31]));

    BOOST_CHECK_EQUAL('C', nucl(GenLSeq(33, "ACGTACGTACACGTACGTACACGTACGTACACC")[15]));
    BOOST_CHECK_EQUAL('G', nucl(GenLSeq(33, "ACGTACGTACACGTACGTACACGTACGTACACC")[16]));
    BOOST_CHECK_EQUAL('C', nucl(GenLSeq(33, "ACGTACGTACACGTACGTACACGTACGTACACC")[32]));

}

BOOST_AUTO_TEST_CASE( TestLSeqShiftLeft ) {
    BOOST_CHECK_EQUAL(GenLSeq(5, "ACACA"), (GenLSeq(5, "CACAC") << dignucl('A')));
    BOOST_CHECK_EQUAL(GenLSeq(5, "ACACC"), (GenLSeq(5, "CACAC") << dignucl('C')));
    BOOST_CHECK_EQUAL(GenLSeq(5, "ACACG"), (GenLSeq(5, "CACAC") << dignucl('G')));
    BOOST_CHECK_EQUAL(GenLSeq(5, "ACACT"), (GenLSeq(5, "CACAC") << dignucl('T')));

    LSeq s = GenLSeq(10, "ACGTACGTAC");
    BOOST_CHECK_EQUAL(GenLSeq(10, "CGTACGTACA"), (s << dignucl('A')));
    BOOST_CHECK_EQUAL(GenLSeq(10, "CGTACGTACC"), (s << dignucl('C')));
    BOOST_CHECK_EQUAL(GenLSeq(10, "CGTACGTACG"), (s << dignucl('G')));
    BOOST_CHECK_EQUAL(GenLSeq(10, "CGTACGTACT"), (s << dignucl('T')));

    LSeq s2 = GenLSeq(60, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC");
    BOOST_CHECK_EQUAL(GenLSeq(60, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACA"), (s2 << dignucl('A')));
    BOOST_CHECK_EQUAL(GenLSeq(60, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACC"), (s2 << dignucl('C')));
    BOOST_CHECK_EQUAL(GenLSeq(60, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACG"), (s2 << dignucl('G')));
    BOOST_CHECK_EQUAL(GenLSeq(60, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACT"), (s2 << dignucl('T')));

    LSeq s2b = GenLSeq(64, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG");
    BOOST_CHECK_EQUAL(GenLSeq(64, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGA"), (s2b << dignucl('A')));
    BOOST_CHECK_EQUAL(GenLSeq(64, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGC"), (s2b << dignucl('C')));
    BOOST_CHECK_EQUAL(GenLSeq(64, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGG"), (s2b << dignucl('G')));
    BOOST_CHECK_EQUAL(GenLSeq(64, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGT"), (s2b << dignucl('T')));

    LSeq s3 = GenLSeq(32, "ACGTACGTACACGTACGTACACGTACGTACAC");
    BOOST_CHECK_EQUAL(GenLSeq(32, "CGTACGTACACGTACGTACACGTACGTACACA"), (s3 << dignucl('A')));
    BOOST_CHECK_EQUAL(GenLSeq(32, "CGTACGTACACGTACGTACACGTACGTACACC"), (s3 << dignucl('C')));
    BOOST_CHECK_EQUAL(GenLSeq(32, "CGTACGTACACGTACGTACACGTACGTACACG"), (s3 << dignucl('G')));
    BOOST_CHECK_EQUAL(GenLSeq(32, "CGTACGTACACGTACGTACACGTACGTACACT"), s3 << dignucl('T'));

    LSeq s4 = GenLSeq(33, "TACGTACGTACACGTACGTACACGTACGTACAC");
    BOOST_CHECK_EQUAL(GenLSeq(33, "ACGTACGTACACGTACGTACACGTACGTACACA"), (s4 << dignucl('A')));
    BOOST_CHECK_EQUAL(GenLSeq(33, "ACGTACGTACACGTACGTACACGTACGTACACC"), (s4 << dignucl('C')));
    BOOST_CHECK_EQUAL(GenLSeq(33, "ACGTACGTACACGTACGTACACGTACGTACACG"), (s4 << dignucl('G')));
    BOOST_CHECK_EQUAL(GenLSeq(33, "ACGTACGTACACGTACGTACACGTACGTACACT"), s4 << dignucl('T'));
}


BOOST_AUTO_TEST_CASE( TestLSeqShiftLeftThis ) {
    LSeq s0 = GenLSeq(5, "CACACACGT");
    s0 <<= 'A';
    BOOST_CHECK_EQUAL(GenLSeq(5, "ACACA"), s0);
    s0 <<= 'C';
    BOOST_CHECK_EQUAL(GenLSeq(5, "CACAC"), s0);
    s0 <<= 'G';
    BOOST_CHECK_EQUAL(GenLSeq(5, "ACACG"), s0);
    s0 <<= 'T';
    BOOST_CHECK_EQUAL(GenLSeq(5, "CACGT"), s0);

    unsigned l;
    LSeq s = GenLSeq(10, "ACGTACGTACACGT");
    l = 10;
    s <<= 'A';
    BOOST_CHECK_EQUAL(GenLSeq(l, "CGTACGTACA"), s);
    s <<= 'C';
    BOOST_CHECK_EQUAL(GenLSeq(l, "GTACGTACAC"), s);
    s <<= 'G';
    BOOST_CHECK_EQUAL(GenLSeq(l, "TACGTACACG"), s);
    s <<= 'T';
    BOOST_CHECK_EQUAL(GenLSeq(l, "ACGTACACGT"), s);

    LSeq s2 = GenLSeq(60, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGT");
    l = 60;
    s2 <<= 'A';
    BOOST_CHECK_EQUAL(GenLSeq(l, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACA"), s2);
    s2 <<= 'C';
    BOOST_CHECK_EQUAL(GenLSeq(l, "GTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACAC"), s2);
    s2 <<= 'G';
    BOOST_CHECK_EQUAL(GenLSeq(l, "TACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACG"), s2);
    s2 <<= 'T';
    BOOST_CHECK_EQUAL(GenLSeq(l, "ACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGT"), s2);

    LSeq s2b = GenLSeq(64, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGACGT");
    l = 64;
    s2b <<= 'A';
    BOOST_CHECK_EQUAL(GenLSeq(l, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGA"), s2b);
    s2b <<= 'C';
    BOOST_CHECK_EQUAL(GenLSeq(l, "GTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGAC"), s2b);
    s2b <<= 'G';
    BOOST_CHECK_EQUAL(GenLSeq(l, "TACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGACG"), s2b);
    s2b <<= 'T';
    BOOST_CHECK_EQUAL(GenLSeq(l, "ACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGACGT"), s2b);

    LSeq s3 = GenLSeq(32, "ACGTACGTACACGTACGTACACGTACGTACACCTAG");
    l = 32;
    s3 <<= 'C';
    BOOST_CHECK_EQUAL(GenLSeq(l, "CGTACGTACACGTACGTACACGTACGTACACC"), s3);
    s3 <<= 'T';
    BOOST_CHECK_EQUAL(GenLSeq(l, "GTACGTACACGTACGTACACGTACGTACACCT"), s3);
    s3 <<= 'A';
    BOOST_CHECK_EQUAL(GenLSeq(l, "TACGTACACGTACGTACACGTACGTACACCTA"), s3);
    s3 <<= 'G';
    BOOST_CHECK_EQUAL(GenLSeq(l, "ACGTACACGTACGTACACGTACGTACACCTAG"), s3);

    LSeq s4 = GenLSeq(33, "TACGTACGTACACGTACGTACACGTACGTACACCTAG");
    l = 33;
    s4 <<= 'C';
    BOOST_CHECK_EQUAL(GenLSeq(l, "ACGTACGTACACGTACGTACACGTACGTACACC"), s4);
    s4 <<= 'T';
    BOOST_CHECK_EQUAL(GenLSeq(l, "CGTACGTACACGTACGTACACGTACGTACACCT"), s4);
    s4 <<= 'A';
    BOOST_CHECK_EQUAL(GenLSeq(l, "GTACGTACACGTACGTACACGTACGTACACCTA"), s4);
    s4 <<= 'G';
    BOOST_CHECK_EQUAL(GenLSeq(l, "TACGTACACGTACGTACACGTACGTACACCTAG"), s4);
}


BOOST_AUTO_TEST_CASE( TestLSeqShiftRight ) {
    BOOST_CHECK_EQUAL(GenLSeq(5, "ACACA"), (GenLSeq(5, "CACAC") >> dignucl('A')));
    BOOST_CHECK_EQUAL(GenLSeq(5, "CCACA"), (GenLSeq(5, "CACAC") >> dignucl('C')));
    BOOST_CHECK_EQUAL(GenLSeq(5, "GCACA"), (GenLSeq(5, "CACAC") >> dignucl('G')));
    BOOST_CHECK_EQUAL(GenLSeq(5, "TCACA"), (GenLSeq(5, "CACAC") >> dignucl('T')));

    LSeq s = GenLSeq(10, "ACGTACGTAC");
    BOOST_CHECK_EQUAL(GenLSeq(10, "AACGTACGTA"), (s >> dignucl('A')));
    BOOST_CHECK_EQUAL(GenLSeq(10, "CACGTACGTA"), (s >> dignucl('C')));
    BOOST_CHECK_EQUAL(GenLSeq(10, "GACGTACGTA"), (s >> dignucl('G')));
    BOOST_CHECK_EQUAL(GenLSeq(10, "TACGTACGTA"), (s >> dignucl('T')));

    LSeq s2 = GenLSeq(60, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC");
    BOOST_CHECK_EQUAL(GenLSeq(60, "AACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTA"), (s2 >> dignucl('A')));
    BOOST_CHECK_EQUAL(GenLSeq(60, "CACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTA"), (s2 >> dignucl('C')));
    BOOST_CHECK_EQUAL(GenLSeq(60, "GACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTA"), (s2 >> dignucl('G')));
    BOOST_CHECK_EQUAL(GenLSeq(60, "TACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTA"), (s2 >> dignucl('T')));

    LSeq s2b = GenLSeq(64, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG");
    BOOST_CHECK_EQUAL(GenLSeq(64, "AACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGC"), (s2b >> dignucl('A')));
    BOOST_CHECK_EQUAL(GenLSeq(64, "CACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGC"), (s2b >> dignucl('C')));
    BOOST_CHECK_EQUAL(GenLSeq(64, "GACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGC"), (s2b >> dignucl('G')));
    BOOST_CHECK_EQUAL(GenLSeq(64, "TACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGC"), (s2b >> dignucl('T')));

    LSeq s3 = GenLSeq(32, "ACGTACGTACACGTACGTACACGTACGTACAC");
    BOOST_CHECK_EQUAL(GenLSeq(32, "AACGTACGTACACGTACGTACACGTACGTACA"), (s3 >> dignucl('A')));
    BOOST_CHECK_EQUAL(GenLSeq(32, "CACGTACGTACACGTACGTACACGTACGTACA"), (s3 >> dignucl('C')));
    BOOST_CHECK_EQUAL(GenLSeq(32, "GACGTACGTACACGTACGTACACGTACGTACA"), (s3 >> dignucl('G')));
    BOOST_CHECK_EQUAL(GenLSeq(32, "TACGTACGTACACGTACGTACACGTACGTACA"), (s3 >> dignucl('T')));

    LSeq s4 = GenLSeq(33, "ACGTACGTACACGTACGTACACGTACGTACACT");
    BOOST_CHECK_EQUAL(GenLSeq(33, "AACGTACGTACACGTACGTACACGTACGTACAC"), (s4 >> dignucl('A')));
    BOOST_CHECK_EQUAL(GenLSeq(33, "CACGTACGTACACGTACGTACACGTACGTACAC"), (s4 >> dignucl('C')));
    BOOST_CHECK_EQUAL(GenLSeq(33, "GACGTACGTACACGTACGTACACGTACGTACAC"), (s4 >> dignucl('G')));
    BOOST_CHECK_EQUAL(GenLSeq(33, "TACGTACGTACACGTACGTACACGTACGTACAC"), (s4 >> dignucl('T')));
}


BOOST_AUTO_TEST_CASE( TestLSeqShiftRightThis ) {
    unsigned l;
    LSeq s = GenLSeq(5, "TGCACACAC", 4);
    l = 5;
    s >>= ('A');
    BOOST_CHECK_EQUAL(GenLSeq(l, "ACACA"), s);
    s >>= ('C');
    BOOST_CHECK_EQUAL(GenLSeq(l, "CACAC"), s);
    s >>= ('G');
    BOOST_CHECK_EQUAL(GenLSeq(l, "GCACA"), s);
    s >>= ('T');
    BOOST_CHECK_EQUAL(GenLSeq(l, "TGCAC"), s);


    l = 9;
    s = GenLSeq(l, "TGCACGTACGTAC", 4);
    s >>= ('A');
    BOOST_CHECK_EQUAL(GenLSeq(l, "ACGTACGTA"), s);
    s >>= ('C');
    BOOST_CHECK_EQUAL(GenLSeq(l, "CACGTACGT"), s);
    s >>= ('G');
    BOOST_CHECK_EQUAL(GenLSeq(l, "GCACGTACG"), s);
    s >>= ('T');
    BOOST_CHECK_EQUAL(GenLSeq(l, "TGCACGTAC"), s);

    l = 59;
    s = GenLSeq(l, "TGCACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC", 4);
    s >>= ('A');
    BOOST_CHECK_EQUAL(GenLSeq(l, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTA"), s);
    s >>= ('C');
    BOOST_CHECK_EQUAL(GenLSeq(l, "CACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGT"), s);
    s >>= ('G');
    BOOST_CHECK_EQUAL(GenLSeq(l, "GCACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACG"), s);
    s >>= ('T');
    BOOST_CHECK_EQUAL(GenLSeq(l, "TGCACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTAC"), s);

    l = 63;
    s = GenLSeq(l, "TGCACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG", 4);
    s >>= ('A');
    BOOST_CHECK_EQUAL(GenLSeq(l, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGC"), s);
    s >>= ('C');
    BOOST_CHECK_EQUAL(GenLSeq(l, "CACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCG"), s);
    s >>= ('G');
    BOOST_CHECK_EQUAL(GenLSeq(l, "GCACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACC"), s);
    s >>= ('T');
    BOOST_CHECK_EQUAL(GenLSeq(l, "TGCACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC"), s);


    l = 31;
    s = GenLSeq(l, "TGCACGTACGTACACGTACGTACACGTACGTACAC", 4);
    s >>= ('A');
    BOOST_CHECK_EQUAL(GenLSeq(l, "ACGTACGTACACGTACGTACACGTACGTACA"), s);
    s >>= ('C');
    BOOST_CHECK_EQUAL(GenLSeq(l, "CACGTACGTACACGTACGTACACGTACGTAC"), s);
    s >>= ('G');
    BOOST_CHECK_EQUAL(GenLSeq(l, "GCACGTACGTACACGTACGTACACGTACGTA"), s);
    s >>= ('T');
    BOOST_CHECK_EQUAL(GenLSeq(l, "TGCACGTACGTACACGTACGTACACGTACGT"), s);

    l = 32;
    s = GenLSeq(l, "TGCAACGTACGTACACGTACGTACACGTACGTACAC", 4);
    s >>= ('A');
    BOOST_CHECK_EQUAL(GenLSeq(l, "AACGTACGTACACGTACGTACACGTACGTACA"), s);
    s >>= ('C');
    BOOST_CHECK_EQUAL(GenLSeq(l, "CAACGTACGTACACGTACGTACACGTACGTAC"), s);
    s >>= ('G');
    BOOST_CHECK_EQUAL(GenLSeq(l, "GCAACGTACGTACACGTACGTACACGTACGTA"), s);
    s >>= ('T');
    BOOST_CHECK_EQUAL(GenLSeq(l, "TGCAACGTACGTACACGTACGTACACGTACGT"), s);

    l = 33;
    s = GenLSeq(l, "TGCACACGTACGTACACGTACGTACACGTACGTACAC", 4);
    s >>= ('A');
    BOOST_CHECK_EQUAL(GenLSeq(l, "ACACGTACGTACACGTACGTACACGTACGTACA"), s);
    s >>= ('C');
    BOOST_CHECK_EQUAL(GenLSeq(l, "CACACGTACGTACACGTACGTACACGTACGTAC"), s);
    s >>= ('G');
    BOOST_CHECK_EQUAL(GenLSeq(l, "GCACACGTACGTACACGTACGTACACGTACGTA"), s);
    s >>= ('T');
    BOOST_CHECK_EQUAL(GenLSeq(l, "TGCACACGTACGTACACGTACGTACACGTACGT"), s);
}



BOOST_AUTO_TEST_CASE( TestLSeqStr ) {
    LSeq s = GenLSeq(10, "ACGTACGTAC");
    BOOST_CHECK_EQUAL("ACGTACGTAC", s.str());
    LSeq s2 = GenLSeq(60, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC");
    BOOST_CHECK_EQUAL("ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC", s2.str());
    LSeq s2b = GenLSeq(64, "CCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG");
    BOOST_CHECK_EQUAL("CCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG", s2b.str());
    LSeq s3 = GenLSeq(32, "TCGTACGTACACGTACGTACACGTACGTACAC");
    BOOST_CHECK_EQUAL("TCGTACGTACACGTACGTACACGTACGTACAC", s3.str());
    LSeq s4 = GenLSeq(33, "ACGTACGTACACGTACGTACACGTACGTACACT");
    BOOST_CHECK_EQUAL("ACGTACGTACACGTACGTACACGTACGTACACT", s4.str());
}

BOOST_AUTO_TEST_CASE( TestLSeqHeadAndTail ) {
    unsigned l;

    l = 5;
    LSeq s = GenLSeq(l, "GCATC");
    BOOST_CHECK_EQUAL(GenLSeq(l - 1, "CATC"), GenLSeq(l - 1, s, 1)); // tail
    BOOST_CHECK_EQUAL(GenLSeq(l - 1, "GCAT"), GenLSeq(l - 1, s)); // head

    l = 10;
    LSeq s1 = GenLSeq(l, "CCGTACGTAC");
    BOOST_CHECK_EQUAL(GenLSeq(l - 1, "CGTACGTAC"), GenLSeq(l - 1, s1, 1)); // tail
    BOOST_CHECK_EQUAL(GenLSeq(l - 1, "CCGTACGTA"), GenLSeq(l - 1, s1)); // head

    l = 60;
    LSeq s2 = GenLSeq(l, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC");
    BOOST_CHECK_EQUAL(GenLSeq(l - 1, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC"), GenLSeq(l - 1, s2, 1)); // tail
    BOOST_CHECK_EQUAL(GenLSeq(l - 1, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTA"), GenLSeq(l - 1, s2)); // head

    l = 64;
    LSeq s2b = GenLSeq(l, "CCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG");
    BOOST_CHECK_EQUAL(GenLSeq(l - 1, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG"), GenLSeq(l - 1, s2b, 1)); // tail
    BOOST_CHECK_EQUAL(GenLSeq(l - 1, "CCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGC"), GenLSeq(l - 1, s2b)); // head

    l = 32;
    LSeq s3 = GenLSeq(l, "TCGTACGTACACGTACGTACACGTACGTACAC");
    BOOST_CHECK_EQUAL(GenLSeq(l - 1, "CGTACGTACACGTACGTACACGTACGTACAC"), GenLSeq(l - 1, s3, 1)); // tail
    BOOST_CHECK_EQUAL(GenLSeq(l - 1, "TCGTACGTACACGTACGTACACGTACGTACA"), GenLSeq(l - 1, s3)); // head

    l = 33;
    LSeq s4 = GenLSeq(l, "GCGTACGTACACGTACGTACACGTACGTACACT");
    BOOST_CHECK_EQUAL(GenLSeq(l - 1, "CGTACGTACACGTACGTACACGTACGTACACT"), GenLSeq(l - 1, s4, 1)); // tail
    BOOST_CHECK_EQUAL(GenLSeq(l - 1, "GCGTACGTACACGTACGTACACGTACGTACAC"), GenLSeq(l - 1, s4)); // head
}

BOOST_AUTO_TEST_CASE( TestLSeqFromBiggerLSeq ) {
    LSeq s = GenLSeq(64, "CCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG");
    BOOST_CHECK_EQUAL("CCGTA", GenLSeq(5, s).str());
    BOOST_CHECK_EQUAL("CCGTACGTAC", GenLSeq(10, s).str());
    BOOST_CHECK_EQUAL("CCGTACGTACACGTAC", GenLSeq(16, s).str());
    BOOST_CHECK_EQUAL("CCGTACGTACACGTACG", GenLSeq(17, s).str());
    BOOST_CHECK_EQUAL("CCGTACGTACACGTACGTACACGTACGTACAC", GenLSeq(32, s).str());
    BOOST_CHECK_EQUAL("CCGTACGTACACGTACGTACACGTACGTACACG", GenLSeq(33, s).str());
    BOOST_CHECK_EQUAL("CCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC", GenLSeq(60, s).str());
    BOOST_CHECK_EQUAL("CCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG", GenLSeq(64, s).str());
}

/*
BOOST_AUTO_TEST_CASE( TestLSeqFromSeq ) {
    Seq<5> s("CGTAC");
    Seq<10> s1("ACGTACGTAC");
    Seq<16> s2("CCCCGTACGTACGTAC");
    Seq<32> s3("ACGTACGTACACGTACGTACACGTACGTACAC");
    Seq<33> s4("GACGTACGTACACGTACGTACACGTACGTACAC");
    Seq<60> s5("ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC");
    Seq<64> s6("CCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG");

    BOOST_CHECK_EQUAL(GenLSeq(s, true).str(), s.str());
    BOOST_CHECK_EQUAL(GenLSeq(s1, true).str(), s1.str());
    BOOST_CHECK_EQUAL(GenLSeq(s2, true).str(), s2.str());
    BOOST_CHECK_EQUAL(GenLSeq(s3, true).str(), s3.str());
    BOOST_CHECK_EQUAL(GenLSeq(s4, true).str(), s4.str());
    BOOST_CHECK_EQUAL(GenLSeq(s5, true).str(), s5.str());
    BOOST_CHECK_EQUAL(GenLSeq(s6, true).str(), s6.str());
}

BOOST_AUTO_TEST_CASE( TestLSeqToSeq ) {
    Seq<5> s("CGTAC");
    Seq<10> s1("ACGTACGTAC");
    Seq<16> s2("CCCCGTACGTACGTAC");
    Seq<32> s3("ACGTACGTACACGTACGTACACGTACGTACAC");
    Seq<33> s4("GACGTACGTACACGTACGTACACGTACGTACAC");
    Seq<60> s5("ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC");
    Seq<64> s6("CCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG");

    BOOST_CHECK_EQUAL(GenLSeq(5, "CGTAC").get_seq<5>(), s);
    BOOST_CHECK_EQUAL(GenLSeq(10, "ACGTACGTAC").get_seq<10>(), s1);
    BOOST_CHECK_EQUAL(GenLSeq(16, "CCCCGTACGTACGTAC").get_seq<16>(), s2);
    BOOST_CHECK_EQUAL(GenLSeq(32, "ACGTACGTACACGTACGTACACGTACGTACAC").get_seq<32>(), s3);
    BOOST_CHECK_EQUAL(GenLSeq(33, "GACGTACGTACACGTACGTACACGTACGTACAC").get_seq<33>(), s4);
    BOOST_CHECK_EQUAL(GenLSeq(60, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC").get_seq<60>(), s5);
    BOOST_CHECK_EQUAL(GenLSeq(64, "CCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG").get_seq<64>(), s6);

}

BOOST_AUTO_TEST_CASE( TestLSeqFromType ) {
    Sequence s("CCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG");
    BOOST_CHECK_EQUAL("CCGTA", GenLSeq(5, s).str());
    BOOST_CHECK_EQUAL("CCGTACGTAC", GenLSeq(10, s).str());
    BOOST_CHECK_EQUAL("CCGTACGTACACGTAC", GenLSeq(16, s).str());
    BOOST_CHECK_EQUAL("CCGTACGTACACGTACG", GenLSeq(17, s).str());
    BOOST_CHECK_EQUAL("CCGTACGTACACGTACGTACACGTACGTACAC", GenLSeq(32, s).str());
    BOOST_CHECK_EQUAL("CCGTACGTACACGTACGTACACGTACGTACACG", GenLSeq(33, s).str());
    BOOST_CHECK_EQUAL("CCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC", GenLSeq(60, s).str());
    BOOST_CHECK_EQUAL("CCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG", GenLSeq(64, s).str());

    BOOST_CHECK_EQUAL("GTA", GenLSeq(3, s, 2).str());
    BOOST_CHECK_EQUAL("GTACGTACAC", GenLSeq(10, s, 2).str());
    BOOST_CHECK_EQUAL("GTACGTACACGTACGT", GenLSeq(16, s, 2).str());
    BOOST_CHECK_EQUAL("GTACGTACACGTACGTA", GenLSeq(17, s, 2).str());
    BOOST_CHECK_EQUAL("GTACGTACACGTACGTACACGTACGTACACGT", GenLSeq(32, s, 2).str());
    BOOST_CHECK_EQUAL("GTACGTACACGTACGTACACGTACGTACACGTA", GenLSeq(33, s, 2).str());
    BOOST_CHECK_EQUAL("GTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC", GenLSeq(58, s, 2).str());
    BOOST_CHECK_EQUAL("GTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG", GenLSeq(62, s, 2).str());
}

*/

BOOST_AUTO_TEST_CASE( TestLSeqPushBack ) {
    BOOST_CHECK_EQUAL(GenLSeq(6, "CACACA"), (GenLSeq(5, "CACAC").pushBack(dignucl('A'))));
    BOOST_CHECK_EQUAL(GenLSeq(6, "CACACC"), (GenLSeq(5, "CACAC").pushBack(dignucl('C'))));
    BOOST_CHECK_EQUAL(GenLSeq(6, "CACACG"), (GenLSeq(5, "CACAC").pushBack(dignucl('G'))));
    BOOST_CHECK_EQUAL(GenLSeq(6, "CACACT"), (GenLSeq(5, "CACAC").pushBack(dignucl('T'))));

    LSeq s = GenLSeq(9, "CGTACGTAC");
    BOOST_CHECK_EQUAL(GenLSeq(10, "CGTACGTACA"), (s.pushBack(dignucl('A'))));
    BOOST_CHECK_EQUAL(GenLSeq(10, "CGTACGTACC"), (s.pushBack(dignucl('C'))));
    BOOST_CHECK_EQUAL(GenLSeq(10, "CGTACGTACG"), (s.pushBack(dignucl('G'))));
    BOOST_CHECK_EQUAL(GenLSeq(10, "CGTACGTACT"), (s.pushBack(dignucl('T'))));

    LSeq s2 = GenLSeq(59, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC");
    BOOST_CHECK_EQUAL(GenLSeq(60, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACA"), (s2.pushBack(dignucl('A'))));
    BOOST_CHECK_EQUAL(GenLSeq(60, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACC"), (s2.pushBack(dignucl('C'))));
    BOOST_CHECK_EQUAL(GenLSeq(60, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACG"), (s2.pushBack(dignucl('G'))));
    BOOST_CHECK_EQUAL(GenLSeq(60, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACT"), (s2.pushBack(dignucl('T'))));

    LSeq s2b = GenLSeq(63, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG");
    BOOST_CHECK_EQUAL(GenLSeq(64, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGA"), (s2b.pushBack(dignucl('A'))));
    BOOST_CHECK_EQUAL(GenLSeq(64, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGC"), (s2b.pushBack(dignucl('C'))));
    BOOST_CHECK_EQUAL(GenLSeq(64, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGG"), (s2b.pushBack(dignucl('G'))));
    BOOST_CHECK_EQUAL(GenLSeq(64, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGT"), (s2b.pushBack(dignucl('T'))));

    LSeq s3 = GenLSeq(31, "CGTACGTACACGTACGTACACGTACGTACAC");
    BOOST_CHECK_EQUAL(GenLSeq(32, "CGTACGTACACGTACGTACACGTACGTACACA"), (s3.pushBack(dignucl('A'))));
    BOOST_CHECK_EQUAL(GenLSeq(32, "CGTACGTACACGTACGTACACGTACGTACACC"), (s3.pushBack(dignucl('C'))));
    BOOST_CHECK_EQUAL(GenLSeq(32, "CGTACGTACACGTACGTACACGTACGTACACG"), (s3.pushBack(dignucl('G'))));
    BOOST_CHECK_EQUAL(GenLSeq(32, "CGTACGTACACGTACGTACACGTACGTACACT"), (s3.pushBack(dignucl('T'))));

    LSeq s4 = GenLSeq(32, "ACGTACGTACACGTACGTACACGTACGTACAC");
    BOOST_CHECK_EQUAL(GenLSeq(33, "ACGTACGTACACGTACGTACACGTACGTACACA"), (s4.pushBack(dignucl('A'))));
    BOOST_CHECK_EQUAL(GenLSeq(33, "ACGTACGTACACGTACGTACACGTACGTACACC"), (s4.pushBack(dignucl('C'))));
    BOOST_CHECK_EQUAL(GenLSeq(33, "ACGTACGTACACGTACGTACACGTACGTACACG"), (s4.pushBack(dignucl('G'))));
    BOOST_CHECK_EQUAL(GenLSeq(33, "ACGTACGTACACGTACGTACACGTACGTACACT"), (s4.pushBack(dignucl('T'))));

    LSeq s5 = GenLSeq(33, "CACGTACGTACACGTACGTACACGTACGTACAC");
    BOOST_CHECK_EQUAL(GenLSeq(34, "CACGTACGTACACGTACGTACACGTACGTACACA"), (s5.pushBack(dignucl('A'))));
    BOOST_CHECK_EQUAL(GenLSeq(34, "CACGTACGTACACGTACGTACACGTACGTACACC"), (s5.pushBack(dignucl('C'))));
    BOOST_CHECK_EQUAL(GenLSeq(34, "CACGTACGTACACGTACGTACACGTACGTACACG"), (s5.pushBack(dignucl('G'))));
    BOOST_CHECK_EQUAL(GenLSeq(34, "CACGTACGTACACGTACGTACACGTACGTACACT"), (s5.pushBack(dignucl('T'))));
}


/*
BOOST_AUTO_TEST_CASE( TestLSeqPushBackThis ) {
    size_t l;
    LSeq s = GenLSeq(5, "CACACACGT");
    l = 5;
    s.pushBackThis('A');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "CACACA"), s);
    s.pushBackThis('C');
    BOOST_CHECK_EQUAL(GenLSeq(l + 2, "CACACAC"), s);
    s.pushBackThis('G');
    BOOST_CHECK_EQUAL(GenLSeq(l + 3, "CACACACG"), s);
    s.pushBackThis('T');
    BOOST_CHECK_EQUAL(GenLSeq(l + 4, "CACACACGT"), s);


    l = 9;
    s = GenLSeq(l, "CGTACGTACACGT");
    s.pushBackThis('A');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "CGTACGTACA"), s);
    s.pushBackThis('C');
    BOOST_CHECK_EQUAL(GenLSeq(l + 2, "CGTACGTACAC"), s);
    s.pushBackThis('G');
    BOOST_CHECK_EQUAL(GenLSeq(l + 3, "CGTACGTACACG"), s);
    s.pushBackThis('T');
    BOOST_CHECK_EQUAL(GenLSeq(l + 4, "CGTACGTACACGT"), s);

    l = 59;
    s = GenLSeq(l, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGT");
    s.pushBackThis('A');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACA"), s);
    s.pushBackThis('C');
    BOOST_CHECK_EQUAL(GenLSeq(l + 2, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACAC"), s);
    s.pushBackThis('G');
    BOOST_CHECK_EQUAL(GenLSeq(l + 3, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACG"), s);
    s.pushBackThis('T');
    BOOST_CHECK_EQUAL(GenLSeq(l + 4, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGT"), s);

    l = 63;
    s = GenLSeq(l, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGACGT");
    s.pushBackThis('A');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGA"), s);
    s = GenLSeq(l, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG");
    s.pushBackThis('C');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGC"), s);
    s = GenLSeq(l, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG");
    s.pushBackThis('G');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGG"), s);
    s = GenLSeq(l, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG");
    s.pushBackThis('T');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCGT"), s);


    l = 31;
    s = GenLSeq(l, "CGTACGTACACGTACGTACACGTACGTACACACGT");
    s.pushBackThis('A');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "CGTACGTACACGTACGTACACGTACGTACACA"), s);
    s.pushBackThis('C');
    BOOST_CHECK_EQUAL(GenLSeq(l + 2, "CGTACGTACACGTACGTACACGTACGTACACAC"), s);
    s.pushBackThis('G');
    BOOST_CHECK_EQUAL(GenLSeq(l + 3, "CGTACGTACACGTACGTACACGTACGTACACACG"), s);
    s.pushBackThis('T');
    BOOST_CHECK_EQUAL(GenLSeq(l + 4, "CGTACGTACACGTACGTACACGTACGTACACACGT"), s);

    l = 32;
    s = GenLSeq(l, "ACGTACGTACACGTACGTACACGTACGTACACACGT");
    s.pushBackThis('A');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "ACGTACGTACACGTACGTACACGTACGTACACA"), s);
    s.pushBackThis('C');
    BOOST_CHECK_EQUAL(GenLSeq(l + 2, "ACGTACGTACACGTACGTACACGTACGTACACAC"), s);
    s.pushBackThis('G');
    BOOST_CHECK_EQUAL(GenLSeq(l + 3, "ACGTACGTACACGTACGTACACGTACGTACACACG"), s);
    s.pushBackThis('T');
    BOOST_CHECK_EQUAL(GenLSeq(l + 4, "ACGTACGTACACGTACGTACACGTACGTACACACGT"), s);

    l = 33;
    s = GenLSeq(l, "CACGTACGTACACGTACGTACACGTACGTACACACGT");
    s.pushBackThis('A');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "CACGTACGTACACGTACGTACACGTACGTACACA"), s);
    s.pushBackThis('C');
    BOOST_CHECK_EQUAL(GenLSeq(l + 2, "CACGTACGTACACGTACGTACACGTACGTACACAC"), s);
    s.pushBackThis('G');
    BOOST_CHECK_EQUAL(GenLSeq(l + 3, "CACGTACGTACACGTACGTACACGTACGTACACACG"), s);
    s.pushBackThis('T');
    BOOST_CHECK_EQUAL(GenLSeq(l + 4, "CACGTACGTACACGTACGTACACGTACGTACACACGT"), s);
}
*/


BOOST_AUTO_TEST_CASE( TestLSeqPushFront ) {
    BOOST_CHECK_EQUAL(GenLSeq(6, "ACACAC"), (GenLSeq(5, "CACAC").pushFront(dignucl('A'))));
    BOOST_CHECK_EQUAL(GenLSeq(6, "CCACAC"), (GenLSeq(5, "CACAC").pushFront(dignucl('C'))));
    BOOST_CHECK_EQUAL(GenLSeq(6, "GCACAC"), (GenLSeq(5, "CACAC").pushFront(dignucl('G'))));
    BOOST_CHECK_EQUAL(GenLSeq(6, "TCACAC"), (GenLSeq(5, "CACAC").pushFront(dignucl('T'))));

    LSeq s = GenLSeq(9, "CGTACGTAC");
    BOOST_CHECK_EQUAL(GenLSeq(10, "ACGTACGTAC"), (s.pushFront(dignucl('A'))));
    BOOST_CHECK_EQUAL(GenLSeq(10, "CCGTACGTAC"), (s.pushFront(dignucl('C'))));
    BOOST_CHECK_EQUAL(GenLSeq(10, "GCGTACGTAC"), (s.pushFront(dignucl('G'))));
    BOOST_CHECK_EQUAL(GenLSeq(10, "TCGTACGTAC"), (s.pushFront(dignucl('T'))));

    LSeq s2 = GenLSeq(59, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC");
    BOOST_CHECK_EQUAL(GenLSeq(60, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC"), (s2.pushFront(dignucl('A'))));
    BOOST_CHECK_EQUAL(GenLSeq(60, "CCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC"), (s2.pushFront(dignucl('C'))));
    BOOST_CHECK_EQUAL(GenLSeq(60, "GCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC"), (s2.pushFront(dignucl('G'))));
    BOOST_CHECK_EQUAL(GenLSeq(60, "TCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC"), (s2.pushFront(dignucl('T'))));

    LSeq s2b = GenLSeq(63, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG");
    BOOST_CHECK_EQUAL(GenLSeq(64, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG"), (s2b.pushFront(dignucl('A'))));
    BOOST_CHECK_EQUAL(GenLSeq(64, "CCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG"), (s2b.pushFront(dignucl('C'))));
    BOOST_CHECK_EQUAL(GenLSeq(64, "GCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG"), (s2b.pushFront(dignucl('G'))));
    BOOST_CHECK_EQUAL(GenLSeq(64, "TCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG"), (s2b.pushFront(dignucl('T'))));

    LSeq s3 = GenLSeq(31, "CGTACGTACACGTACGTACACGTACGTACAC");
    BOOST_CHECK_EQUAL(GenLSeq(32, "ACGTACGTACACGTACGTACACGTACGTACAC"), (s3.pushFront(dignucl('A'))));
    BOOST_CHECK_EQUAL(GenLSeq(32, "CCGTACGTACACGTACGTACACGTACGTACAC"), (s3.pushFront(dignucl('C'))));
    BOOST_CHECK_EQUAL(GenLSeq(32, "GCGTACGTACACGTACGTACACGTACGTACAC"), (s3.pushFront(dignucl('G'))));
    BOOST_CHECK_EQUAL(GenLSeq(32, "TCGTACGTACACGTACGTACACGTACGTACAC"), (s3.pushFront(dignucl('T'))));

    LSeq s4 = GenLSeq(32, "ACGTACGTACACGTACGTACACGTACGTACAC");
    BOOST_CHECK_EQUAL(GenLSeq(33, "AACGTACGTACACGTACGTACACGTACGTACAC"), (s4.pushFront(dignucl('A'))));
    BOOST_CHECK_EQUAL(GenLSeq(33, "CACGTACGTACACGTACGTACACGTACGTACAC"), (s4.pushFront(dignucl('C'))));
    BOOST_CHECK_EQUAL(GenLSeq(33, "GACGTACGTACACGTACGTACACGTACGTACAC"), (s4.pushFront(dignucl('G'))));
    BOOST_CHECK_EQUAL(GenLSeq(33, "TACGTACGTACACGTACGTACACGTACGTACAC"), (s4.pushFront(dignucl('T'))));

    LSeq s5 = GenLSeq(33, "CACGTACGTACACGTACGTACACGTACGTACAC");
    BOOST_CHECK_EQUAL(GenLSeq(34, "ACACGTACGTACACGTACGTACACGTACGTACAC"), (s5.pushFront(dignucl('A'))));
    BOOST_CHECK_EQUAL(GenLSeq(34, "CCACGTACGTACACGTACGTACACGTACGTACAC"), (s5.pushFront(dignucl('C'))));
    BOOST_CHECK_EQUAL(GenLSeq(34, "GCACGTACGTACACGTACGTACACGTACGTACAC"), (s5.pushFront(dignucl('G'))));
    BOOST_CHECK_EQUAL(GenLSeq(34, "TCACGTACGTACACGTACGTACACGTACGTACAC"), (s5.pushFront(dignucl('T'))));
}


/*
BOOST_AUTO_TEST_CASE( TestLSeqPushFrontThis ) {
    size_t l;
    LSeq s = GenLSeq(5, "TGCACACAC", 4);
    l = 5;
    s.pushFrontThis('A');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "ACACAC"), s);
    s.pushFrontThis('C');
    BOOST_CHECK_EQUAL(GenLSeq(l + 2, "CACACAC"), s);
    s.pushFrontThis('G');
    BOOST_CHECK_EQUAL(GenLSeq(l + 3, "GCACACAC"), s);
    s.pushFrontThis('T');
    BOOST_CHECK_EQUAL(GenLSeq(l + 4, "TGCACACAC"), s);


    l = 9;
    s = GenLSeq(l, "TGCACGTACGTAC", 4);
    s.pushFrontThis('A');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "ACGTACGTAC"), s);
    s.pushFrontThis('C');
    BOOST_CHECK_EQUAL(GenLSeq(l + 2, "CACGTACGTAC"), s);
    s.pushFrontThis('G');
    BOOST_CHECK_EQUAL(GenLSeq(l + 3, "GCACGTACGTAC"), s);
    s.pushFrontThis('T');
    BOOST_CHECK_EQUAL(GenLSeq(l + 4, "TGCACGTACGTAC"), s);

    l = 59;
    s = GenLSeq(l, "TGCACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC", 4);
    s.pushFrontThis('A');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC"), s);
    s.pushFrontThis('C');
    BOOST_CHECK_EQUAL(GenLSeq(l + 2, "CACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC"), s);
    s.pushFrontThis('G');
    BOOST_CHECK_EQUAL(GenLSeq(l + 3, "GCACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC"), s);
    s.pushFrontThis('T');
    BOOST_CHECK_EQUAL(GenLSeq(l + 4, "TGCACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC"), s);

    l = 63;
    s = GenLSeq(l, "TGCACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG", 4);
    s.pushFrontThis('A');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG"), s);
    s = GenLSeq(l, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG");
    s.pushFrontThis('C');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "CCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG"), s);
    s = GenLSeq(l, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG");
    s.pushFrontThis('G');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "GCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG"), s);
    s = GenLSeq(l, "CGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG");
    s.pushFrontThis('T');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "TCGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACCGCG"), s);


    l = 31;
    s = GenLSeq(l, "TGCACGTACGTACACGTACGTACACGTACGTACAC", 4);
    s.pushFrontThis('A');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "ACGTACGTACACGTACGTACACGTACGTACAC"), s);
    s.pushFrontThis('C');
    BOOST_CHECK_EQUAL(GenLSeq(l + 2, "CACGTACGTACACGTACGTACACGTACGTACAC"), s);
    s.pushFrontThis('G');
    BOOST_CHECK_EQUAL(GenLSeq(l + 3, "GCACGTACGTACACGTACGTACACGTACGTACAC"), s);
    s.pushFrontThis('T');
    BOOST_CHECK_EQUAL(GenLSeq(l + 4, "TGCACGTACGTACACGTACGTACACGTACGTACAC"), s);

    l = 32;
    s = GenLSeq(l, "TGCAACGTACGTACACGTACGTACACGTACGTACAC", 4);
    s.pushFrontThis('A');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "AACGTACGTACACGTACGTACACGTACGTACAC"), s);
    s.pushFrontThis('C');
    BOOST_CHECK_EQUAL(GenLSeq(l + 2, "CAACGTACGTACACGTACGTACACGTACGTACAC"), s);
    s.pushFrontThis('G');
    BOOST_CHECK_EQUAL(GenLSeq(l + 3, "GCAACGTACGTACACGTACGTACACGTACGTACAC"), s);
    s.pushFrontThis('T');
    BOOST_CHECK_EQUAL(GenLSeq(l + 4, "TGCAACGTACGTACACGTACGTACACGTACGTACAC"), s);

    l = 33;
    s = GenLSeq(l, "TGCACACGTACGTACACGTACGTACACGTACGTACAC", 4);
    s.pushFrontThis('A');
    BOOST_CHECK_EQUAL(GenLSeq(l + 1, "ACACGTACGTACACGTACGTACACGTACGTACAC"), s);
    s.pushFrontThis('C');
    BOOST_CHECK_EQUAL(GenLSeq(l + 2, "CACACGTACGTACACGTACGTACACGTACGTACAC"), s);
    s.pushFrontThis('G');
    BOOST_CHECK_EQUAL(GenLSeq(l + 3, "GCACACGTACGTACACGTACGTACACGTACGTACAC"), s);
    s.pushFrontThis('T');
    BOOST_CHECK_EQUAL(GenLSeq(l + 4, "TGCACACGTACGTACACGTACGTACACGTACGTACAC"), s);
}
*/

BOOST_AUTO_TEST_CASE( TestLSeqNull ) {
    LSeq s = GenLSeq(0,"");
    BOOST_CHECK_EQUAL("", s.str());
}


//BOOST_AUTO_TEST_CASE( TestLSeqAddSymbolForNullValue ) {
//    LSeq s1 = GenLSeq(1, "G");
//    LSeq s2 = (s1 << 'A');
//    LSeq s3 = GenLSeq(1, "A");
//    BOOST_CHECK_EQUAL(s3, s2);
//}


BOOST_AUTO_TEST_CASE( TestLSeqComplex ) {
    Sequence s1("ACAAA");
    Sequence s2("CAAAC");
    BOOST_CHECK_EQUAL((!(LSeq(4, !s1))).str(), LSeq(4, s2).str());
    BOOST_CHECK_EQUAL(!(LSeq(4, !s1)), LSeq(4, s2));
}

BOOST_AUTO_TEST_CASE( TestLSeqFromCharArray ) {
    std::string s = "ACGTACGTAC";
    BOOST_CHECK_EQUAL("ACGTACGTAC", GenLSeq(10, s.c_str()).str());
}

BOOST_AUTO_TEST_CASE( TestLSeqReverseComplement ) {
    LSeq s = GenLSeq(10, "ACGTACGTAC");
    BOOST_CHECK_EQUAL("GTACGTACGT", (!s).str());
    LSeq s1 = GenLSeq(9, "CGTACGTAC");
    BOOST_CHECK_EQUAL("GTACGTACG", (!s1).str());
    LSeq s2 = GenLSeq(60, "ACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC");
    BOOST_CHECK_EQUAL("GTACGTACGTGTACGTACGTGTACGTACGTGTACGTACGTGTACGTACGTGTACGTACGT", (!s2).str());
    LSeq s2b = GenLSeq(64, "TGCAACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTACACGTACGTAC");
    BOOST_CHECK_EQUAL("GTACGTACGTGTACGTACGTGTACGTACGTGTACGTACGTGTACGTACGTGTACGTACGTTGCA", (!s2b).str());
    LSeq s3 = GenLSeq(32, "CGTACGTACACGTACGTACACGTACGTACACG");
    BOOST_CHECK_EQUAL("CGTGTACGTACGTGTACGTACGTGTACGTACG", (!s3).str());
    LSeq s4 = GenLSeq(33, "ACGTACGTACACGTACGTACACGTACGTACACG");
    BOOST_CHECK_EQUAL("CGTGTACGTACGTGTACGTACGTGTACGTACGT", (!s4).str());
}

BOOST_AUTO_TEST_CASE( TestLSeq16 ) {
    LSeq s = GenLSeq(16, "AAAAAAAAAAAAAAAA");
    BOOST_CHECK_EQUAL(s << 'C', GenLSeq(16, "AAAAAAAAAAAAAAAC"));
}

BOOST_AUTO_TEST_CASE( TestLSeq16_2 ) {
    LSeq s = GenLSeq(16, "TTTTTTTTTTTTTTTT");
    BOOST_CHECK_EQUAL(GenLSeq(16, "TTTTTTTTTTTTTTTA"), s << 'A');
}

/*
BOOST_AUTO_TEST_CASE( TestLSeqFirstLast ) {
    LSeq s1 = GenLSeq(7, "ACGTACT");
    BOOST_CHECK_EQUAL(0, s1.first());
    BOOST_CHECK_EQUAL(3, s1.last());
    LSeq s2 = GenLSeq(7, "TTTTTTT");
    BOOST_CHECK_EQUAL(3, s2.first());
    BOOST_CHECK_EQUAL(3, s2.last());
}
*/

BOOST_AUTO_TEST_SUITE_END()

}
