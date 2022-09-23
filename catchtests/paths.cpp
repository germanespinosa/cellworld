#include<catch.h>
#include<cell_world.h>
#include<iostream>

using namespace cell_world;
using namespace std;

TEST_CASE("euclidean")
{
    World w;
    w.add({{0,1},{0,1},false});
    w.add({{1,1},{1,1},false});
    w.add({{2,1},{2,1},false});
    w.add({{3,1},{3,1},false});
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> w.connection_pattern;
    auto g = w.create_graph();
    Paths p = Paths::get_euclidean(g);
    CHECK(p.get_move(w[0],w[3]) == Move{1,0});
    CHECK(p.get_move(w[1],w[3]) == Move{1,0});
    CHECK(p.get_move(w[2],w[3]) == Move{1,0});
    CHECK(p.get_move(w[3],w[3]) == Move{0,0});
    CHECK(p.get_move(w[3],w[0]) == Move{-1,0});
    CHECK(p.get_move(w[3],w[1]) == Move{-1,0});
    CHECK(p.get_move(w[3],w[2]) == Move{-1,0});
    CHECK(p.get_move(w[3],w[3]) == Move{0,0});
}

TEST_CASE("a_star")
{
    World w;
    w.add({{0,1},{0,1},false});
    w.add({{1,1},{1,1},false});
    w.add({{2,1},{2,1},false});
    w.add({{3,1},{3,1},false});
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> w.connection_pattern;
    auto g = w.create_graph();
    Paths p = Paths::get_manhattan(g);
    CHECK(p.get_move(w[0],w[3]) == Move{1,0});
    CHECK(p.get_move(w[1],w[3]) == Move{1,0});
    CHECK(p.get_move(w[2],w[3]) == Move{1,0});
    CHECK(p.get_move(w[3],w[3]) == Move{0,0});
    CHECK(p.get_move(w[3],w[0]) == Move{-1,0});
    CHECK(p.get_move(w[3],w[1]) == Move{-1,0});
    CHECK(p.get_move(w[3],w[2]) == Move{-1,0});
    CHECK(p.get_move(w[3],w[3]) == Move{0,0});
}

TEST_CASE("euclidean_blocked")
{
    World w;
    w.add({{0,0},{0,0},false}); // w[0]
    w.add({{0,1},{0,1},false}); // w[1]
    w.add({{0,2},{0,2},false}); // w[2]
    w.add({{1,0},{1,0},false}); // w[3]
    w.add({{1,1},{1,1},true});  // w[4]
    w.add({{1,2},{1,2},false}); // w[5]
    w.add({{2,0},{2,0},true});  // w[6]
    w.add({{2,1},{2,1},true});  // w[7]
    w.add({{2,2},{2,2},false}); // w[8]
    w.add({{3,0},{3,0},false}); // w[9]
    w.add({{3,1},{3,1},false}); // w[10]
    w.add({{3,2},{3,2},false}); // w[11]
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0},{\"x\":0,\"y\":-1},{\"x\":0,\"y\":1}]" >> w.connection_pattern;
    CHECK(w.size() == 12);
    Graph g = w.create_graph();
    CHECK(g.size() == 12); //three occluded are not part of the graph
    CHECK(g[w[0]].size() == 2);
    CHECK(g[w[1]].size() == 2);
    CHECK(g[ w[2]].size() == 2);
    CHECK(g.cells.find(w[3]) != Not_found);
    CHECK(g.cells.find(w[3]) == 3);
    CHECK(g[w[3]].size() == 1);
    Paths p = Paths::get_euclidean(g);
    CHECK(p.get_move(w[3], w[9]) == Move{-1,0});
    CHECK(p.get_move(w[0], w[9]) == Move{1,0});
    CHECK(p.get_move(w[1], w[9]) == Move{0,-1});
    CHECK(p.get_move(w[2], w[9]) == Move{1,0});
    CHECK(p.get_move( w[5], w[9]) == Move{1,0});
    CHECK(p.get_move(w[8], w[9]) == Move{1,0});
    CHECK(p.get_move( w[11], w[9]) == Move{0,-1});
    CHECK(p.get_move( w[10], w[9]) == Move{0,-1});
    CHECK(p.get_move( w[9], w[9]) == Move{0,0});
}

TEST_CASE("manhattan")
{
    World w;
    w.add({{0,0},{0,0},false}); // w[0]
    w.add({{0,1},{0,1},false}); // w[1]
    w.add({{0,2},{0,2},false}); // w[2]
    w.add({{1,0},{1,0},false}); // w[3]
    w.add({{1,1},{1,1},true});  // w[4]
    w.add({{1,2},{1,2},false}); // w[5]
    w.add({{2,0},{2,0},true});  // w[6]
    w.add({{2,1},{2,1},true});  // w[7]
    w.add({{2,2},{2,2},false}); // w[8]
    w.add({{3,0},{3,0},false}); // w[9]
    w.add({{3,1},{3,1},false}); // w[10]
    w.add({{3,2},{3,2},false}); // w[11]
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0},{\"x\":0,\"y\":-1},{\"x\":0,\"y\":1}]" >> w.connection_pattern;
    CHECK(w.size() == 12);

    Graph g = w.create_graph();
    CHECK(g.size() == 12);
    CHECK(g[w[0]].size() == 2);
    CHECK(g[w[1]].size() == 2);
    CHECK(g[w[2]].size() == 2);
    CHECK(g.cells.find(w[3]) != Not_found);
    CHECK(g.cells.find(w[3]) == 3);
    CHECK(g[w[3]].size() == 1);

    Paths p = Paths::get_manhattan(g);
    CHECK(p.get_move(w[3],w[9]) == Move{-1,0});
    CHECK(p.get_move(w[0],w[9]) == Move{0,1});
    CHECK(p.get_move(w[1],w[9]) == Move{0,1});
    CHECK(p.get_move(w[2],w[9]) == Move{1,0});
    CHECK(p.get_move(w[5],w[9]) == Move{1,0});
    CHECK(p.get_move(w[8],w[9]) == Move{1,0});
    CHECK(p.get_move(w[11],w[9]) == Move{0,-1});
    CHECK(p.get_move(w[10],w[9]) == Move{0,-1});
    CHECK(p.get_move(w[9],w[9]) == Move{0,0});
}

TEST_CASE("astar")
{
    World w;
    w.add({{0,0},{0,0},false}); // w[0]
    w.add({{0,1},{0,1},false}); // w[1]
    w.add({{0,2},{0,2},false}); // w[2]
    w.add({{1,0},{1,0},false}); // w[3]
    w.add({{1,1},{1,1},true});  // w[4]
    w.add({{1,2},{1,2},false}); // w[5]
    w.add({{2,0},{2,0},true});  // w[6]
    w.add({{2,1},{2,1},true});  // w[7]
    w.add({{2,2},{2,2},false}); // w[8]
    w.add({{3,0},{3,0},false}); // w[9]
    w.add({{3,1},{3,1},false}); // w[10]
    w.add({{3,2},{3,2},false}); // w[11]
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0},{\"x\":0,\"y\":-1},{\"x\":0,\"y\":1}]" >> w.connection_pattern;
    CHECK(w.size() == 12);

    Graph g = w.create_graph();
    CHECK(g.size() == 12);
    CHECK(g[w[0]].size() == 2);
    CHECK(g[w[1]].size() == 2);
    CHECK(g[w[2]].size() == 2);
    CHECK(g.cells.find(w[3]) != Not_found);
    CHECK(g.cells.find(w[3]) == 3);
    CHECK(g[w[3]].size() == 1);

    Paths p = Paths::get_astar(g);
    CHECK(p.get_move(w[3],w[9]) == Move{-1,0});
    CHECK(p.get_move(w[0],w[9]) == Move{0,1});
    CHECK(p.get_move(w[1],w[9]) == Move{0,1});
    CHECK(p.get_move(w[2],w[9]) == Move{1,0});
    CHECK(p.get_move(w[5],w[9]) == Move{1,0});
    CHECK(p.get_move(w[8],w[9]) == Move{1,0});
    CHECK(p.get_move(w[11],w[9]) == Move{0,-1});
    CHECK(p.get_move(w[10],w[9]) == Move{0,-1});
    CHECK(p.get_move(w[9],w[9]) == Move{0,0});
}

TEST_CASE("astar paths")
{
    World w;
    w.add({{0,0},{0,0},false}); // w[0]
    w.add({{0,1},{0,1},false}); // w[1]
    w.add({{0,2},{0,2},false}); // w[2]
    w.add({{1,0},{1,0},false}); // w[3]
    w.add({{1,1},{1,1},true});  // w[4]
    w.add({{1,2},{1,2},false}); // w[5]
    w.add({{2,0},{2,0},true});  // w[6]
    w.add({{2,1},{2,1},true});  // w[7]
    w.add({{2,2},{2,2},false}); // w[8]
    w.add({{3,0},{3,0},false}); // w[9]
    w.add({{3,1},{3,1},false}); // w[10]
    w.add({{3,2},{3,2},false}); // w[11]
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0},{\"x\":0,\"y\":-1},{\"x\":0,\"y\":1}]" >> w.connection_pattern;
    CHECK(w.size() == 12);

    Graph g = w.create_graph();
    Paths p = Paths::get_astar(g);
    CHECK(p.get_path(w[3],w[9]).size() == 9);
    CHECK(p.get_path(w[3],w[3]).size() == 1);
    CHECK(p.get_path(w[3],w[0]).size() == 2);
}

TEST_CASE("path_save_shortest")
{
    World w;
    w.add({{0,0},{0,0},false}); // w[0]
    w.add({{0,1},{0,1},false}); // w[1]
    w.add({{0,2},{0,2},false}); // w[2]
    w.add({{1,0},{1,0},false}); // w[3]
    w.add({{1,1},{1,1},true});  // w[4]
    w.add({{1,2},{1,2},false}); // w[5]
    w.add({{2,0},{2,0},true});  // w[6]
    w.add({{2,1},{2,1},true});  // w[7]
    w.add({{2,2},{2,2},false}); // w[8]
    w.add({{3,0},{3,0},false}); // w[9]
    w.add({{3,1},{3,1},false}); // w[10]
    w.add({{3,2},{3,2},false}); // w[11]
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0},{\"x\":0,\"y\":-1},{\"x\":0,\"y\":1}]" >> w.connection_pattern;
    CHECK(w.size() == 12);
    Graph g = w.create_graph();
    CHECK(g.size() == 12);
    CHECK(g[w[0]].size() == 2);
    CHECK(g[w[1]].size() == 2);
    CHECK(g[w[2]].size() == 2);
    CHECK(g.cells.find(w[3]) != Not_found);
    CHECK(g.cells.find(w[3]) == 3);
    CHECK(g[w[3]].size() == 1);
    Paths p = Paths::get_manhattan(g);
    p.save("test_path");
    Paths np = Paths::get_manhattan(g);
    CHECK(p == np);
}

TEST_CASE("euclidean_blocked_save")
{
    World w;
    w.add({{0,0},{0,0},false}); // w[0]
    w.add({{0,1},{0,1},false}); // w[1]
    w.add({{0,2},{0,2},false}); // w[2]
    w.add({{1,0},{1,0},false}); // w[3]
    w.add({{1,1},{1,1},true});  // w[4]
    w.add({{1,2},{1,2},false}); // w[5]
    w.add({{2,0},{2,0},true});  // w[6]
    w.add({{2,1},{2,1},true});  // w[7]
    w.add({{2,2},{2,2},false}); // w[8]
    w.add({{3,0},{3,0},false}); // w[9]
    w.add({{3,1},{3,1},false}); // w[10]
    w.add({{3,2},{3,2},false}); // w[11]
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0},{\"x\":0,\"y\":-1},{\"x\":0,\"y\":1}]" >> w.connection_pattern;
    CHECK(w.size() == 12);
    Graph g = w.create_graph();
    CHECK(g.size() == 12);
    CHECK(g[w[0]].size() == 2);
    CHECK(g[w[1]].size() == 2);
    CHECK(g[w[2]].size() == 2);
    CHECK(g.cells.find(w[3]) != Not_found);
    CHECK(g.cells.find(w[3]) == 3);
    CHECK(g[w[3]].size() == 1);
    Paths p = Paths::get_euclidean(g);
    p.save("test_path.path");
    Paths np = Paths::get_euclidean(g);
    CHECK(p == np);
}
