//***************************************************************************
//* Copyright (c) 2011-2013 Saint-Petersburg Academic University
//* All Rights Reserved
//* See file LICENSE for details.
//****************************************************************************

#pragma once

#include <boost/test/unit_test.hpp>

#include "test_utils.hpp"
#include "omni/omni_utils.hpp"

namespace debruijn_graph {

BOOST_FIXTURE_TEST_SUITE(basic_debruijn_graph_tests, TmpFolderFixture)

BOOST_AUTO_TEST_CASE( EmptyGraphTest ) {
	Graph g(11);
	IdTrackHandler<Graph> int_ids(g);
	BOOST_CHECK_EQUAL(11u, g.k());
	BOOST_CHECK_EQUAL(0u, g.size());
}

BOOST_AUTO_TEST_CASE( OneVertexGraphTest ) {
	Graph g(11);
	IdTrackHandler<Graph> int_ids(g);
	g.AddVertex();
	BOOST_CHECK_EQUAL(2u, g.size());
	VertexId v = *(g.begin());
	VertexId rcv = g.conjugate(v);
	BOOST_CHECK(v != rcv);
	BOOST_CHECK_EQUAL(v, g.conjugate(rcv));
}

pair<vector<VertexId> , vector<EdgeId> > createGraph(Graph &graph,
		int edgeNumber) {
	vector<VertexId> v;
	vector<EdgeId> e;
	v.push_back(graph.AddVertex());
	for (int i = 0; i < edgeNumber; i++) {
		v.push_back(graph.AddVertex());
		e.push_back(
				graph.AddEdge(v[v.size() - 2], v[v.size() - 1],
						Sequence("AAAAAAAAAAAAAAAAA")));
	}
	return make_pair(v, e);
}

BOOST_AUTO_TEST_CASE( OneEdgeGraphTest ) {
	Graph g(11);
	IdTrackHandler<Graph> int_ids(g);
	pair<vector<VertexId> , vector<EdgeId> > data = createGraph(g, 1);
	BOOST_CHECK_EQUAL(1u, g.OutgoingEdgeCount(data.first[0]));
	BOOST_CHECK_EQUAL(0u, g.OutgoingEdgeCount(data.first[1]));
	BOOST_CHECK_EQUAL(data.second[0], g.GetUniqueOutgoingEdge(data.first[0]));
	BOOST_CHECK_EQUAL(g.conjugate(data.second[0]),
			g.GetUniqueOutgoingEdge(g.conjugate(data.first[1])));
	BOOST_CHECK_EQUAL(data.second[0],
			g.conjugate(g.conjugate(data.second[0])));
	BOOST_CHECK_EQUAL(!(g.EdgeNucls(data.second[0])),
			g.EdgeNucls(g.conjugate(data.second[0])));
}

/*void EdgeMethodsSimpleTest() {
	Graph g(11);
	pair<vector<VertexId> , vector<EdgeId> > data = createGraph(g, 2);
//	ASSERT_EQUAL(data.second[0], &g.GetData(data.second[0]));
	ASSERT_EQUAL(
			true,
			g.AreLinkable(data.first[0], data.first[1],
					Sequence("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA")));
	ASSERT_EQUAL(
			false,
			g.AreLinkable(data.first[0], data.first[1],
					Sequence("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC")));
}*/

BOOST_AUTO_TEST_CASE( VertexMethodsSimpleTest ) {
	Graph g(11);
	IdTrackHandler<Graph> int_ids(g);
	pair<vector<VertexId> , vector<EdgeId> > data = createGraph(g, 2);
	BOOST_CHECK_EQUAL(data.second[0], g.GetUniqueIncomingEdge(data.first[1]));
	BOOST_CHECK_EQUAL(data.second[0], g.GetUniqueOutgoingEdge(data.first[0]));
	BOOST_CHECK_EQUAL(false, g.CanCompressVertex(data.first[0]));
	BOOST_CHECK_EQUAL(true, g.CanCompressVertex(data.first[1]));
	BOOST_CHECK_EQUAL(false, g.CheckUniqueIncomingEdge(data.first[0]));
	BOOST_CHECK_EQUAL(true, g.CheckUniqueIncomingEdge(data.first[1]));
	BOOST_CHECK_EQUAL(false, g.CheckUniqueOutgoingEdge(data.first[2]));
	BOOST_CHECK_EQUAL(true, g.CheckUniqueOutgoingEdge(data.first[1]));
	BOOST_CHECK_EQUAL(true, g.IsDeadEnd(data.first[2]));
	BOOST_CHECK_EQUAL(false, g.IsDeadEnd(data.first[1]));
	BOOST_CHECK_EQUAL(true, g.IsDeadStart(data.first[0]));
	BOOST_CHECK_EQUAL(false, g.IsDeadStart(data.first[1]));
}

//void GraphMethodsSimpleTest() {
//	EdgeGraph g(11);
//	pair<vector<VertexId> , vector<EdgeId> > data = createGraph(g, 2);
//	ASSERT_EQUAL(vector<ActionHandler*> (), g.GetHandlers());
//	ActionHandler* handler = new ActionHandler();
//	g.AddActionHandler(handler);
//	vector<ActionHandler*> handlers = g.GetHandlers();
//	ASSERT_EQUAL(1u, handlers.size());
//	ASSERT_EQUAL(handler, handlers[0]);
//	g.RemoveActionHandler(handler);
//	ASSERT_EQUAL(vector<ActionHandler*> (), g.GetHandlers());
//}

BOOST_AUTO_TEST_CASE( SmartIteratorTest ) {
	Graph g(11);
	IdTrackHandler<Graph> int_ids(g);
	pair<vector<VertexId> , vector<EdgeId> > data = createGraph(g, 4);
	size_t num = 0;
	set<VertexId> visited;
//	std::less<VertexId> comp;
//	auto it = g.SmartVertexBegin(comp);
	for (auto it = g.SmartVertexBegin(); !it.IsEnd(); ++it) {
		num++;
		DEBUG( "with seq in vert" << g.VertexNucls(*it).str());
		visited.insert(*it);
	}
	BOOST_CHECK_EQUAL(num, data.first.size() * 2);
	for (size_t i = 0; i < data.first.size(); i++) {
		BOOST_CHECK(visited.find(data.first[i]) != visited.end());
		BOOST_CHECK(visited.find(g.conjugate(data.first[i])) != visited.end());
	}
}

//todo rename tests

BOOST_AUTO_TEST_CASE( TestSimpleThread ) {
	vector<string> reads = { "ACAAACCACCA" };
//	vector<string> edges = { "ACAAACCACCA" };
	AssertGraph (5, reads, reads);
}

BOOST_AUTO_TEST_CASE( TestSimpleThread2 ) {
	vector<string> reads = { "ACAAACCACCC", "AAACCACCCAC" };
	vector<string> edges = { "ACAAACCACCCAC" };
	AssertGraph (5, reads, edges);
}

BOOST_AUTO_TEST_CASE( TestSplitThread ) {
	vector<string> reads = { "ACAAACCACCA", "ACAAACAACCC" };
	vector<string> edges = { "ACAAAC", "CAAACCACCA", "CAAACAACCC" };
	AssertGraph (5, reads, edges);
}

BOOST_AUTO_TEST_CASE( TestSplitThread2 ) {
	vector<string> reads = { "ACAAACCACCA", "ACAAACAACCA" };
	vector<string> edges = { "AACCACCA", "ACAAAC", "CAAACCA", "CAAACAACCA" };
	AssertGraph (5, reads, edges);
}

BOOST_AUTO_TEST_CASE( TestBuldge ) {
	vector<string> reads = { "ACAAAACACCA", "ACAAACCACCA" };
//	vector<string> edges = { "ACAAAACACCA", "ACAAACCACCA" };
	AssertGraph (5, reads, reads);
}

BOOST_AUTO_TEST_CASE( TestCondenseSimple ) {
	vector<string> reads = { "CGAAACCAC", "CGAAAACAC", "AACCACACC", "AAACACACC" };
	vector<string> edges = { "CGAAAACACAC", "CACACC", "CGAAACCACAC" };
	AssertGraph (5, reads, edges);
}

BOOST_AUTO_TEST_CASE( TestKmerStoringIndex ) {
    vector<string> reads = { "CGAAACCAC", "CGAAAACAC", "AACCACACC", "AAACACACC" };
    CheckIndex<graph_pack<Graph, runtime_k::RtSeq>>(reads, 5);
}

BOOST_AUTO_TEST_CASE( TestKmerFreeIndex ) {
    vector<string> reads = { "CGAAACCAC", "CGAAAACAC", "AACCACACC", "AAACACACC" };
    CheckIndex<conj_graph_pack>(reads, 5);
}

//BOOST_AUTO_TEST_CASE( TestStrange ) {
//	vector<string> reads = {"TTCTGCATGGTTATGCATAACCATGCAGAA", "ACACACACTGGGGGTCCCTTTTGGGGGGGGTTTTTTTTG"};
//	typedef VectorStream<SingleRead> RawStream;
//	typedef io::RCReaderWrapper<SingleRead> Stream;
//	RawStream raw_stream(MakeReads(reads));
//	Stream read_stream(raw_stream);
//	Graph g(27);
//	EdgeIndex<28, Graph> index(g);
//
//	ConstructGraph<27, Stream>(g, index, read_stream);
//	EdgeId e = index.get(Seq<28>("TTCTGCATGGTTATGCATAACCATGCAG")).first;
//	VertexId start = g.EdgeEnd(e);
//	vector<EdgeId> edgeIds[2];
//	edgeIds[0] = g.OutgoingEdges(start);
//	edgeIds[1] = g.IncomingEdges(start);
//	for(int ii = 0; ii < 2; ii++)
//		for(auto e_iter = edgeIds[ii].begin(), end_iter = edgeIds[ii].end(); e_iter != end_iter; e_iter++) {
//			g.DeleteEdge(*e_iter);
//		}
//	g.DeleteVertex(start);
//
////		g.DeleteEdge(e);
////
////
////
////	g.DeleteEdge(r_e);
////	g.DeleteVertex(start);
//
//	INFO("FINISH");
//
////	AssertEdges(g, AddComplement(Edges(etalon_edges.begin(), etalon_edges.end())));
//
//}

BOOST_AUTO_TEST_CASE( SimpleTestEarlyPairedInfo ) {
	vector<MyPairedRead> paired_reads = {{"CCCAC", "CCACG"}, {"ACCAC", "CCACA"}};
	vector<MyEdge> edges = {"CCCA", "ACCA", "CCAC", "CACG", "CACA"};
	CoverageInfo coverage_info = {{"CCCA", 1}, {"ACCA", 1}, {"CCAC", 4}, {"CACG", 1}, {"CACA", 1}};
	EdgePairInfo edge_pair_info = {{{"CCCA", "CACG"}, {2, 1.0}}, {{"ACCA", "CACA"}, {2, 1.0}}
		, {{"CCCA", "CCAC"}, {1, 1.0}}, {{"ACCA", "CCAC"}, {1, 1.0}}
		, {{"CCAC", "CACG"}, {1, 1.0}}, {{"CCAC", "CACA"}, {1, 1.0}}};

	AssertGraph(3, paired_reads, 5, 6, edges, coverage_info, edge_pair_info);
}

BOOST_AUTO_TEST_CASE( TestSelfRCEdgeMerge ) {
	Graph g(5);
	VertexId v1 = g.AddVertex();
	VertexId v2 = g.AddVertex();
	EdgeId edge1 = g.AddEdge(v1, v2, Sequence("AACGCTATT"));
	EdgeId edge2 = g.AddEdge(v2, g.conjugate(v2), Sequence("CTATTCACGTGAATAG"));
	vector<EdgeId> path = {edge1, edge2};
	g.MergePath(path);
	BOOST_CHECK_EQUAL(2u, g.size());
	BOOST_CHECK_EQUAL(1u, g.OutgoingEdgeCount(v1));
	BOOST_CHECK_EQUAL(Sequence("AACGCTATTCACGTGAATAGCGTT"), g.EdgeNucls(g.GetUniqueOutgoingEdge(v1)));
}

BOOST_AUTO_TEST_SUITE_END()

}
