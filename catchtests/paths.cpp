#include<catch.h>
#include<cell_world.h>
#include<iostream>
using namespace cell_world;
using namespace std;

TEST_CASE("euclidean")
{
    World w("test");
    Cell c0(Circle, {0,1},{0,1},0,false);
    Cell c1(Circle, {1,1},{1,1},0,false);
    Cell c2(Circle, {2,1},{2,1},0,false);
    Cell c3(Circle, {3,1},{3,1},0,false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.connection_pattern = Connection_pattern({{1,0},{-1,0}});
    auto g = w.create_graph();
    Path p(g, Path::Distance_type::euclidean);
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
    Cell c0(Circle, {0,1},{0,1},0,false);
    Cell c1(Circle, {1,1},{1,1},0,false);
    Cell c2(Circle, {2,1},{2,1},0,false);
    Cell c3(Circle, {3,1},{3,1},0,false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.connection_pattern = Connection_pattern({{1,0},{-1,0}});
    auto g = w.create_graph();
    Path p(g, Path::Distance_type::a_star);
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
    Cell c00(Circle, {0,0},{0,0},0,false);
    Cell c01(Circle, {0,1},{0,1},1,false);
    Cell c02(Circle, {0,2},{0,2},2,false);
    Cell c10(Circle, {1,0},{1,0},3,false);
    Cell c11(Circle, {1,1},{1,1},4,true);
    Cell c12(Circle, {1,2},{1,2},5,false);
    Cell c20(Circle, {2,0},{2,0},6,true);
    Cell c21(Circle, {2,1},{2,1},7,true);
    Cell c22(Circle, {2,2},{2,2},8,false);
    Cell c30(Circle, {3,0},{3,0},9,false);
    Cell c31(Circle, {3,1},{3,1},10,false);
    Cell c32(Circle, {3,2},{3,2},11,false);
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
    w.connection_pattern = Connection_pattern({{1,0},{-1,0},{0,1},{0,-1}});
    CHECK(w.size() == 12);
    Graph g = w.create_graph();
    CHECK(g.nodes.size() == 9);
    CHECK(g[c00].size() == 2);
    CHECK(g[c01].size() == 2);
    CHECK(g[c02].size() == 2);
    CHECK(g.nodes.find(c10) != Not_found);
    CHECK(g.nodes.find(c10) == 3);
    CHECK(g[c10].size() == 1);
    Path p(g, Path::Distance_type::euclidean);
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

TEST_CASE("a_start_blocked")
{
    World w("test");
    Cell c00(Circle, {0,0},{0,0},0,false);
    Cell c01(Circle, {0,1},{0,1},1,false);
    Cell c02(Circle, {0,2},{0,2},2,false);
    Cell c10(Circle, {1,0},{1,0},3,false);
    Cell c11(Circle, {1,1},{1,1},4,true);
    Cell c12(Circle, {1,2},{1,2},5,false);
    Cell c20(Circle, {2,0},{2,0},6,true);
    Cell c21(Circle, {2,1},{2,1},7,true);
    Cell c22(Circle, {2,2},{2,2},8,false);
    Cell c30(Circle, {3,0},{3,0},9,false);
    Cell c31(Circle, {3,1},{3,1},10,false);
    Cell c32(Circle, {3,2},{3,2},11,false);
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
    w.connection_pattern = Connection_pattern({{1,0},{-1,0},{0,1},{0,-1}});
    CHECK(w.size() == 12);
    Graph g = w.create_graph();
    CHECK(g.nodes.size() == 9);
    CHECK(g[c00].size() == 2);
    CHECK(g[c01].size() == 2);
    CHECK(g[c02].size() == 2);
    CHECK(g.nodes.find(c10) != Not_found);
    CHECK(g.nodes.find(c10) == 3);
    CHECK(g[c10].size() == 1);
    Path p(g, Path::Distance_type::a_star);
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
