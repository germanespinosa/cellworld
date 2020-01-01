#pragma once

#include <map>
#include <vector>

namespace cellworld {

    struct Connection {
        bool is_connected(uint32_t) const;
        bool add(uint32_t);
        bool remove(uint32_t);
        double eigen_centrality;
        uint32_t size() const;
        uint32_t operator [](uint32_t) const;
    private:
        std::vector<uint32_t> _connections;
    };

    struct Connections {
        Connections();
        bool add(uint32_t, uint32_t );
        bool remove(uint32_t, uint32_t );
        /*bool load(const std::string &);
        bool load();*/
        bool save(const std::string &) const;
        //bool save();
        const Connection &operator [](uint32_t) const;
        bool process_eigen_centrality();
        bool process_eigen_centrality(uint32_t, double);
        void clear();
        uint32_t size() const;
    private:
        std::vector<Connection> _connections;
    };
}