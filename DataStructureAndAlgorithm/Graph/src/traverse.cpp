#include"A.h"
#include<string>
#include <vector>
#include <stack>
#include<queue>

//ÁÚ½Ó¾ØÕó
template<typename VertexType, typename DataType>
struct Graph
{
	Graph() { }

	template<typename VertexType, typename DataType>
	Graph(const Graph<VertexType, DataType> graph):
		vertexes(graph.vertexes), adjacency_matrix(graph.adjacency_matrix),num_vertex(graph.num_vertex){ }

	std::vector<VertexType> vertexes;
	std::vector<std::vector<DataType>> adjacency_matrix;
	int num_vertex;
};

template<typename VertexType, typename DataType>
void Dfs(Graph<VertexType, DataType>& graph, int i, std::vector<bool>& visit)
{
	visit[i] = true;

	std::cout << graph.vertexes[i];

	for (int j = 0; j < graph.num_vertex; j++)
	{
		if (graph.adjacency_matrix[i][j] != 0 && !visit[j])
			DFS(graph, j, visit);
	}
}


template<typename VertexType, typename DataType>
void DfsTrave(Graph<VertexType, DataType> graph)
{
	int num_vertex = graph.num_vertex;

	std::vector<bool> visit(num_vertex, false);

	for (int i = 0; i < num_vertex; ++i)
	{
		if (!visit[i])
			Dfs(graph, i, visit);
	}
}

template<typename VertexType, typename DataType>
void Bfs(Graph<VertexType, DataType> graph)
{
	int num_vertex = graph.num_vertex;
	std::vector<bool> visit(num_vertex,false);

	std::queue<int> que;
	for (int i = 0; i < num_vertex; i++)
	{
		if (!visit[i])
		{
			std::cout << graph.vertexes[i];
			visit[i] = true;

			que.push(i);

			while (!que.empty())
			{
				int k = que.front();
				que.pop();

				for (int j = 0; j < num_vertex; j++)
				{
					if (graph.adjacency_matrix[k][j] != 0 && !visit[j])
					{
						std::cout << graph.vertexes[j];
						visit[j] = true;
						que.push(j);
					}
				}
			}
		}
	}
	
}

template<typename VertexType, typename DataType>
void DfsNoRecursive(Graph<VertexType, DataType> graph)
{
	std::stack<int> stk;
	int num_vertex = graph.num_vertex;

	std::vector<bool> visit(num_vertex, false);

	int i = 0;
	stk.push(i);

	while (!stk.empty())
	{
		int k = stk.top();
		if (!visit[k])
			std::cout << graph.vertexes[k];

		stk.pop();
		visit[k] = true;

		for (int j = 0; j < num_vertex; j++)
		{
			if (graph.adjacency_matrix[k][j] != 0 && !visit[j])
			{
				stk.push(j);
				break;
			}
		}
	}
}

//ÁÚ½Ó±í
void Dfs(std::vector<std::vector<int>>& graph, int i, std::vector<bool>& visit)
{
	visit[i] = true;

	std::cout << i;
	
	for (int j = 0; j < graph[i].size(); j++)
	{
		if (!visit[graph[i][j]])
			Dfs(graph, graph[i][j], visit);
	}
}

void DfsTrave(std::vector<std::vector<int>> graph)
{
	int num_vertex = graph.size();
	std::vector<bool> visit(num_vertex, false);

	for (int i = 0; i < num_vertex; i++)
	{
		if (!visit[i])
			Dfs(graph, i, visit);
	}
}

void Bfs(std::vector<std::vector<int>> graph)
{
	std::queue<int> que;
	int num_vertex = graph.size();

	std::vector<bool> visit(num_vertex, false);

	for (int i = 0; i < num_vertex; i++)
	{
		if (!visit[i])
		{
			std::cout << i;
			visit[i] = true;
			que.push(i);

			while (!que.empty())
			{
				int k = que.front();
				que.pop();

				for (int j = 0; j < graph[k].size(); j++)
				{
					if (!visit[graph[k][j]])
					{
						std::cout << graph[k][j];
						visit[graph[k][j]] = true;
						que.push(graph[k][j]);
					}
				}
			}
		}
	}
}
int main()
{
	Graph<std::string, int> graph;
	graph.num_vertex = 4;
	graph.vertexes = { "a","b","c","d" };
	graph.adjacency_matrix = { {0,0,1,1},{0,0,0,1},{1,0,0,1},{1,1,1,0} };
	DfsNoRecursive(graph);
	Bfs(graph);

	std::cout << std::endl;
	std::vector<std::vector<int>> ad = { {2,3},{3},{0,3},{0,1,2} };
	DfsTrave(ad);
	Bfs(ad);

}