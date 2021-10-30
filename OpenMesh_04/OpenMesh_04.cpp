#include <iostream>

#include<OpenMesh/Core/IO/MeshIO.hh>
#include<OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include "smooth_alog.h"

#ifndef DOXY_IGNORE_THIS
struct Traits:public OpenMesh::DefaultTraits
{
	typedef OpenMesh::Vec3d Point;
	typedef OpenMesh::Vec3d Normal;
	HalfedgeAttributes(OpenMesh::Attributes::PrevHalfedge);
};
#endif // !DOXY_IGNORE_THIS

typedef OpenMesh::TriMesh_ArrayKernelT<Traits> TriMesh;

int main()
{
	if (typeid(OpenMesh::vector_traits<TriMesh::Point>::value_type) != typeid(double))
	{
		std::cerr << "Ouch! ERROR! Data type is wrong!\n";
		return EXIT_FAILURE;
	}
	if (typeid(OpenMesh::vector_traits<TriMesh::Normal>::value_type) != typeid(double))
	{
		std::cerr << "Ouch! ERROR! Data type is wrong!\n";
		return EXIT_FAILURE;
	}

	auto spTriMesh = std::make_shared<TriMesh>();
	spTriMesh->request_vertex_normals();
	spTriMesh->request_face_normals();

	if (!spTriMesh->has_vertex_normals() || !spTriMesh->has_face_normals())
	{
		std::cerr << "ERROR: Standard vertex/face property 'Normals' not available!\n";
		return EXIT_FAILURE;
	}

	OpenMesh::IO::Options opt;
	try
	{
		if (!OpenMesh::IO::read_mesh(*spTriMesh, "../source/output_0.off", opt))
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


	if (!opt.check(OpenMesh::IO::Options::VertexNormal))
	{
		spTriMesh->update_normals();
	}

	for (auto v_it = spTriMesh->vertices_begin(); v_it != spTriMesh->vertices_end(); ++v_it)
	{
		std::cout << "Vertex #" << *v_it << ": " << spTriMesh->point(*v_it);
		spTriMesh->set_point(*v_it, spTriMesh->point(*v_it) + spTriMesh->normal(*v_it));
		std::cout << "  move to: " << spTriMesh->point(*v_it) << std::endl;
	}

	spTriMesh->release_vertex_normals();
	spTriMesh->release_face_normals();
	if (spTriMesh->has_vertex_normals() || spTriMesh->has_face_normals())
	{
		std::cerr << "Ouch! ERROR! Shouldn't have any vertex/face normals anymore!\n";
		return EXIT_FAILURE;
	}

	auto spSmoother = std::make_shared<SmootherT<TriMesh>>(*spTriMesh);
	spSmoother->smooth(3);

	// write ployMesh to output.obj
	try
	{
		if (!OpenMesh::IO::write_mesh(*spTriMesh, "../source/output_3.off"))
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