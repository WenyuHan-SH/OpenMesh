#include<iostream>
#include<iterator>

#include<OpenMesh/Core/IO/MeshIO.hh>
#include<OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include<OpenMesh/Tools/Utils/getopt.h>

typedef OpenMesh::TriMesh_ArrayKernelT<> TriMesh;
using namespace OpenMesh;

#define CHKROPT(Option)\
std::cout << "provides" << #Option\
		  << (ropt.check(IO::Options::Option)?":yes\n" : ":no\n")

#define CHKWOPT(Option)\
std::cout << "write" << #Option\
		  << (wopt.check(IO::Options::Option) ? ":yes\n" : ":no\n")

#define MESHOPT(msg,tf)\
std::cout << " " << msg << ": " << ((tf)?"yes\n" : "no\n")

void parse_commandline(const std::string& sMode, const std::string& sPro,
	TriMesh& _mesh, IO::Options& ropt, IO::Options& wopt);
void usage_and_exit(int xcode);

int main()
{
	TriMesh triMesh;
	IO::Options ropt, wopt;
	std::string sModeBits("");
	std::string sPropertyBits("");
	parse_commandline(sModeBits, sPropertyBits, triMesh, ropt, wopt);

	try
	{
		if (!IO::read_mesh(triMesh, "../source/output_0.off", ropt))
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

	std::cout << " is binary" << (ropt.check(IO::Options::Binary) ? "yes\n" : "no\n");
	std::cout << " byte order: ";
	if (ropt.check(IO::Options::Swap))
	{
		std::cout << "swapped\n";
	}
	else if (ropt.check(IO::Options::LSB))
	{
		std::cout << "little endian\n";
	}
	else if (ropt.check(IO::Options::MSB))
	{
		std::cout << "big endian\n";
	}
	else
	{
		std::cout << "don't care\n";
	}
	std::cout << " provides VertexNormal" << (ropt.check(IO::Options::VertexNormal) ? ":yes\n" : ":no\n");
	CHKROPT(VertexColor);
	CHKROPT(VertexTexCoord);
	CHKROPT(FaceNormal);
	CHKROPT(FaceColor);

	std::cout << "# Vertices: " << triMesh.n_vertices() << std::endl;
	std::cout << "# Vertices: " << triMesh.n_faces() << std::endl;
	std::cout << "# Vertices: " << triMesh.n_vertices() << std::endl;

	std::cout << "Selected write options:\n";
	std::cout << " use binary: "
		<< (wopt.check(IO::Options::VertexNormal) ? ":yes\n" : ":no\n");
	CHKWOPT(VertexColor);
	CHKWOPT(VertexTexCoord);
	CHKWOPT(FaceNormal);
	CHKWOPT(FaceColor);

	std::cout << "Mesh support\n";
	MESHOPT("vertex normals", triMesh.has_vertex_normals());
	MESHOPT("vertex colors", triMesh.has_vertex_colors());
	MESHOPT("texcoords", triMesh.has_vertex_texcoords2D());
	MESHOPT("face normals", triMesh.has_face_normals());
	MESHOPT("face colors", triMesh.has_face_colors());

	std::cout << "Write mesh to output_6.off\n";
	try
	{
		if (!OpenMesh::IO::write_mesh(triMesh, "../source/output_6.off"))
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

void parse_commandline(const std::string& sMode, const std::string& sPro,
	TriMesh& _mesh, IO::Options& ropt, IO::Options& wopt)
{
	for (int i = 0; i < sMode.size(); ++i)
	{
		switch (sMode[i])
		{
		case 'b':
			ropt += IO::Options::Binary;
			break;
		case 's':
			ropt += IO::Options::Swap;
			break;
		case'B':
			wopt += IO::Options::Binary;
			break;
		case 'F':
		{
			for (int j = 0; j < sPro.size(); ++j)
			{
				switch (sPro[j])
				{
				case 'n':
					wopt += IO::Options::FaceNormal;
					break;
				case 'c':
					wopt += IO::Options::FaceColor;
					break;
				}
			}
			break;
		}
		case 'L':
			wopt += IO::Options::LSB;
			break;
		case 'M':
			wopt += IO::Options::MSB;
			break;
		case 'S':
			wopt += IO::Options::Swap;
			break;
		case 'v':
		{
			for (int j = 0; j < sPro.size(); ++j)
			{
				switch (sPro[j])
				{
				case 't':
					wopt += IO::Options::VertexTexCoord;
					break;
				case 'c':
					wopt += IO::Options::VertexColor;
					break;
				}
			}
			break;
		}
		case 'X':
		{
			char entity = '\n';
			for (int j = 0; j < sPro.size(); ++j)
			{
				switch (sPro[j])
				{
				case 'v':
				case 'f':
					entity = sPro[j];
					break;
				case 'n':
					switch (entity)
					{
					case'v':
						_mesh.request_vertex_normals();
						break;
					case 'f':
						_mesh.request_face_normals();
						break;
					}
					break;
				case 'c':
					switch (entity)
					{
					case 'v':
						_mesh.request_vertex_colors();
						break;
					case 'f':
						_mesh.request_face_colors();
						break;
					}
				case 't':
					switch (entity)
					{
					case 'v':
						_mesh.request_vertex_texcoords2D();
						break;
					}
				}
			}
		}
		case 'h':
			usage_and_exit(0);
			break;
		default:
			usage_and_exit(1);
			break;
		}
	}
}

void usage_and_exit(int xcode)
{
	std::ostream& os = xcode ? std::cerr : std::cout;
	os << "Usage: io_options [Options] <input> <output>\n"
		<< std::endl;
	os << "  Read and write a mesh, using OpenMesh::IO::Options\n"
		<< std::endl;
	os << "Options:\n"
		<< std::endl;
	os << "a) read options\n"
		<< std::endl
		<< "  -b\n"
		<< "\tAssume input file is a binary file\n"
		<< std::endl
		<< "  -s\n"
		<< "\tSwap byte order when reading a binary file!\n"
		<< std::endl;
	os << "b) write options\n"
		<< std::endl
		<< "  -B\n"
		<< "\tWrite binary data\n"
		<< std::endl
		<< "  -S\n"
		<< "\tSwap byte order, when writing binary data\n"
		<< std::endl
		<< "  -M/-L\n"
		<< "\tUse MSB/LSB byte ordering, when writing binary data\n"
		<< std::endl
		<< "  -V{n|t|c}\n"
		<< "\tWrite vertex normals, texcoords, and/or colors\n"
		<< std::endl
		<< "  -F{n|c}\n"
		<< "\tWrite face normals, and/or colors\n"
		<< std::endl;
	os << "c) Mesh properties\n"
		<< std::endl
		<< "  -Xv{n|c|t}\n"
		<< "\tRequest vertex property normals|colors|texcoords\n"
		<< std::endl
		<< "  -Xf{n|c}\n"
		<< "\tRequest face property normals|colors\n"
		<< std::endl;
	exit(xcode);
}