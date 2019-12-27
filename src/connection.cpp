//
// Created by german on 12/14/19.
//
#include <connection.h>
#include <iostream>
using namespace std;
using namespace cellworld;

World_connections::World_connections(World &w, std::vector<Coordinates> patterns) {
    for (unsigned int i=0; i < w.size(); i++){
        vector<uint32_t> cell_conns;
        for (unsigned int j=0; j < patterns.size(); j++) {
            Coordinates c = w[i].coordinates + patterns[j];
            int32_t id = w.find(c);
            if (id >=0 && !w[id].occluded){
                cell_conns.push_back(id);
            }
        }
        _connections.push_back(cell_conns);
    }
}

std::vector<uint32_t> &World_connections::cell_connections(uint32_t cell_id) {
    return _connections[cell_id];
}

bool World_connections::process_eigen_centrality() {
    return process_eigen_centrality(100, 0.000001);
}

bool World_connections::process_eigen_centrality(uint32_t max_iterations, double tolerance) {
    double nnodes = (double)_connections.size();
    vector<double> xlast(_connections.size());
    double err = nnodes * 100;
    for (uint32_t index = 0; index < _connections.size(); index++) eigen_centrality.push_back(1.0/nnodes);
    for (uint32_t iteration = 0; iteration < max_iterations && err > nnodes * tolerance; iteration++){
        for (uint32_t index = 0; index < _connections.size(); index++)
            xlast[index] = eigen_centrality[index];
        for (uint32_t n = 0; n < _connections.size(); n++)
            for (uint32_t nbr = 0; nbr < _connections[n].size(); nbr++)
                eigen_centrality[_connections[n][nbr]] += xlast[n];
        double d=0;
        for (uint32_t index = 0; index < _connections.size(); index++)
            d += pow(eigen_centrality[index],2);
        d = pow(d,.5);
        double s = d==0?1:1.0/d;
        for (uint32_t index = 0; index < _connections.size(); index++) eigen_centrality[index] *= s;
        err=0;
        for (uint32_t index = 0; index < _connections.size(); index++)
            err += abs(eigen_centrality[index]-xlast[index]);
    }
    return err < nnodes * tolerance;
}

/*
 * def eigenvector_centrality(G, max_iter=100, tol=1.0e-6, nstart=None,
                           weight='weight'):
    from math import sqrt
        x = dict([(n,1.0/len(G)) for n in G])
    else:
        x = nstart

    # normalize starting vector
    s = 1.0/sum(x.values())
    for k in x:
        x[k] *= s
    nnodes = G.number_of_nodes()

    # make up to max_iter iterations
    for i in range(max_iter):
        xlast = x
        x = dict.fromkeys(xlast, 0)

        # do the multiplication y^T = x^T A
        for n in x:
            for nbr in G[n]:
                x[nbr] += xlast[n] * G[n][nbr].get(weight, 1)

        # normalize vector
        try:
            s = 1.0/sqrt(sum(v**2 for v in x.values()))

        # this should never be zero?
        except ZeroDivisionError:
            s = 1.0
        for n in x:
            x[n] *= s

        # check convergence
        err = sum([abs(x[n]-xlast[n]) for n in x])
        if err < nnodes*tol:
            return x

    raise nx.NetworkXError("""eigenvector_centrality():
power iteration failed to converge in %d iterations."%(i+1))""")
 */

