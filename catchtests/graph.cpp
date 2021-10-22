#include<catch.h>
#include<cell_world.h>
#include<iostream>

using namespace cell_world;
using namespace std;

TEST_CASE("Network")
{
    World w;
    Cell c1({0,1},{1,1},false);
    Cell c2({1,1},{1,1},false);
    Cell c3({2,1},{1,1},false);
    Cell c4({3,1},{1,1},false);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    Graph n(w.create_cell_group());
    n[0].add(c2);
    n[0].add(c3);
    CHECK(n[0].size()==2);
    n[c4].add(c2);
    n[c4].add(c3);
    CHECK(n[c4].size()==2);
}

TEST_CASE("graph")
{
    World w;
    Cell c0({0,1},{1,1},false);
    Cell c1({1,1},{1,1},false);
    Cell c2({2,1},{1,1},false);
    Cell c3({3,1},{1,1},false);
    Cell c4({4,1},{1,1},false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    Cell_group cg = w.create_cell_group();
    Connection_pattern cp;
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> cp;
    Graph g = cp.get_graph(cg);
    CHECK(g.size() == 5);
    CHECK(g[c0].size() == 1);
    CHECK(g[c1].size() == 2);
    CHECK(g[c2].size() == 2);
    CHECK(g[c3].size() == 2);
    CHECK(g[c4].size() == 1);
}

TEST_CASE("graph.invert")
{
    World w;
    Cell c0({0,1},{1,1},false);
    Cell c1({1,1},{1,1},false);
    Cell c2({2,1},{1,1},false);
    Cell c3({3,1},{1,1},false);
    Cell c4({4,1},{1,1},false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    Cell_group cg = w.create_cell_group();
    Connection_pattern cp;
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> cp;
    auto g = cp.get_graph(cg).invert();
    CHECK(g.size() == 5);
    CHECK(g[c0].size() == 4);
    CHECK(g[c1].size() == 3);
    CHECK(g[c2].size() == 3);
    CHECK(g[c3].size() == 3);
    CHECK(g[c4].size() == 4);
}


TEST_CASE("graph.invert.empty")
{
    World w;
    Cell c0({0,1},{1,1},false);
    Cell c1({1,1},{1,1},false);
    Cell c2({2,1},{1,1},false);
    Cell c3({3,1},{1,1},false);
    Cell c4({4,1},{1,1},false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    Cell_group cg = w.create_cell_group();
    Shape desc(6);
    Transformation t (.5001,0);
    Graph g = Coordinates_visibility::create_graph(cg, desc, t).invert();
    CHECK(g.size() == 5);
    CHECK(g[c0].size() == 0);
    CHECK(g[c1].size() == 0);
    CHECK(g[c2].size() == 0);
    CHECK(g[c3].size() == 0);
    CHECK(g[c4].size() == 0);
}


TEST_CASE("Connections shortest path")
{
    World w;
    Cell c0({0,1},{1,1},false);
    Cell c1({1,1},{1,1},false);
    Cell c2({2,1},{1,1},false);
    Cell c3({3,1},{1,1},false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> w.connection_pattern;
    auto c = w.create_graph();
    auto sp = c.get_shortest_path(c0,c3);
    CHECK(sp.size()==4);
    CHECK(sp[0]==c3);
    CHECK(sp[1]==c2);
    CHECK(sp[2]==c1);
    CHECK(sp[3]==c0);
}

TEST_CASE("Sub graphs big") {
    World w;
    for (int8_t i = 0; i <= 100; i++){
        Cell c0({i, i}, {(double)i, (double)i},false);
        w.add(c0);
    }
    Cell_group cg1 = w.create_cell_group();
    Connection_pattern cp;
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> cp;
    auto wc = cp.get_graph(cg1);
    Cell_group cg;
    cg.add(w[0]);
    cg.add(w[50]);
    cg.add(w[100]);
    Graph gates(cg);
    auto sw = wc.get_sub_graphs(gates);
}

TEST_CASE("Sub graphs disconnected") {
    World w;
    Cell c0({0,1},{1,1},false);
    Cell c1({1,1},{1,1},false);
    Cell c2({2,1},{1,1},true);
    Cell c3({3,1},{1,1},false);
    Cell c4({4,1},{1,1},false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    Cell_group cg1 = w.create_cell_group();
    Connection_pattern cp;
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0},{\"x\":0,\"y\":-1},{\"x\":0,\"y\":1}]" >> w.connection_pattern;
    auto wc = w.create_graph();
    vector<Graph> sw = wc.get_sub_graphs();
    CHECK(sw.size() == 3);
    CHECK(sw[0].cells.contains(c0));
    CHECK(sw[0].cells.contains(c1));
    CHECK(sw[2].cells.contains(c3));
    CHECK(sw[2].cells.contains(c4));
    CHECK(sw[0][c0].size()==1);
    CHECK(sw[0][c0].contains(c1));
    CHECK(sw[0][c1].size()==1);
    CHECK(sw[0][c1].contains(c0));
    CHECK(sw[2][c3].size()==1);
    CHECK(sw[2][c3].contains(c4));
    CHECK(sw[2][c4].size()==1);
    CHECK(sw[2][c4].contains(c3));
}

TEST_CASE("Sub graphs options")
{
    World w;
    Cell c0({0,1},{1,1},false);
    Cell c1({1,1},{1,1},false);
    Cell c2({2,1},{1,1},false);
    Cell c3({3,1},{1,1},false);
    Cell c4({4,1},{1,1},false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    Cell_group cg1 = w.create_cell_group();
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0},{\"x\":0,\"y\":-1},{\"x\":0,\"y\":1}]" >> w.connection_pattern;
    auto wc = w.create_graph();
    CHECK(wc.size()==5);
    CHECK(wc[0].size()==1);
    CHECK(wc[1].size()==2);
    CHECK(wc[2].size()==2);
    CHECK(wc[3].size()==2);
    CHECK(wc[4].size()==1);

    Cell_group cg;
    cg.add(c0);
    cg.add(c2);
    cg.add(c4);
    Graph gates(cg);

    CHECK(gates.size() == 3);
    Graph options(cg1);
    vector<Graph> sw = wc.get_sub_graphs(gates,options);
    CHECK(sw.size() == 2);
    CHECK(sw[0].cells.contains(c0));
    CHECK(sw[0].cells.contains(c1));
    CHECK(sw[0].cells.contains(c2));
    CHECK(sw[1].cells.contains(c2));
    CHECK(sw[1].cells.contains(c3));
    CHECK(sw[1].cells.contains(c4));
    CHECK(sw[0][c0].size()==1);
    CHECK(sw[0][c0].contains(c1));
    CHECK(sw[0][c1].size()==2);
    CHECK(sw[0][c1].contains(c0));
    CHECK(sw[0][c1].contains(c2));
    CHECK(sw[0][c2].size()==1);
    CHECK(sw[0][c2].contains(c1));

    CHECK(sw[1][c2].size()==1);
    CHECK(sw[1][c2].contains(c3));
    CHECK(sw[1][c3].size()==2);
    CHECK(sw[1][c3].contains(c2));
    CHECK(sw[1][c3].contains(c4));
    CHECK(sw[1][c4].size()==1);
    CHECK(sw[1][c4].contains(c3));

    CHECK(gates[c0].size()==1);
    CHECK(gates[c2].size()==2);
    CHECK(gates[c4].size()==1);

    CHECK(options[c0].size()==1);
    CHECK(options[c0].contains(c2));
    CHECK(options[c1].size()==2);
    CHECK(options[c1].contains(c0));
    CHECK(options[c1].contains(c2));
    CHECK(options[c2].size()==2);
    CHECK(options[c2].contains(c0));
    CHECK(options[c2].contains(c4));
    CHECK(options[c3].size()==2);
    CHECK(options[c3].contains(c2));
    CHECK(options[c3].contains(c4));
    CHECK(options[c4].size()==1);
    CHECK(options[c4].contains(c2));

    auto inv = !options;
    CHECK(inv.size()==5);
    CHECK(inv[c0].size()==2);
    CHECK(inv[c0].contains(c1));
    CHECK(inv[c0].contains(c2));
    CHECK(inv[c1].size()==0);
    CHECK(inv[c2].size()==4);
    CHECK(inv[c2].contains(c0));
    CHECK(inv[c2].contains(c1));
    CHECK(inv[c2].contains(c3));
    CHECK(inv[c2].contains(c4));
    CHECK(inv[c3].size()==0);
    CHECK(inv[c4].size()==2);
    CHECK(inv[c4].contains(c2));
    CHECK(inv[c4].contains(c3));

    Cell_group ncg;
    ncg.add(c0);
    ncg.add(c1);
    ncg.add(c2);
    Graph ng(ncg);
    ncg.add(c3);
    ncg.add(c4);

    ng.connect(options);
    CHECK(ng[c0].size()==1);
    CHECK(ng[c1].size()==2);
    CHECK(ng[c2].size()==1);
    ng.add(c3);
    ng.add(c4);
    ng.connect(options);
    CHECK(ng[c0].size()==1);
    CHECK(ng[c1].size()==2);
    CHECK(ng[c2].size()==2);
    CHECK(ng[c3].size()==2);
    CHECK(ng[c4].size()==1);
}


TEST_CASE("Sub graphs")
{
    World w;
    Cell c0({0,1},{1,1},false);
    Cell c1({1,1},{1,1},false);
    Cell c2({2,1},{1,1},false);
    Cell c3({3,1},{1,1},false);
    Cell c4({4,1},{1,1},false);

    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    Cell_group cg1 = w.create_cell_group();
    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> w.connection_pattern;
    auto wc = w.create_graph();
    CHECK(wc.size()==5);
    CHECK(wc[0].size()==1);
    CHECK(wc[0].contains(c1));
    CHECK(wc[1].size()==2);
    CHECK(wc[2].size()==2);
    CHECK(wc[3].size()==2);
    CHECK(wc[4].size()==1);

    Cell_group cg;
    cg.add(c0);
    cg.add(c2);
    cg.add(c4);
    Graph gates(cg);
    CHECK(gates.size() == 3);
    vector<Graph> sw = wc.get_sub_graphs(gates);
    CHECK(sw.size() == 2);
    CHECK(sw[0].cells.contains(c0));
    CHECK(sw[0].cells.contains(c1));
    CHECK(sw[0].cells.contains(c2));
    CHECK(sw[1].cells.contains(c2));
    CHECK(sw[1].cells.contains(c3));
    CHECK(sw[1].cells.contains(c4));
    CHECK(sw[0][c0].size()==1);
    CHECK(sw[0][c0].contains(c1));
    CHECK(sw[0][c1].size()==2);
    CHECK(sw[0][c1].contains(c0));
    CHECK(sw[0][c1].contains(c2));
    CHECK(sw[0][c2].size()==1);
    CHECK(sw[0][c2].contains(c1));

    CHECK(sw[1][c2].size()==1);
    CHECK(sw[1][c2].contains(c3));
    CHECK(sw[1][c3].size()==2);
    CHECK(sw[1][c3].contains(c2));
    CHECK(sw[1][c3].contains(c4));
    CHECK(sw[1][c4].size()==1);
    CHECK(sw[1][c4].contains(c3));

    CHECK(gates[c0].size()==1);
    CHECK(gates[c2].size()==2);
    CHECK(gates[c4].size()==1);
}


TEST_CASE("shortest path") {
    World w;
    Cell c00({0, 0}, {0, 0}, false);
    Cell c01({0, 1}, {0, 1}, false);
    Cell c02({0, 2}, {0, 2}, false);
    Cell c10({1, 0}, {1, 0}, false);
    Cell c11({1, 1}, {1, 1}, true);
    Cell c12({1, 2}, {1, 2}, false);
    Cell c20({2, 0}, {2, 0}, true);
    Cell c21({2, 1}, {2, 1}, true);
    Cell c22({2, 2}, {2, 2}, false);
    Cell c30({3, 0}, {3, 0}, false);
    Cell c31({3, 1}, {3, 1}, false);
    Cell c32({3, 2}, {3, 2}, false);
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
    CHECK(g.get_shortest_path(c10,c30).size() == 9);
    CHECK(g.get_shortest_path(c00,c30).size() == 8);
    CHECK(g.get_shortest_path(c01,c30).size() == 7);
    CHECK(g.get_shortest_path(c02,c30).size() == 6);
    CHECK(g.get_shortest_path(c12,c30).size() == 5);
    CHECK(g.get_shortest_path(c22,c30).size() == 4);
    CHECK(g.get_shortest_path(c32,c30).size() == 3);
    CHECK(g.get_shortest_path(c31,c30).size() == 2);
    CHECK(g.get_shortest_path(c30,c30).size() == 1);
}