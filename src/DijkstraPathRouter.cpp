#include "DijkstraPathRouter.h"
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <queue>

struct CDijkstraPathRouter::SImplementation {

	struct Vertex 
	{
		std::any tag;
		std::unordered_map < TVertexID, double > edges;

		Vertex(std::any Tag) {
			tag = Tag;
		}
	};
	
	std::vector < std::shared_ptr<Vertex> > vertices;

	std::size_t VertexCount() const noexcept
	{
		return vertices.size();
	}

	TVertexID AddVertex(std::any tag) noexcept
	{
		vertices.push_back(std::make_shared<Vertex>(tag));
		return VertexCount() - 1;
	}

	std::any GetVertexTag(TVertexID id) const noexcept
	{
		try
        {
            return vertices[id]->tag;
        }
        catch (const std::out_of_range &excpt)
        {
            return std::any();
        }
	}

	bool AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir) noexcept {
		if (src < VertexCount() && dest < VertexCount()) 
		{
			vertices[src]->edges.insert({dest, weight});

			if (bidir)
				vertices[dest]->edges.insert({src, weight});
	
			return true;
		}

		return false;
	}

	double FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept { 
		std::unordered_map<TVertexID, double> d;
		std::unordered_map<TVertexID, TVertexID> pi;
		std::priority_queue<std::pair<double, TVertexID>, std::vector<std::pair<double, TVertexID>>, std::greater<>> Q;

		std::size_t V = VertexCount();

		for (std::size_t i = 0; i < V; i++)
		{
			d[i] = std::numeric_limits<double>::max();
			pi[i] = -1;
		}
		d[src] = 0;

		Q.push(std::make_pair(0, src));

		while (!Q.empty())
		{
			TVertexID u = Q.top().second;
			Q.pop();
			
			auto& edges = vertices[u]->edges;

			for (const auto& edge : edges)
			{
				TVertexID v = edge.first;
				double weight = edge.second;

				if (d[v] > d[u] + weight)
				{
					d[v] = d[u] + weight;
					pi[v] = u;
					Q.push(std::make_pair(d[v], v));
				}
			}
		}

		TVertexID current = dest;
		while (current != -1)
		{
			path.insert(path.begin(), current);
			current = pi[current];
		}

		return d[dest];
  }

};

CDijkstraPathRouter::CDijkstraPathRouter() {
	DImplementation = std::make_unique<SImplementation>();
}

CDijkstraPathRouter::~CDijkstraPathRouter() {};

std::size_t CDijkstraPathRouter::VertexCount() const noexcept {
	return DImplementation->VertexCount();
}

CPathRouter::TVertexID CDijkstraPathRouter::AddVertex(std::any tag) noexcept { // nodeID <-> vertexID 
	return DImplementation->AddVertex(tag);
}

std::any CDijkstraPathRouter::GetVertexTag(TVertexID id) const noexcept {
	return DImplementation->GetVertexTag(id);
}

bool CDijkstraPathRouter::AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir) noexcept {
	return DImplementation->AddEdge(src, dest, weight, bidir);
}


bool CDijkstraPathRouter::Precompute(std::chrono::steady_clock::time_point deadline) noexcept {
	return true;
}

double CDijkstraPathRouter::FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept {
	return DImplementation->FindShortestPath(src, dest, path);
}

