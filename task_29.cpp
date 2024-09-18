#include <vector>
#include <queue>

using std::vector;
using std::queue;

using adjacency_list = vector<vector<int>>;

class Solution {
public:
    bool bfs(adjacency_list& graph,vector<int>& colors,int i){
        queue<int> vertices;
        vertices.push(i);

        // Coloring of vertex
        colors[i] = 1;

        while(!vertices.empty()){
            int id = vertices.front();
            vertices.pop();

            for(int adj: graph[id]){
                if(colors[adj] == 0){
                    colors[adj] = -colors[id];
                    vertices.push(adj);
                }
                else if(colors[adj] == colors[id])
                    return false;
            }            
        }

        return true;
    }

    bool isBipartite(adjacency_list& graph) {
        vector<int> colors(graph.size(), 0);

        for(int i = 0; i < graph.size(); i++){
            if(colors[i] == 0) {
                if(!bfs(graph, colors, i))
                    return false;
            }
        }

        return true;
    }
};
