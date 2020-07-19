#include<catch.h>
#include<cell_world.h>
#include<iostream>
/*
using namespace cell_world;
using namespace std;

TEST_CASE("Network")
{
    World w("test");
    Cell c1(Circle, {0,1},{1,1},0,false);
    Cell c2(Circle, {1,1},{1,1},0,false);
    Cell c3(Circle, {2,1},{1,1},0,false);
    Cell c4(Circle, {3,1},{1,1},0,false);
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
    World w("test");
    Cell c0(Circle, {0,1},{1,1},0,false);
    Cell c1(Circle, {1,1},{1,1},0,false);
    Cell c2(Circle, {2,1},{1,1},0,false);
    Cell c3(Circle, {3,1},{1,1},0,false);
    Cell c4(Circle, {4,1},{1,1},0,false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    Cell_group cg = w.create_cell_group();
    Graph g = Connection_pattern({{-1,0},{1,0}}).get_graph(cg);
    CHECK(g.nodes.size() == 5);
    CHECK(g[c0].size() == 1);
    CHECK(g[c1].size() == 2);
    CHECK(g[c2].size() == 2);
    CHECK(g[c3].size() == 2);
    CHECK(g[c4].size() == 1);
}

TEST_CASE("graph.invert")
{
    World w("test");
    Cell c0(Circle, {0,1},{1,1},0,false);
    Cell c1(Circle, {1,1},{1,1},0,false);
    Cell c2(Circle, {2,1},{1,1},0,false);
    Cell c3(Circle, {3,1},{1,1},0,false);
    Cell c4(Circle, {4,1},{1,1},0,false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    Cell_group cg = w.create_cell_group();
    Graph g = Connection_pattern({{-1,0},{1,0}}).get_graph(cg).invert();
    CHECK(g.nodes.size() == 5);
    CHECK(g[c0].size() == 4);
    CHECK(g[c1].size() == 3);
    CHECK(g[c2].size() == 3);
    CHECK(g[c3].size() == 3);
    CHECK(g[c4].size() == 4);
}


TEST_CASE("graph.invert.empty")
{
    World w("test");
    Cell c0(Circle, {0,1},{1,1},0,false);
    Cell c1(Circle, {1,1},{1,1},0,false);
    Cell c2(Circle, {2,1},{1,1},0,false);
    Cell c3(Circle, {3,1},{1,1},0,false);
    Cell c4(Circle, {4,1},{1,1},0,false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    Cell_group cg = w.create_cell_group();
    Graph g = Visibility::create_graph(cg).invert();
    CHECK(g.nodes.size() == 5);
    CHECK(g[c0].size() == 0);
    CHECK(g[c1].size() == 0);
    CHECK(g[c2].size() == 0);
    CHECK(g[c3].size() == 0);
    CHECK(g[c4].size() == 0);
}


TEST_CASE("Connections shortest path")
{
    World w("test");
    Cell c0(Circle, {0,1},{1,1},0,false);
    Cell c1(Circle, {1,1},{1,1},0,false);
    Cell c2(Circle, {2,1},{1,1},0,false);
    Cell c3(Circle, {3,1},{1,1},0,false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.connection_pattern = Connection_pattern({{1,0},{-1,0}});
    auto c = w.create_graph();
    auto sp = c.get_shortest_path(c0,c3);
    CHECK(sp.size()==4);
    CHECK(sp[0]==c3);
    CHECK(sp[1]==c2);
    CHECK(sp[2]==c1);
    CHECK(sp[3]==c0);
}

TEST_CASE("Sub graphs")
{
    World w("test");
    Cell c0(Circle, {0,1},{1,1},0,false);
    Cell c1(Circle, {1,1},{1,1},0,false);
    Cell c2(Circle, {2,1},{1,1},0,false);
    Cell c3(Circle, {3,1},{1,1},0,false);
    Cell c4(Circle, {4,1},{1,1},0,false);

    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    Cell_group cg1 = w.create_cell_group();
    w.connection_pattern = Connection_pattern({{-1,0},{1,0},{0,-1},{0,1}});
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
    CHECK(sw[0].nodes.contains(c0));
    CHECK(sw[0].nodes.contains(c1));
    CHECK(sw[0].nodes.contains(c2));
    CHECK(sw[1].nodes.contains(c2));
    CHECK(sw[1].nodes.contains(c3));
    CHECK(sw[1].nodes.contains(c4));
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

TEST_CASE("Sub graphs big") {
    World w("test");
    for (int8_t i = 0; i <=100; i++){
        Cell c0(Circle, {i, i}, {(double)i, (double)i}, 0, false);
        w.add(c0);
    }
    Cell_group cg1 = w.create_cell_group();
    auto wc = Connection_pattern({{-1, 0},{1,  0}}).get_graph(cg1);
    Cell_group cg;
    cg.add(w[0]);
    cg.add(w[50]);
    cg.add(w[100]);
    Graph gates(cg);
    auto sw = wc.get_sub_graphs(gates);
}

TEST_CASE("Sub graphs disconnected") {
    World w("test");
    Cell c0(Circle, {0,1},{1,1},0,false);
    Cell c1(Circle, {1,1},{1,1},0,false);
    Cell c2(Circle, {2,1},{1,1},0,true);
    Cell c3(Circle, {3,1},{1,1},0,false);
    Cell c4(Circle, {4,1},{1,1},0,false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    Cell_group cg1 = w.create_cell_group();
    w.connection_pattern = Connection_pattern({{-1,0},{1,0},{0,-1},{0,1}});
    auto wc = w.create_graph();
    vector<Graph> sw = wc.get_sub_graphs();
    CHECK(sw.size() == 2);
    CHECK(sw[0].nodes.contains(c0));
    CHECK(sw[0].nodes.contains(c1));
    CHECK(sw[1].nodes.contains(c3));
    CHECK(sw[1].nodes.contains(c4));
    CHECK(sw[0][c0].size()==1);
    CHECK(sw[0][c0].contains(c1));
    CHECK(sw[0][c1].size()==1);
    CHECK(sw[0][c1].contains(c0));
    CHECK(sw[1][c3].size()==1);
    CHECK(sw[1][c3].contains(c4));
    CHECK(sw[1][c4].size()==1);
    CHECK(sw[1][c4].contains(c3));
}


TEST_CASE("Sub graphs options")
{
    World w("test");
    Cell c0(Circle, {0,1},{1,1},0,false);
    Cell c1(Circle, {1,1},{1,1},0,false);
    Cell c2(Circle, {2,1},{1,1},0,false);
    Cell c3(Circle, {3,1},{1,1},0,false);
    Cell c4(Circle, {4,1},{1,1},0,false);
    w.add(c0);
    w.add(c1);
    w.add(c2);
    w.add(c3);
    w.add(c4);
    Cell_group cg1 = w.create_cell_group();
    w.connection_pattern = Connection_pattern({{-1,0},{1,0},{0,-1},{0,1}});
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
    CHECK(sw[0].nodes.contains(c0));
    CHECK(sw[0].nodes.contains(c1));
    CHECK(sw[0].nodes.contains(c2));
    CHECK(sw[1].nodes.contains(c2));
    CHECK(sw[1].nodes.contains(c3));
    CHECK(sw[1].nodes.contains(c4));
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
    CHECK(inv.nodes.size()==5);
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
*/