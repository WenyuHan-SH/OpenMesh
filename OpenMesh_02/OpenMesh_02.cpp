#include <iostream>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
using namespace std;

typedef OpenMesh::TriMesh_ArrayKernelT<>  TriMesh;
int main()
{
	TriMesh triMesh;
	try
	{
		if (!OpenMesh::IO::read_mesh(triMesh, "../source/output_0.off"))
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

	std::vector<TriMesh::Point> cogs;
	int iterations(3);
	for (int i = 0; i < iterations; ++i)
	{
		for (auto v_it = triMesh.vertices_begin(); v_it != triMesh.vertices_end(); ++v_it)
		{
			TriMesh::Point cog(0.0, 0.0, 0.0);
			TriMesh::Scalar valence(0.0);
			for (auto vv_it = triMesh.vv_iter(*v_it); vv_it.is_valid(); ++vv_it)
			{
				cog += triMesh.point(*vv_it);
				++valence;
			}
			cogs.push_back(cog / valence);
		}
	}
	
	auto cog_it = cogs.begin();
	for (auto v_it = triMesh.vertices_begin(); v_it != triMesh.vertices_end(); ++v_it)
	{
		if (!triMesh.is_boundary(*v_it))
		{
			triMesh.set_point(*v_it, *cog_it);
		}
		++cog_it;
	}

	// write ployMesh to output.obj
	try
	{
		if (!OpenMesh::IO::write_mesh(triMesh, "../source/output_1.off"))
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
