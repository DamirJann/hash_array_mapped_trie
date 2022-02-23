#include "../include/visualize.h"
int counter = 0;
const int MAX_EDGE_COUNT = 10000;
const int MAX_HEIGHT_COUNT = 10000;



void visualize(ofstream &f, Trie *trie) {
    vector <height> heights = {trie->getRoot()};
    vector <edge> edges;
    walk_and_collect(heights[0], heights, edges);

    typedef pair<int, int> Edge;
    Edge used_by[MAX_EDGE_COUNT];

    const int nedges = edges.size();
    int weights[MAX_EDGE_COUNT];
    std::fill(weights, weights + nedges, 34);

    using namespace boost;

    string names[MAX_HEIGHT_COUNT];
    for (size_t i = 0; i < heights.size(); i++) {
        names[i] = heights[i].getLabel();
    }

    typedef adjacency_list <vecS, vecS, directedS,
    property<vertex_color_t, default_color_type>,
    property<edge_weight_t, int>
    > Graph;
    Graph g(used_by, used_by, weights, heights.size());

    std::map <Graph::edge_descriptor, std::string> ename;
    for (auto &edge: edges) {
        ename[add_edge(edge.from, edge.to, g).first] = edge.label;
    }

    write_graphviz(f, g, make_label_writer(names), make_label_writer(boost::make_assoc_property_map(ename)));
}

struct height {
    height(Node *v) {
        this->v = v;
        id = counter++;
    }

    int id;
    Node *v;

    string getLabel() {
        switch (v->type) {
            case I_NODE: {
                return "I" + to_string(id);
            }
            case C_NODE:
                return "C" + to_string(id);
            case S_NODE:
                return "S(" +
                       to_string(static_cast<SNode*>(v))->key) + to_string(static_cast<SNode*>(v).key)
                ")";
            default:
                return "Unknown";
        }
    }
};


struct edge {
    string label;
    int from;
    int to;
};


void walk_and_collect(height currHeight, vector<height> &hs, vector<edge> &es) {
    if (currHeight.v->type == I_NODE) {
        height c(static_cast<INode *>(currHeight.v)->main);
        hs.push_back(c);

        es.push_back({"", currHeight.id, c.id});

        for (uint8_t path = 0b00000; path < 0b11111; path++) {
            CNode *n = reinterpret_cast<CNode *>(c.v);
            if (n->getSubNode(path) != nullptr) {
                height child(n->getSubNode(path));
                hs.push_back(child);
                es.push_back({to_string(path), c.id, child.id});
                walk_and_collect(child, hs, es);
            }
        }
    }
}
