#pragma once

#include <vector>



inline std::vector<Vertex> convertVertexStructres(std::vector<VertexMaths>& vertices) {
	std::vector<Vertex> verticesToReturn;
	for (auto& vertex : vertices) {
		Vertex v;
		v.n = vertex.n;
		v.pos = vertex.pos;
		verticesToReturn.push_back(v);
	}
	return verticesToReturn;
}