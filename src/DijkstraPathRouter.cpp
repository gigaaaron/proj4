#include "DijkstraPathRouter.h"
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <iostream>

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
		return VertexCount();
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
		if (src <= VertexCount() && dest <= VertexCount()) 
		{
			vertices[src]->edges.insert({dest, weight});

			if (bidir)
				vertices[dest]->edges.insert({src, weight});
	
			return true;
		}

		return false;
	}

	double FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept { // THIS IS FOR YOU :D 
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

