#ifndef _TRIANGLE_MESH_H_
#define _TRIANGLE_MESH_H_

#include "vec3.h"
#include "constant.h"
#include "bbox.h"
#include "intersection.h"
#include "qbvh.h"

#include "memfile.h"
#include "textutil.h"

#include "image.h"

namespace hstd {

namespace rt {

struct Material {
	Float3 diffuse;
	Float3 specular;
	float specular_coefficient;
	float metalic;
	Image* tex_diffuse;
	Image* tex_attrib;
	bool projection_receive;
	bool projection_send;

	Material(Float3& diffuse, Float3& specular, float specular_coefficient, float metalic, Image* img, 
		 Image* attrib,
		 bool proj_send, bool proj_recv) :
	diffuse(diffuse), specular(specular), specular_coefficient(specular_coefficient), metalic(metalic),
		/* Extension */
		tex_diffuse(img), tex_attrib(attrib), projection_receive(proj_recv), projection_send(proj_send) {}
};

typedef std::map<std::string, Material*> MaterialMap;

struct Triangle {
	Int3 v_index;
	Int3 vt_index;
	Int3 vn_index;
	Material* material;
};

struct MeshBody {
	std::vector<Float3> v;
	std::vector<Float3> vt;
	std::vector<Float3> vn;
	std::vector<Triangle> triangle;
	MaterialMap matmap;
};

struct TriangleElement {
	Float3* v[3];
	Float3* vt[3];
	Float3* vn[3];
	Material* material;

	TriangleElement() {
		v[0] = v[1] = v[2] = NULL;
		vn[0] = vn[1] = vn[2] = NULL;
		vt[0] = vt[1] = vt[2] = NULL;
		material = NULL;
	}

	inline void fetchTextures(float b1, float b2, Float3* diffuse, float* alpha, Float3* attrib) const {
	    Float3 m;
	    m = *vt[0] + b1 * (*vt[1] - *vt[0]) + b2 * (*vt[2] - *vt[0]);
	    if (diffuse) {
		if (material->tex_diffuse) {
		    *diffuse = material->tex_diffuse->atUV(m.x, m.y);
#if 0
                    printf("Diffuse: %f,%f => %f,%f = (%f,%f,%f)\n",
                           b1,b2, m.x, m.y, diffuse->x, diffuse->y, diffuse->z);
#endif
		}
	    }
	    if (alpha) {
		if (material->tex_diffuse) {
		    *alpha = material->tex_diffuse->alphaUV(m.x, m.y);
		}
	    }
	    if (attrib) {
		if (material->tex_attrib) {
		    *attrib = material->tex_attrib->atUV(m.x, m.y);
		}
	    }
	}
};

class TriangleMesh {
private:
	MeshBody body_;
	QBVH qbvh_;
public:
	void set(const MeshBody& body) {
		body_ = body;
	}

	void build(std::vector<RefTriangle>& ref_triangle) {
		qbvh_.build(ref_triangle);
	}

	TriangleElement getTriangle(const int triangle_index) {
		TriangleElement t;
		if (triangle_index >= body_.triangle.size())
			return t;

		Triangle& now_t = body_.triangle[triangle_index];
		for (int i = 0; i < 3; ++i) {
			t.v[i] = &body_.v[now_t.v_index[i]];
			if (0 <= now_t.vn_index[i] && now_t.vn_index[i] < body_.vn.size())
				t.vn[i] = &body_.vn[now_t.vn_index[i]];
			if (0 <= now_t.vt_index[i] && now_t.vt_index[i] < body_.vt.size())
				t.vt[i] = &body_.vt[now_t.vt_index[i]];
		}
		t.material = now_t.material;

		return t;
	}

	virtual bool intersect(const Ray &ray, Hitpoint* hitpoint) {
		*hitpoint = Hitpoint();
		return qbvh_.intersect(ray, hitpoint);
	}
};


} // namespace rt

} // namespace hstd

#endif // _TRIANGLE_MESH_H_
