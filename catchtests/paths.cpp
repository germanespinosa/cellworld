#include<catch.h>
#include<cell_world.h>
#include<iostream>

using namespace cell_world;
using namespace std;

TEST_CASE("euclidean")
{
    World w("test");
    Cell c0(Circle, {0,1},{0,1},false);
    Cell c1(Circle, {1,1},{1,1},false);
    Cell c2(Circle, {2,1},{2,1},false);
    Cell c3(Circle, {3,1},{3,1},false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> w.connection_pattern;
    auto g = w.create_graph();
    Paths p = Paths::get_euclidean(g);
    CHECK(p.get_move(c0,c3) == Move{1,0});
    CHECK(p.get_move(c1,c3) == Move{1,0});
    CHECK(p.get_move(c2,c3) == Move{1,0});
    CHECK(p.get_move(c3,c3) == Move{0,0});
    CHECK(p.get_move(c3,c0) == Move{-1,0});
    CHECK(p.get_move(c3,c1) == Move{-1,0});
    CHECK(p.get_move(c3,c2) == Move{-1,0});
    CHECK(p.get_move(c3,c3) == Move{0,0});
}

TEST_CASE("a_star")
{
    World w("test");
    Cell c0(Circle, {0,1},{0,1},false);
    Cell c1(Circle, {1,1},{1,1},false);
    Cell c2(Circle, {2,1},{2,1},false);
    Cell c3(Circle, {3,1},{3,1},false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> w.connection_pattern;
    auto g = w.create_graph();
    Paths p = Paths::get_manhattan(g);
    CHECK(p.get_move(c0,c3) == Move{1,0});
    CHECK(p.get_move(c1,c3) == Move{1,0});
    CHECK(p.get_move(c2,c3) == Move{1,0});
    CHECK(p.get_move(c3,c3) == Move{0,0});
    CHECK(p.get_move(c3,c0) == Move{-1,0});
    CHECK(p.get_move(c3,c1) == Move{-1,0});
    CHECK(p.get_move(c3,c2) == Move{-1,0});
    CHECK(p.get_move(c3,c3) == Move{0,0});
}

TEST_CASE("euclidean_blocked")
{
    World w("test");
    Cell c00(Circle, {0,0},{0,0},false);
    Cell c01(Circle, {0,1},{0,1},false);
    Cell c02(Circle, {0,2},{0,2},false);
    Cell c10(Circle, {1,0},{1,0},false);
    Cell c11(Circle, {1,1},{1,1},true);
    Cell c12(Circle, {1,2},{1,2},false);
    Cell c20(Circle, {2,0},{2,0},true);
    Cell c21(Circle, {2,1},{2,1},true);
    Cell c22(Circle, {2,2},{2,2},false);
    Cell c30(Circle, {3,0},{3,0},false);
    Cell c31(Circle, {3,1},{3,1},false);
    Cell c32(Circle, {3,2},{3,2},false);
    w.add(c00);
    w.add(c10);
    w.add(c20);
    w.add(c30);
    w.add(c01);
    w.add(c11);
    w.add(c21);
    w.add(c31);
    w.add(c02);
    w.add(c12);
    w.add(c22);
    w.add(c32);
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0},{\"x\":0,\"y\":-1},{\"x\":0,\"y\":1}]" >> w.connection_pattern;
    CHECK(w.size() == 12);
    Graph g = w.create_graph();
    CHECK(g.size() == 12); //three occluded are not part of the graph
    CHECK(g[c00].size() == 2);
    CHECK(g[c01].size() == 2);
    CHECK(g[c02].size() == 2);
    CHECK(g.cells.find(c10) != Not_found);
    CHECK(g.cells.find(c10) == 1);
    CHECK(g[c10].size() == 1);
    Paths p = Paths::get_euclidean(g);
    CHECK(p.get_move(c10,c30) == Move{-1,0});
    CHECK(p.get_move(c00,c30) == Move{1,0});
    CHECK(p.get_move(c01,c30) == Move{0,-1});
    CHECK(p.get_move(c02,c30) == Move{1,0});
    CHECK(p.get_move(c12,c30) == Move{1,0});
    CHECK(p.get_move(c22,c30) == Move{1,0});
    CHECK(p.get_move(c32,c30) == Move{0,-1});
    CHECK(p.get_move(c31,c30) == Move{0,-1});
    CHECK(p.get_move(c30,c30) == Move{0,0});
}

TEST_CASE("manhattan")
{
    World w("test");
    Cell c00(Circle, {0,0},{0,0},false);
    Cell c01(Circle, {0,1},{0,1},false);
    Cell c02(Circle, {0,2},{0,2},false);
    Cell c10(Circle, {1,0},{1,0},false);
    Cell c11(Circle, {1,1},{1,1},true);
    Cell c12(Circle, {1,2},{1,2},false);
    Cell c20(Circle, {2,0},{2,0},true);
    Cell c21(Circle, {2,1},{2,1},true);
    Cell c22(Circle, {2,2},{2,2},false);
    Cell c30(Circle, {3,0},{3,0},false);
    Cell c31(Circle, {3,1},{3,1},false);
    Cell c32(Circle, {3,2},{3,2},false);
    w.add(c00);
    w.add(c01);
    w.add(c02);
    w.add(c10);
    w.add(c11);
    w.add(c12);
    w.add(c20);
    w.add(c21);
    w.add(c22);
    w.add(c30);
    w.add(c31);
    w.add(c32);
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0},{\"x\":0,\"y\":-1},{\"x\":0,\"y\":1}]" >> w.connection_pattern;
    CHECK(w.size() == 12);

    Graph g = w.create_graph();
    CHECK(g.size() == 12);
    CHECK(g[c00].size() == 2);
    CHECK(g[c01].size() == 2);
    CHECK(g[c02].size() == 2);
    CHECK(g.cells.find(c10) != Not_found);
    CHECK(g.cells.find(c10) == 3);
    CHECK(g[c10].size() == 1);

    Paths p = Paths::get_manhattan(g);
    CHECK(p.get_move(c10,c30) == Move{-1,0});
    CHECK(p.get_move(c00,c30) == Move{0,1});
    CHECK(p.get_move(c01,c30) == Move{0,1});
    CHECK(p.get_move(c02,c30) == Move{1,0});
    CHECK(p.get_move(c12,c30) == Move{1,0});
    CHECK(p.get_move(c22,c30) == Move{1,0});
    CHECK(p.get_move(c32,c30) == Move{0,-1});
    CHECK(p.get_move(c31,c30) == Move{0,-1});
    CHECK(p.get_move(c30,c30) == Move{0,0});
}

TEST_CASE("astar")
{
    World w("test");
    Cell c00(Circle, {0,0},{0,0},false);
    Cell c01(Circle, {0,1},{0,1},false);
    Cell c02(Circle, {0,2},{0,2},false);
    Cell c10(Circle, {1,0},{1,0},false);
    Cell c11(Circle, {1,1},{1,1},true);
    Cell c12(Circle, {1,2},{1,2},false);
    Cell c20(Circle, {2,0},{2,0},true);
    Cell c21(Circle, {2,1},{2,1},true);
    Cell c22(Circle, {2,2},{2,2},false);
    Cell c30(Circle, {3,0},{3,0},false);
    Cell c31(Circle, {3,1},{3,1},false);
    Cell c32(Circle, {3,2},{3,2},false);
    w.add(c00);
    w.add(c01);
    w.add(c02);
    w.add(c10);
    w.add(c11);
    w.add(c12);
    w.add(c20);
    w.add(c21);
    w.add(c22);
    w.add(c30);
    w.add(c31);
    w.add(c32);
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0},{\"x\":0,\"y\":-1},{\"x\":0,\"y\":1}]" >> w.connection_pattern;
    CHECK(w.size() == 12);

    Graph g = w.create_graph();
    CHECK(g.size() == 12);
    CHECK(g[c00].size() == 2);
    CHECK(g[c01].size() == 2);
    CHECK(g[c02].size() == 2);
    CHECK(g.cells.find(c10) != Not_found);
    CHECK(g.cells.find(c10) == 3);
    CHECK(g[c10].size() == 1);

    Paths p = Paths::get_astar(g);
    CHECK(p.get_move(c10,c30) == Move{-1,0});
    CHECK(p.get_move(c00,c30) == Move{0,1});
    CHECK(p.get_move(c01,c30) == Move{0,1});
    CHECK(p.get_move(c02,c30) == Move{1,0});
    CHECK(p.get_move(c12,c30) == Move{1,0});
    CHECK(p.get_move(c22,c30) == Move{1,0});
    CHECK(p.get_move(c32,c30) == Move{0,-1});
    CHECK(p.get_move(c31,c30) == Move{0,-1});
    CHECK(p.get_move(c30,c30) == Move{0,0});
}

TEST_CASE("astar paths")
{
    World w("test");
    Cell c00(Circle, {0,0},{0,0},false);
    Cell c01(Circle, {0,1},{0,1},false);
    Cell c02(Circle, {0,2},{0,2},false);
    Cell c10(Circle, {1,0},{1,0},false);
    Cell c11(Circle, {1,1},{1,1},true);
    Cell c12(Circle, {1,2},{1,2},false);
    Cell c20(Circle, {2,0},{2,0},true);
    Cell c21(Circle, {2,1},{2,1},true);
    Cell c22(Circle, {2,2},{2,2},false);
    Cell c30(Circle, {3,0},{3,0},false);
    Cell c31(Circle, {3,1},{3,1},false);
    Cell c32(Circle, {3,2},{3,2},false);
    w.add(c00);
    w.add(c01);
    w.add(c02);
    w.add(c10);
    w.add(c11);
    w.add(c12);
    w.add(c20);
    w.add(c21);
    w.add(c22);
    w.add(c30);
    w.add(c31);
    w.add(c32);
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0},{\"x\":0,\"y\":-1},{\"x\":0,\"y\":1}]" >> w.connection_pattern;
    CHECK(w.size() == 12);

    Graph g = w.create_graph();
    Paths p = Paths::get_astar(g);
    CHECK(p.get_path(c10,c30).size() == 9);
    CHECK(p.get_path(c10,c10).size() == 1);
    CHECK(p.get_path(c10,c00).size() == 2);
}

TEST_CASE("path_save_shortest")
{
    World w("test");
    Cell c00(Circle, {0,0},{0,0},false);
    Cell c01(Circle, {0,1},{0,1},false);
    Cell c02(Circle, {0,2},{0,2},false);
    Cell c10(Circle, {1,0},{1,0},false);
    Cell c11(Circle, {1,1},{1,1},true);
    Cell c12(Circle, {1,2},{1,2},false);
    Cell c20(Circle, {2,0},{2,0},true);
    Cell c21(Circle, {2,1},{2,1},true);
    Cell c22(Circle, {2,2},{2,2},false);
    Cell c30(Circle, {3,0},{3,0},false);
    Cell c31(Circle, {3,1},{3,1},false);
    Cell c32(Circle, {3,2},{3,2},false);
    w.add(c00);
    w.add(c01);
    w.add(c02);
    w.add(c10);
    w.add(c11);
    w.add(c12);
    w.add(c20);
    w.add(c21);
    w.add(c22);
    w.add(c30);
    w.add(c31);
    w.add(c32);
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0},{\"x\":0,\"y\":-1},{\"x\":0,\"y\":1}]" >> w.connection_pattern;
    CHECK(w.size() == 12);
    Graph g = w.create_graph();
    CHECK(g.size() == 12);
    CHECK(g[c00].size() == 2);
    CHECK(g[c01].size() == 2);
    CHECK(g[c02].size() == 2);
    CHECK(g.cells.find(c10) != Not_found);
    CHECK(g.cells.find(c10) == 3);
    CHECK(g[c10].size() == 1);
    Paths p = Paths::get_manhattan(g);
    p.save("test_path");
    Paths np = Paths::get_manhattan(g);
    CHECK(p == np);
}

TEST_CASE("euclidean_blocked_save")
{
    World w("test");
    Cell c00(Circle, {0,0},{0,0},false);
    Cell c01(Circle, {0,1},{0,1},false);
    Cell c02(Circle, {0,2},{0,2},false);
    Cell c10(Circle, {1,0},{1,0},false);
    Cell c11(Circle, {1,1},{1,1},true);
    Cell c12(Circle, {1,2},{1,2},false);
    Cell c20(Circle, {2,0},{2,0},true);
    Cell c21(Circle, {2,1},{2,1},true);
    Cell c22(Circle, {2,2},{2,2},false);
    Cell c30(Circle, {3,0},{3,0},false);
    Cell c31(Circle, {3,1},{3,1},false);
    Cell c32(Circle, {3,2},{3,2},false);
    w.add(c00);
    w.add(c01);
    w.add(c02);
    w.add(c10);
    w.add(c11);
    w.add(c12);
    w.add(c20);
    w.add(c21);
    w.add(c22);
    w.add(c30);
    w.add(c31);
    w.add(c32);
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0},{\"x\":0,\"y\":-1},{\"x\":0,\"y\":1}]" >> w.connection_pattern;
    CHECK(w.size() == 12);
    Graph g = w.create_graph();
    CHECK(g.size() == 12);
    CHECK(g[c00].size() == 2);
    CHECK(g[c01].size() == 2);
    CHECK(g[c02].size() == 2);
    CHECK(g.cells.find(c10) != Not_found);
    CHECK(g.cells.find(c10) == 3);
    CHECK(g[c10].size() == 1);
    Paths p = Paths::get_euclidean(g);
    p.save("test_path.path");
    Paths np = Paths::get_euclidean(g);
    CHECK(p == np);
}
