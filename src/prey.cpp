#include "../include/prey.h"
#include <stdlib.h>

//
// Created by german on 11/13/19.
//

using namespace cellworld;
using namespace std;

Prey::Prey(World &w, Visibility &v, uint32_t predator_id) : world(w), visibility(v), predator_id(predator_id){
    data.id = 0;
    data.color = Green;
    data.status= 1;
    int r;
    while( world[r = rand() % world.size()].occluded);
    data.coordinates = world[r].coordinates;
    action.iteration = 0;
    action.action = {2,0};
}

void Prey::update_state(State state){
    action.iteration = state.iteration;

    bool prey_visible = false;
    Cell &predator_cell  =world[data.coordinates];
    auto conns = world[data.coordinates].connections;
    if (state.agents.size()>predator_id) {
        Cell &prey_cell = world[state.agents[predator_id].coordinates];
        if (visibility.is_visible(predator_cell, prey_cell)){
            action.action = predator_cell.coordinates - data.coordinates;
            double min_dist = world.distance(predator_cell,prey_cell);
            for (unsigned int i=1;i<conns.size();i++){
                double dist = world.distance( world[conns[i]],prey_cell);
                if (dist<min_dist) {
                    action.action = world[conns[i]].coordinates - data.coordinates;
                }
            }
            data.color = Red;
            return ;
        }
    }
    data.color = Green;
    int r = rand() % conns.size();
    action.action = world[conns[r]].coordinates - data.coordinates;
}

bool Prey::get_action(){
    return true;
}

