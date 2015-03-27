#include <fstream>
#include <iostream>

#include "Graph.h"
#include "util.h"


Edge::Edge(int e0, int e1, double length) {
    end[0] = e0;
    end[1] = e1;
    len = length;
    int_len = nint(len);
    useless = false;
}

//Load a graph from a file
Graph::Graph(const char *tsp_file) {
    fstream fin;
    fin.open(tsp_file);

    string temp;
    getline(fin, temp); //Skip name
    getline(fin, temp); //Skip comment
    getline(fin, temp); //Skip type

    int num_points;
    fin >> temp >> temp >> num_points; //Read edge count
    getline(fin, temp); //Skip weight type
    getline(fin, temp); //Skip weight type
    getline(fin, temp); //Skip section type


    //test
    // num_points = 50;
    // vector<double> x(num_points);
    // vector<double> y(num_points);
    // CO759_build_xy(num_points, x, y, 100);
    // points.reserve(num_points);
    // for(int i = 0; i < num_points; i++) {
    //     int index;
    //     double x, y;
    //     fin >> index >> x >> y;
    //     points.push_back(Point2D(x,y));
    // }

    //
    points.reserve(num_points);
    for(int i = 0; i < num_points; i++) {
        int index;
        double x, y;
        fin >> index >> x >> y;
        points.push_back(Point2D(x,y));
    }

    fin.close();

    //Assume it is a complete graph
    edges.reserve(num_points * (num_points - 1) / 2);
    int_lengths.resize(num_points);
    lengths.resize(num_points);
    for(int i = 0; i < num_points; i++) {
        int_lengths[i].resize(num_points, 0);
        lengths[i].resize(num_points, 0.0);
    }

    for(int i = 0; i < num_points; i++) {
        for(int j = i; j < num_points; j++) {
            if(i != j) {
                double len = (points[i] - points[j]).length();
                edges.push_back(Edge(i, j, len));
                int_lengths[i][j] = nint(len); int_lengths[j][i] = nint(len);
                lengths[i][j] = len; lengths[j][i] = len;
                // cout << num_points << endl;
                // cout << edge_count() << endl;
            }
        }
    }

    kd_tree = new KdTree(points);
}

unsigned long Graph::sum_edge_weights(vector<int> &edge_indices) {
    unsigned long tot = 0;
    for(int i = 0; i < (int)edge_indices.size(); i++) {
        tot += edges[edge_indices[i]].len;
    }

    return tot;
}

void Graph::print_edges() {
    for (int i = 0; i < edges.size(); ++i)
    {
        cout << edges[i].end[0] << ", " << edges[i].end[1] << ": " << edges[i].int_len<< endl;
    }
}

int Graph::count_useless() {
    int sum = 0;
    for(int i = 0; i < (int)edges.size(); i++) {
        if(edges[i].useless) sum++;
    }
    return sum;
}
