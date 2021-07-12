#include <iostream>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
using namespace std;

typedef OpenMesh::PolyMesh_ArrayKernelT<>  MyMesh;
typedef OpenMesh::TriMesh_ArrayKernelT<OpenMesh::DefaultTraits> TriMesh;
int main()
{
	MyMesh mesh;
	MyMesh::VertexHandle vhandle[8];
	vhandle[0] = mesh.add_vertex(MyMesh::Point(-1, -1, 1));
	vhandle[1] = mesh.add_vertex(MyMesh::Point(1, -1, 1));
	vhandle[2] = mesh.add_vertex(MyMesh::Point(1, 1, 1));
	vhandle[3] = mesh.add_vertex(MyMesh::Point(-1, 1, 1));
	vhandle[4] = mesh.add_vertex(MyMesh::Point(-1, -1, -1));
	vhandle[5] = mesh.add_vertex(MyMesh::Point(1, -1, -1));
	vhandle[6] = mesh.add_vertex(MyMesh::Point(1, 1, -1));
	vhandle[7] = mesh.add_vertex(MyMesh::Point(-1, 1, -1));
	// generate (quadrilateral) faces
	std::vector<MyMesh::VertexHandle>  face_vhandles;
	face_vhandles.clear();
	face_vhandles.push_back(vhandle[0]);
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[2]);
	face_vhandles.push_back(vhandle[3]);
	mesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[7]);
	face_vhandles.push_back(vhandle[6]);
	face_vhandles.push_back(vhandle[5]);
	face_vhandles.push_back(vhandle[4]);
	mesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[0]);
	face_vhandles.push_back(vhandle[4]);
	face_vhandles.push_back(vhandle[5]);
	mesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[2]);
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[5]);
	face_vhandles.push_back(vhandle[6]);
	mesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[3]);
	face_vhandles.push_back(vhandle[2]);
	face_vhandles.push_back(vhandle[6]);
	face_vhandles.push_back(vhandle[7]);
	mesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[0]);
	face_vhandles.push_back(vhandle[3]);
	face_vhandles.push_back(vhandle[7]);
	face_vhandles.push_back(vhandle[4]);
	mesh.add_face(face_vhandles);

	// write mesh to output.obj
	try
	{
		if (!OpenMesh::IO::write_mesh(mesh, "output.off"))
		{
			std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
			return EXIT_FAILURE;
		}
	}
	catch (std::exception& x)
	{
		std::cerr << x.what() << std::endl;
		return EXIT_FAILURE;
	}

	MyMesh readMesh;
	try
	{
		if (!OpenMesh::IO::read_mesh(readMesh, "output.off"))
		{
			std::cerr << "Cannot read mesh to file 'output.off'" << std::endl;
			return EXIT_FAILURE;
		}
	}
	catch (const std::exception& x)
	{
		std::cerr << x.what() << std::endl;
		return EXIT_FAILURE;
	}

	std::vector<MyMesh::Point> cogs; 
	for (auto v_it = readMesh.vertices_begin(); v_it != readMesh.vertices_end(); ++v_it)
	{
		MyMesh::Point cog(0.0, 0.0, 0.0);
		MyMesh::Scalar valence(0.0);
		for (auto vv_it = readMesh.vv_iter(*v_it); vv_it.is_valid(); ++vv_it)
		{
			cog += readMesh.point(*vv_it);
			++valence;
		}
		cogs.push_back(cog / valence);
	}

	auto cog_it = cogs.begin();
	for (auto v_it = readMesh.vertices_begin(); v_it != readMesh.vertices_end(); ++v_it)
	{
		if (!readMesh.is_boundary(*v_it))
		{
			readMesh.set_point(*v_it, *cog_it);
		}
		++cog_it;
	}

	// write mesh to output.obj
	try
	{
		if (!OpenMesh::IO::write_mesh(readMesh, "output_0.off"))
		{
			std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
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
