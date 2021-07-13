#include <iostream>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Utils/PropertyManager.hh>
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

	auto cog = OpenMesh::makeTemporaryProperty<OpenMesh::VertexHandle, TriMesh::Point>(triMesh, "cog");
	int iterations(3);
	for (int i = 0; i < iterations; ++i)
	{
		for (const auto& vh : triMesh.vertices())
		{
			cog[vh] = { 0,0,0 };
			int valence(0);
			for (const auto& vvh :triMesh.vv_range(vh))
			{
				cog[vh] += triMesh.point(vvh);
				++valence;
			}
			cog[vh] /= valence;
		}
	}

	auto cogPro = OpenMesh::getProperty<OpenMesh::VertexHandle, TriMesh::Point>(triMesh, "cog");
	for (const auto& vh : triMesh.vertices())
	{
		if (!triMesh.is_boundary(vh))
		{
			triMesh.point(vh) = cogPro[vh];
		}
	}

	// write ployMesh to output.obj
	try
	{
		if (!OpenMesh::IO::write_mesh(triMesh, "../source/output_2.off"))
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
