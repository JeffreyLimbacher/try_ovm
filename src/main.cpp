
#include <fstream>
#include <optional>

#include <OpenVolumeMesh/Mesh/HexahedralMesh.hh>

using namespace OpenVolumeMesh;

struct HexDescription {
	std::vector<Geometry::Vec3d> points;
	std::vector<std::array<int, 8>> elems;
};

template<class T, size_t N, size_t ...I>
std::istream & parse(std::istream& o, std::array<T, N>& arr, std::index_sequence<I...> v) {
	(o >> ... >> std::get<I>(arr));
	return o;
}

template<class T, size_t N> 
std::istream& parse(std::istream& o, std::array<T, N>& arr) {
	parse(o, arr, std::make_index_sequence<N>{});
	return o;
}

std::optional<HexDescription> parse_k(const std::string& filename) {
	std::string l;
	std::ifstream k_file(filename);
	if (!k_file.is_open())
		return std::nullopt;

	bool pelems = false;
	bool pnodes = false;
	HexDescription hex;
	while (std::getline(k_file, l)) {
		if (l.size() == 0 || l[0] == '$') {
			continue;
		}
		if (l[0] == '*') {
			pnodes = l.substr(0, 5) == "*NODE";
			pelems = l.substr(0, 14) == "*ELEMENT_SOLID";
			continue;
		}
		std::stringstream ls(l);
		if (pnodes) {
			int nid;
			double x, y, z;
			ls >> nid >> x >> y >> z;
			Vec3d d;
			hex.points.emplace_back( x,y,z );
		}
		else if (pelems) {
			std::array<int, 8> e;
			int junk;
			ls >> junk >> junk;
			parse(ls, e);
			for (size_t i = 0; i < 8; i++) {
				e[i] --; //make zero indexable
			}
			hex.elems.emplace_back(e);
		}
	}
	return hex;
}

int main() {
	auto hex_opt = parse_k("../resources/out.k");
	assert(hex_opt.has_value());

	GeometricHexahedralMeshV3d mesh;
	auto& hp = hex_opt->points;
	std::vector<VertexHandle> verts;
	verts.reserve(hp.size());

	std::transform(hp.begin(), hp.end(), std::back_insert_iterator(verts), [&mesh](const auto& v) {
		return mesh.add_vertex(v);
	});



	return 0;
}