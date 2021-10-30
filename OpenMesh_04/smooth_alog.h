#ifndef SMOOTH_ALGO_H_
#define SMOOTH_ALGO_H_
#include <algorithm>
#include<OpenMesh/Core/Utils/Property.hh>
#ifndef DOXY_IGNORE_THIS
template <class Mesh>
class SmootherT
{
public:
	typedef typename Mesh::Point cog_t;
	typedef OpenMesh::VPropHandleT<cog_t> Property_cog;
public:
	explicit SmootherT(Mesh& mesh) :mesh_(mesh)
	{
		mesh_.add_property(cog_);
	}
	~SmootherT()
	{
		mesh_.remove_property(cog_);
	}
	void smooth(const unsigned int _iterations)
	{
		std::for_each(mesh_.vertices_begin(), mesh_.vertices_end(), ComputeCOG(mesh_, cog_));
		std::for_each(mesh_.vertices_begin(), mesh_.vertices_end(), SetCOG(mesh_, cog_));
	}
private:
	class ComputeCOG
	{
	public:
		ComputeCOG(Mesh& _mesh, Property_cog& _cog) :mesh_(_mesh), cog_(_cog) {}
		void operator()(const typename Mesh::VertexHandle& _vh)
		{
			typename Mesh::VertexVertexIter		vv_it;
			typename Mesh::Scalar				valence(0.0);
			mesh_.property(cog_, _vh) = typename Mesh::Point(0.0, 0.0, 0.0);
			for (vv_it = mesh_.vv_iter(_vh); vv_it.is_valid(); ++vv_it)
			{
				mesh_.property(cog_, _vh) += mesh_.point(*vv_it);
				++valence;
			}
			mesh_.property(cog_, _vh) /= valence;
		}
	private:
		Mesh& mesh_;
		Property_cog cog_;
	};

	class SetCOG
	{
	public:
		SetCOG(Mesh& _mesh, Property_cog& _cog) :mesh_(_mesh), cog_(_cog) {}
		void operator()(const typename Mesh::VertexHandle& _vh)
		{
			if (!mesh_.is_boundary(_vh))
			{
				mesh_.set_point(_vh, mesh_.property(cog_, _vh));
			}
		}
	private:
		Mesh& mesh_;
		Property_cog cog_;
	};

private:
	Mesh& mesh_;
	Property_cog cog_;
};
#endif // !DOXY_IGNORE_THIS


#endif // !SMOOTH_ALGO_H_

