#include <iostream>

const size_t MAX_LABEL = 10;

struct Edge {
    char to[MAX_LABEL + 1];
    char from[MAX_LABEL + 1];
};

struct Graph {
    unsigned int verticesCount;
    unsigned int edgesCount;
    Edge* edges_list;
};

Graph initialize_graph(int m) {
    Graph graph;
    graph.edgesCount = m;
    graph.edges_list = new Edge[m];
    graph.verticesCount = 0;
    return graph;
}

void deleteGraph(Graph& graph) {
    delete[] graph.edges_list;
}
void myStrcpy(char* dest, const char* src) {
    if (!src || !dest)
        return;
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

bool invalidInput (const char* to, const char* from, const Graph& graph) {
    return !to || !from || graph.verticesCount == 0 || graph.verticesCount >= graph.edgesCount;
}

int myStrcmp(const char* first, const char* second)
{
    if (!first || !second)
        return 0; //some error value

    //we skip the common prefix, but not the terminating zero!
    while ((*first) && (*second) && ((*first) == (*second))) //acutally the (*second) check can be missed here.
    {
        first++;
        second++;
    }

    return (*first - *second);

}

bool isNewVertex (const char* vertex, const Graph& graph ) {
    unsigned int i;

    for( i = 0; i < graph.verticesCount; i++ ) {
        if((myStrcmp(graph.edges_list[i].to, vertex) == 0) ||
        (myStrcmp (graph.edges_list[i].from, vertex)) == 0) {
            return false;
        }
    }

    return true;
}

void addEdge(const char* to, const char* from, Graph& graph) {
    if ( invalidInput (to, from, graph) ) {
        std::cout << "Invalid input" << std:: endl;
        return;
    }

    Edge edge1;
    myStrcpy(edge1.from, from);
    myStrcpy(edge1.to, to);

    unsigned int numberOfEdges = graph.verticesCount + 1;

    Edge* newEdges = new Edge[numberOfEdges];

    unsigned int i;

    for(i = 0; i < numberOfEdges - 1; i++) {
        newEdges[i] = graph.edges_list[i];
    }

    if( isNewVertex (edge1.to, graph) ) {
        graph.verticesCount++;
    }

    if( isNewVertex (edge1.from, graph) ) {
        graph.verticesCount++;
    }

    newEdges[i] = edge1;
    deleteGraph(graph);
    graph.edges_list = newEdges;
    graph.verticesCount++;

}

void createGraph(Graph& graph) {
    unsigned int m;
    std::cout << "Enter the number of edges: ";
    std::cin >> m;

    graph = initialize_graph(m);

    std::cout << "Enter " << m << " pairs of vertex labels:\n";

    char to[MAX_LABEL + 1], from[MAX_LABEL + 1];

    for (unsigned int i = 0; i < m; ++i) {
        std::cin >> to >> from;
        addEdge(to, from, graph);
    }


}

int getDegree(const char* vertex, const Graph& graph) {
    int degree = 0;
    for (unsigned int i = 0; i < graph.verticesCount; ++i) {
        if (myStrcmp(vertex, graph.edges_list[i].from) == 0)
            degree++;
    }
    return degree;
}

bool isComplete(Graph& graph) {
    for (unsigned int i = 0; i < graph.verticesCount; ++i) {
        for (unsigned int j = i + 1; j < graph.verticesCount; ++j) {
            bool connected = false;
            for (unsigned int k = 0; k < graph.verticesCount; ++k) {
                if (myStrcmp(graph.edges_list[k].from, graph.edges_list[i].from) == 0 &&
                    myStrcmp(graph.edges_list[k].to, graph.edges_list[j].to) == 0) {
                    connected = true;
                    break;
                }
            }
            if (!connected)
                return false;
        }
    }
    return true;
}

void removeEdge(const char* to, const char* from, Graph& graph) {
    for (unsigned int i = 0; i < graph.verticesCount; ++i) {
        if (myStrcmp(graph.edges_list[i].from, from) == 0 &&
            myStrcmp(graph.edges_list[i].to, to) == 0) {

            graph.edges_list[i] = graph.edges_list[graph.verticesCount - 1];
            graph.verticesCount--;

            std::cout << "Edge removed successfully.\n";
            return;
        }
    }
    std::cout << "Edge not found in the graph.\n";
}



int main() {
    Graph graph;
    createGraph(graph);

    char vertex[MAX_LABEL + 1];
    std::cout << "Enter a vertex label to get its degree: ";
    std::cin >> vertex;
    std::cout << "Degree of vertex " << vertex << " is: " << getDegree(vertex, graph) << std::endl;

    if (isComplete(graph))
        std::cout << "Graph is complete.\n";
    else
        std::cout << "Graph is not complete.\n";

    char to[MAX_LABEL + 1], from[MAX_LABEL + 1];
    std::cout << "Enter the edge to remove (from to): ";
    std::cin >> from >> to;
    removeEdge(to, from, graph);

    deleteGraph(graph);

    return 0;
}
