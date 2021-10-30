#include<iostream>
#include<vector>

#include<OpenMesh/Core/IO/MeshIO.hh>
#include<OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include<OpenMesh/Core/Mesh/Traits.hh>

struct Traits :public OpenMesh::DefaultTraits
{
	VertexTraits
	{
	private:
		Point cog_;
		int num;
	public:
		VertexT() :cog_(Point(0.0f, 0.0f, 0.0f)) {}
		const Point& cog()const { return cog_; }
		void set_cog(const Point& _p) { cog_ = _p; }
	};
};
typedef OpenMesh::TriMesh_ArrayKernelT<Traits>	TriMesh;
typedef OpenMesh::TriMesh_ArrayKernelT<>		Mesh;

#define SIZEOF(entity,b)\
std::cout << _prefix << "size of " << #entity << "; "\
			<< sizeof(entity) << std::endl;\
b += sizeof(entity)

template<typename mesh>
void print_size(const std::string& _prefix = "")
{
	size_t total(0);
	SIZEOF(typename mesh::Vertex, total);
	SIZEOF(typename mesh::Halfedge, total);
	SIZEOF(typename mesh::Edge, total);
	SIZEOF(typename mesh::Face, total);
	std::cout << _prefix << "total" << total << std::endl;
}

int main()
{
	std::cout << "Enhanced mesh size statistics\n";
	print_size<TriMesh>();
	std::cout << "Default  mesh size statistics\n";
	print_size<Mesh>();

	auto spTriMesh = std::make_shared<TriMesh>();
	try
	{
		if (!OpenMesh::IO::read_mesh(*spTriMesh, "../source/output_0.off"))
		{
			std::cerr << "Cannot read ployMesh to file 'output_0.off'" << std::endl;
			return EXIT_FAILURE;
		}
	}
	catch (const std::exception& x)
	{
		std::cerr << x.what() << std::endl;
		return EXIT_FAILURE;
	}

	TriMesh::VertexIter v_it;
	for (int iterator = 0; iterator < 3; ++iterator)
	{
		TriMesh::Point cog(0.0f, 0.0f, 0.0f);
		TriMesh::Scalar valence(0.0f);
		for (v_it = spTriMesh->vertices_begin(); v_it != spTriMesh->vertices_end(); ++v_it)
		{
			cog += spTriMesh->point(*v_it);
			++valence;
		}
		spTriMesh->data(*v_it).set_cog(cog / valence);
	}
	for (v_it = spTriMesh->vertices_begin(); v_it != spTriMesh->vertices_end(); ++v_it)
	{
		if (!spTriMesh->is_boundary(*v_it))
		{
			spTriMesh->set_point(*v_it, spTriMesh->data(*v_it).cog());
		}
	}

	try
	{
		if (!OpenMesh::IO::write_mesh(*spTriMesh, "../source/output_4.off"))
		{
			std::cerr << "Cannot write ployMesh to file 'output.off'" << std::endl;
			return EXIT_FAILURE;
		}
	}
	catch (std::exception& x)
	{
		std::cerr << x.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}