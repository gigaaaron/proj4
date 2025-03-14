#include <gtest/gtest.h>
#include "DijkstraPathRouter.h"

TEST(DijkstraPathRouter, VertexMethods) {
    CDijkstraPathRouter Router;
    
    auto v1 = Router.AddVertex(1);
    auto v2 = Router.AddVertex(2);

    EXPECT_EQ(Router.VertexCount(), 2);
    EXPECT_EQ(Router.GetVertexTag(v1), 1);
    EXPECT_EQ(Router.GetVertexTag(v2), 2);
}

TEST(DijkstraPathRouter, AddEdge) {
    CDijkstraPathRouter Router;
    
    auto v1 = Router.AddVertex(1);
    auto v2 = Router.AddVertex(2);

    EXPECT_TRUE(Router.AddEdge(v1, v2, 1.0, false));

    auto v1_edges = Router.DImplementation->vertices[v1]->edges;
    auto v2_edges = Router.DImplementation->vertices[v2]->edges;

    EXPECT_EQ(v1_edges.size(), 1);
    EXPECT_EQ(v2_edges.size(), 0);

    EXPECT_EQ(v1_edges[v2], 1.0);

    EXPECT_TRUE(Router.AddEdge(v2, v1, 2.0, true));

    v1_edges = Router.DImplementation->vertices[v1]->edges;
    v2_edges = Router.DImplementation->vertices[v2]->edges;

    EXPECT_EQ(v1_edges.size(), 1);
    EXPECT_EQ(v2_edges.size(), 1);
}

TEST(DijkstraPathRouter, FindShortestPath) {
    CDijkstraPathRouter Router;
    
    auto v1 = Router.AddVertex(1);
    auto v2 = Router.AddVertex(2);
    auto v3 = Router.AddVertex(3);
    auto v4 = Router.AddVertex(4);

    Router.AddEdge(v1, v2, 1.0, true);
    Router.AddEdge(v2, v3, 2.0, true);
    Router.AddEdge(v3, v4, 3.0, true);
    Router.AddEdge(v4, v1, 4.0, true);

    std::vector<TVertexID> path;

    EXPECT_EQ(Router.FindShortestPath(v1, v4, path), 6.0);
    EXPECT_EQ(path.size(), 4);
    EXPECT_EQ(path[0], v1);
    EXPECT_EQ(path[1], v2);
    EXPECT_EQ(path[2], v3);
    EXPECT_EQ(path[3], v4);
}

TEST(DijkstraPathRouter, NoPaths) {
    CDijkstraPathRouter Router;
    
    auto v1 = Router.AddVertex(1);
    auto v2 = Router.AddVertex(2);
    auto v3 = Router.AddVertex(3);
    auto v4 = Router.AddVertex(4);

    std::vector<TVertexID> path;

    EXPECT_EQ(Router.FindShortestPath(v1, v4, path), std::numeric_limits<double>::max());
    EXPECT_EQ(path.size(), 0);
}

