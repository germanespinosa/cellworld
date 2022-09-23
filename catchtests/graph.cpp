#include<catch.h>
#include<cell_world.h>
#include<iostream>

using namespace cell_world;
using namespace std;

TEST_CASE("Centrality"){
    World w = World::get_from_parameters_name("hexagonal","canonical","10_05");
    auto g = w.create_graph();
    CHECK(g.is_connected(w.cells[0], w.cells[330]));
//    cout << g.get_centrality() << endl;
//    cout << g.get_centrality(2) << endl;
//    cout << g.get_centrality(3) << endl;
//    cout << g.get_centrality(4) << endl;
//    cout << g.get_centrality(5) << endl;
}

//
//TEST_CASE("Network")
//{
//    World w;
//    Cell c0({0,1},{1,1},false);
//    Cell c1({0,1},{1,1},false);
//    Cell c2({1,1},{1,1},false);
//    Cell c3({2,1},{1,1},false);
//    Cell c4({3,1},{1,1},false);
//    w.add(c0);
//    w.add(c1);
//    w.add(c2);
//    w.add(c3);
//    w.add(c4);
//    Graph n(w.create_cell_group());
//    n[0].add(w[2]);
//    n[0].add(w[3]);
//    CHECK(n[0].size()==2);
//    n[w[4]].add(w[2]);
//    n[w[4]].add(w[3]);
//    CHECK(n[w[4]].size()==2);
//}
//
//TEST_CASE("graph")
//{
//    World w;
//    w.add({0,1});
//    w.add({1,1});
//    w.add({2,1});
//    w.add({3,1});
//    w.add({4,1});
//    Cell_group cg = w.create_cell_group();
//    Connection_pattern cp;
//    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> cp;
//    Graph g = cp.get_graph(cg);
//    CHECK(g.size() == 5);
//    CHECK(g[0].size() == 1);
//    CHECK(g[1].size() == 2);
//    CHECK(g[2].size() == 2);
//    CHECK(g[3].size() == 2);
//    CHECK(g[4].size() == 1);
//}
//
//TEST_CASE("graph.invert")
//{
//    World w;
//    w.add({1,1});
//    w.add({2,1});
//    w.add({3,1});
//    w.add({4,1});
//    w.add({5,1});
//    Cell_group cg = w.create_cell_group();
//    Connection_pattern cp;
//    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> cp;
//    auto g = cp.get_graph(cg).invert();
//    CHECK(g.size() == 5);
//    CHECK(g[0].size() == 4);
//    CHECK(g[1].size() == 3);
//    CHECK(g[2].size() == 3);
//    CHECK(g[3].size() == 3);
//    CHECK(g[4].size() == 4);
//}
//
//
//TEST_CASE("graph.invert.empty")
//{
//    World w;
//    Cell c0({0,1},{1,1},false);
//    Cell c1({1,1},{1,1},false);
//    Cell c2({2,1},{1,1},false);
//    Cell c3({3,1},{1,1},false);
//    Cell c4({4,1},{1,1},false);
//    w.add(c0);
//    w.add(c1);
//    w.add(c2);
//    w.add(c3);
//    w.add(c4);
//    Cell_group cg = w.create_cell_group();
//    Shape desc(6);
//    Transformation t (.5001,0);
//    Graph g = Coordinates_visibility::create_graph(cg, desc, t).invert();
//    CHECK(g.size() == 5);
//    CHECK(g[c0].size() == 0);
//    CHECK(g[c1].size() == 0);
//    CHECK(g[c2].size() == 0);
//    CHECK(g[c3].size() == 0);
//    CHECK(g[c4].size() == 0);
//}
//
//
//TEST_CASE("Connections shortest path")
//{
//    World w;
//    w.add({0,1});
//    w.add({1,1});
//    w.add({2,1});
//    w.add({3,1});
//    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> w.connection_pattern;
//    auto c = w.create_graph();
//    auto sp = c.get_shortest_path(w[0],w[3]);
//    CHECK(sp.size()==4);
//    CHECK(sp[0]==w[3]);
//    CHECK(sp[1]==w[2]);
//    CHECK(sp[2]==w[1]);
//    CHECK(sp[3]==w[0]);
//}
//
//TEST_CASE("Sub graphs big") {
//    World w;
//    for (int8_t i = 0; i <= 100; i++){
//        Cell c0({i, i}, {(double)i, (double)i},false);
//        w.add(c0);
//    }
//    Cell_group cg1 = w.create_cell_group();
//    Connection_pattern cp;
//    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> cp;
//    auto wc = cp.get_graph(cg1);
//    Cell_group cg;
//    cg.add(w[0]);
//    cg.add(w[50]);
//    cg.add(w[100]);
//    Graph gates(cg);
//    auto sw = wc.get_sub_graphs(gates);
//}
//
//TEST_CASE("Sub graphs disconnected") {
//    World w;
//    w.add({0,1});
//    w.add({1,1});
//    w.add({2,1});
//    w[2].occluded = true;
//    w.add({3,1});
//    w.add({4,1});
//    Cell_group cg1 = w.create_cell_group();
//    Connection_pattern cp;
//    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> w.connection_pattern;
//    auto wc = w.create_graph();
//    vector<Graph> sw = wc.get_sub_graphs();
//    CHECK(sw.size() == 3);
//    CHECK(sw[0].cells.contains(w[0]));
//    CHECK(sw[0].cells.contains(w[1]));
//    CHECK(sw[2].cells.contains(w[3]));
//    CHECK(sw[2].cells.contains(w[4]));
//    CHECK(sw[0][w[0]].size()==1);
//    CHECK(sw[0][w[0]].contains(w[1]));
//    CHECK(sw[0][w[1]].size()==1);
//    CHECK(sw[0][w[1]].contains(w[0]));
//    CHECK(sw[2][w[3]].size()==1);
//    CHECK(sw[2][w[3]].contains(w[4]));
//    CHECK(sw[2][w[4]].size()==1);
//    CHECK(sw[2][w[4]].contains(w[3]));
//}
//
//TEST_CASE("Sub graphs options")
//{
//    World w;
//    w.add({0,1});
//    w.add({1,1});
//    w.add({2,1});
//    w.add({3,1});
//    w.add({4,1});
//    Cell_group cg1 = w.create_cell_group();
//    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> w.connection_pattern;
//    auto wc = w.create_graph();
//    CHECK(wc.size()==5);
//    CHECK(wc[0].size()==1);
//    CHECK(wc[1].size()==2);
//    CHECK(wc[2].size()==2);
//    CHECK(wc[3].size()==2);
//    CHECK(wc[4].size()==1);
//
//    Cell_group cg;
//    cg.add(w[0]);
//    cg.add(w[2]);
//    cg.add(w[4]);
//    Graph gates(cg);
//
//    CHECK(gates.size() == 3);
//    Graph options(cg1);
//    vector<Graph> sw = wc.get_sub_graphs(gates,options);
//    CHECK(sw.size() == 2);
//    CHECK(sw[0].cells.contains(w[0]));
//    CHECK(sw[0].cells.contains(w[1]));
//    CHECK(sw[0].cells.contains(w[2]));
//    CHECK(sw[1].cells.contains(w[2]));
//    CHECK(sw[1].cells.contains(w[3]));
//    CHECK(sw[1].cells.contains(w[4]));
//    CHECK(sw[0][w[0]].size()==1);
//    CHECK(sw[0][w[0]].contains(w[1]));
//    CHECK(sw[0][w[1]].size()==2);
//    CHECK(sw[0][w[1]].contains(w[0]));
//    CHECK(sw[0][w[1]].contains(w[2]));
//    CHECK(sw[0][w[2]].size()==1);
//    CHECK(sw[0][w[2]].contains(w[1]));
//
//    CHECK(sw[1][w[2]].size()==1);
//    CHECK(sw[1][w[2]].contains(w[3]));
//    CHECK(sw[1][w[3]].size()==2);
//    CHECK(sw[1][w[3]].contains(w[2]));
//    CHECK(sw[1][w[3]].contains(w[4]));
//    CHECK(sw[1][w[4]].size()==1);
//    CHECK(sw[1][w[4]].contains(w[3]));
//
//    CHECK(gates[w[0]].size()==2);
//    CHECK(gates[w[2]].size()==3);
//    CHECK(gates[w[4]].size()==2);
//
//    CHECK(options[w[0]].size()==2);
//    CHECK(options[w[0]].contains(w[0]));
//    CHECK(options[w[0]].contains(w[2]));
//    CHECK(options[w[1]].size()==2);
//    CHECK(options[w[1]].contains(w[0]));
//    CHECK(options[w[1]].contains(w[2]));
//    CHECK(options[w[2]].size()==3);
//    CHECK(options[w[2]].contains(w[0]));
//    CHECK(options[w[2]].contains(w[2]));
//    CHECK(options[w[2]].contains(w[4]));
//    CHECK(options[w[3]].size()==2);
//    CHECK(options[w[3]].contains(w[2]));
//    CHECK(options[w[3]].contains(w[4]));
//    CHECK(options[w[4]].size()==2);
//    CHECK(options[w[4]].contains(w[2]));
//    CHECK(options[w[4]].contains(w[4]));
//
//    auto inv = !options;
//
//    CHECK(inv.size()==5);
//    CHECK(inv[w[0]].size()==3);
//    CHECK(inv[w[0]].contains(w[0]));
//    CHECK(inv[w[0]].contains(w[1]));
//    CHECK(inv[w[0]].contains(w[2]));
//    CHECK(inv[w[1]].size()==0);
//    CHECK(inv[w[2]].size()==5);
//    CHECK(inv[w[2]].contains(w[0]));
//    CHECK(inv[w[2]].contains(w[1]));
//    CHECK(inv[w[2]].contains(w[2]));
//    CHECK(inv[w[2]].contains(w[3]));
//    CHECK(inv[w[2]].contains(w[4]));
//    CHECK(inv[w[3]].size()==0);
//    CHECK(inv[w[4]].size()==3);
//    CHECK(inv[w[4]].contains(w[2]));
//    CHECK(inv[w[4]].contains(w[3]));
//    CHECK(inv[w[4]].contains(w[4]));
//
//    Cell_group ncg;
//    ncg.add(w[0]);
//    ncg.add(w[1]);
//    ncg.add(w[2]);
//    Graph ng(ncg);
//    ncg.add(w[3]);
//    ncg.add(w[4]);
//
//    ng.connect(options);
//    CHECK(ng[w[0]].size()==2);
//    CHECK(ng[w[1]].size()==2);
//    CHECK(ng[w[2]].size()==2);
//    ng.add(w[3]);
//    ng.add(w[4]);
//    ng.connect(options);
//    CHECK(ng[w[0]].size()==2);
//    CHECK(ng[w[1]].size()==2);
//    CHECK(ng[w[2]].size()==3);
//    CHECK(ng[w[3]].size()==2);
//    CHECK(ng[w[4]].size()==2);
//}
//
//
//TEST_CASE("Sub graphs")
//{
//    World w;
//    w.add({0,1});
//    w.add({1,1});
//    w.add({2,1});
//    w.add({3,1});
//    w.add({4,1});
//    Cell_group cg1 = w.create_cell_group();
//    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0}]" >> w.connection_pattern;
//    auto wc = w.create_graph();
//    CHECK(wc.size()==5);
//    CHECK(wc[0].size()==1);
//    CHECK(wc[0].contains(w[1]));
//    CHECK(wc[1].size()==2);
//    CHECK(wc[2].size()==2);
//    CHECK(wc[3].size()==2);
//    CHECK(wc[4].size()==1);
//
//    Cell_group cg;
//    cg.add(w[0]);
//    cg.add(w[2]);
//    cg.add(w[4]);
//    Graph gates(cg);
//    CHECK(gates.size() == 3);
//    vector<Graph> sw = wc.get_sub_graphs(gates);
//    CHECK(sw.size() == 2);
//    CHECK(sw[0].cells.contains(w[0]));
//    CHECK(sw[0].cells.contains(w[1]));
//    CHECK(sw[0].cells.contains(w[2]));
//    CHECK(sw[1].cells.contains(w[2]));
//    CHECK(sw[1].cells.contains(w[3]));
//    CHECK(sw[1].cells.contains(w[4]));
//    CHECK(sw[0][w[0]].size()==1);
//    CHECK(sw[0][w[0]].contains(w[1]));
//    CHECK(sw[0][w[1]].size()==2);
//    CHECK(sw[0][w[1]].contains(w[0]));
//    CHECK(sw[0][w[1]].contains(w[2]));
//    CHECK(sw[0][w[2]].size()==1);
//    CHECK(sw[0][w[2]].contains(w[1]));
//
//    CHECK(sw[1][w[2]].size()==1);
//    CHECK(sw[1][w[2]].contains(w[3]));
//    CHECK(sw[1][w[3]].size()==2);
//    CHECK(sw[1][w[3]].contains(w[2]));
//    CHECK(sw[1][w[3]].contains(w[4]));
//    CHECK(sw[1][w[4]].size()==1);
//    CHECK(sw[1][w[4]].contains(w[3]));
//
//    CHECK(gates[w[0]].size()==2);
//    CHECK(gates[w[2]].size()==3);
//    CHECK(gates[w[4]].size()==2);
//}
//
//
//TEST_CASE("shortest path") {
//    World w;
//    w.add({{0, 0}, {0, 0}, false}); // w[0]
//    w.add({{0, 1}, {0, 1}, false}); // w[1]
//    w.add({{0, 2}, {0, 2}, false}); // w[2]
//    w.add({{1, 0}, {1, 0}, false}); // w[3]
//    w.add({{1, 1}, {1, 1}, true});  // w[4]
//    w.add({{1, 2}, {1, 2}, false}); // w[5]
//    w.add({{2, 0}, {2, 0}, true});  // w[6]
//    w.add({{2, 1}, {2, 1}, true});  // w[7]
//    w.add({{2, 2}, {2, 2}, false}); // w[8]
//    w.add({{3, 0}, {3, 0}, false}); // w[9]
//    w.add({{3, 1}, {3, 1}, false}); // w[10]
//    w.add({{3, 2}, {3, 2}, false}); // w[11]
//    "[{\"x\":-1,\"y\":0},{\"x\":1,\"y\":0},{\"x\":0,\"y\":-1},{\"x\":0,\"y\":1}]" >> w.connection_pattern;
//    CHECK(w.size() == 12);
//    Graph g = w.create_graph();
//    CHECK(g.size() == 12);
//    CHECK(g.get_shortest_path(w[3],w[9]).size() == 9);
//    CHECK(g.get_shortest_path(w[0],w[9]).size() == 8);
//    CHECK(g.get_shortest_path(w[1],w[9]).size() == 7);
//    CHECK(g.get_shortest_path(w[2],w[9]).size() == 6);
//    CHECK(g.get_shortest_path(w[5],w[9]).size() == 5);
//    CHECK(g.get_shortest_path(w[8],w[9]).size() == 4);
//    CHECK(g.get_shortest_path(w[11],w[9]).size() == 3);
//    CHECK(g.get_shortest_path(w[10],w[9]).size() == 2);
//    CHECK(g.get_shortest_path(w[9],w[9]).size() == 1);
//}