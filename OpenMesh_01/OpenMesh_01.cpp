#include <iostream>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/System/config.h>
#include <OpenMesh/Core/Mesh/Status.hh>

#include <OpenMesh/Tools/Subdivider/Uniform/CatmullClarkT.hh>
#include <OpenMesh/Tools/Smoother/JacobiLaplaceSmootherT.hh>
#include <OpenMesh/Tools/Smoother/LaplaceSmootherT.hh>
struct Traits :public OpenMesh::DefaultTraits
{

};

typedef OpenMesh::PolyMesh_ArrayKernelT<Traits>  PolyMesh;


int main()
{
	PolyMesh ployMesh;

	ployMesh.request_face_status();
	ployMesh.request_edge_status();
	ployMesh.request_vertex_status();

	PolyMesh::VertexHandle vhandle[8];
	PolyMesh::FaceHandle fhandle[6];
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
	fhandle[1] = ployMesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[0]);
	face_vhandles.push_back(vhandle[4]);
	face_vhandles.push_back(vhandle[5]);
	fhandle[2] = ployMesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[2]);
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[5]);
	face_vhandles.push_back(vhandle[6]);
	fhandle[3] = ployMesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[3]);
	face_vhandles.push_back(vhandle[2]);
	face_vhandles.push_back(vhandle[6]);
	face_vhandles.push_back(vhandle[7]);
	fhandle[4] = ployMesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[0]);
	face_vhandles.push_back(vhandle[3]);
	face_vhandles.push_back(vhandle[7]);
	face_vhandles.push_back(vhandle[4]);
	fhandle[5] = ployMesh.add_face(face_vhandles);

	OpenMesh::Subdivider::Uniform::CatmullClarkT<PolyMesh> catmull;
	catmull.attach(ployMesh);
	catmull(3);
	catmull.detach();

	OpenMesh::Smoother::JacobiLaplaceSmootherT<PolyMesh> smoother(ployMesh);
	smoother.initialize(smoother.Tangential_and_Normal, smoother.C1);
	smoother.smooth(3);

	ployMesh.delete_face(fhandle[0], false);
	ployMesh.delete_face(fhandle[2], false);
	ployMesh.delete_face(fhandle[3], false);
	ployMesh.delete_face(fhandle[4], false);
	ployMesh.delete_face(fhandle[5], false);

	ployMesh.delete_vertex(vhandle[0], false);
	ployMesh.delete_vertex(vhandle[1], false);
	ployMesh.delete_vertex(vhandle[2], false);
	ployMesh.delete_vertex(vhandle[3], false);

	ployMesh.garbage_collection();
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
