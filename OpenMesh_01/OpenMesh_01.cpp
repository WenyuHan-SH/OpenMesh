#include <iostream>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
using namespace std;

typedef OpenMesh::PolyMesh_ArrayKernelT<>  PolyMesh;
int main()
{
	PolyMesh ployMesh;
	PolyMesh::VertexHandle vhandle[8];
	vhandle[0] = ployMesh.add_vertex(PolyMesh::Point(-1, -1, 1));
	vhandle[1] = ployMesh.add_vertex(PolyMesh::Point(1, -1, 1));
	vhandle[2] = ployMesh.add_vertex(PolyMesh::Point(1, 1, 1));
	vhandle[3] = ployMesh.add_vertex(PolyMesh::Point(-1, 1, 1));
	vhandle[4] = ployMesh.add_vertex(PolyMesh::Point(-1, -1, -1));
	vhandle[5] = ployMesh.add_vertex(PolyMesh::Point(1, -1, -1));
	vhandle[6] = ployMesh.add_vertex(PolyMesh::Point(1, 1, -1));
	vhandle[7] = ployMesh.add_vertex(PolyMesh::Point(-1, 1, -1));
	// generate (quadrilateral) faces
	std::vector<PolyMesh::VertexHandle>  face_vhandles;
	face_vhandles.clear();
	face_vhandles.push_back(vhandle[0]);
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[2]);
	face_vhandles.push_back(vhandle[3]);
	ployMesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[7]);
	face_vhandles.push_back(vhandle[6]);
	face_vhandles.push_back(vhandle[5]);
	face_vhandles.push_back(vhandle[4]);
	ployMesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[0]);
	face_vhandles.push_back(vhandle[4]);
	face_vhandles.push_back(vhandle[5]);
	ployMesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[2]);
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[5]);
	face_vhandles.push_back(vhandle[6]);
	ployMesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[3]);
	face_vhandles.push_back(vhandle[2]);
	face_vhandles.push_back(vhandle[6]);
	face_vhandles.push_back(vhandle[7]);
	ployMesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[0]);
	face_vhandles.push_back(vhandle[3]);
	face_vhandles.push_back(vhandle[7]);
	face_vhandles.push_back(vhandle[4]);
	ployMesh.add_face(face_vhandles);

	// write ployMesh to output.obj
	try
	{
		if (!OpenMesh::IO::write_mesh(ployMesh, "../source/output_0.off"))
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
