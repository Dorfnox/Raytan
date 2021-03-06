#include "Subtraction.hpp"

SmallVector<double>	Subtraction::findDistances(const Ray&) const
{
	assert(!"non primitive implementation should not be called");
	return SmallVector<double>();
}


SmallVector<Intersect>	Subtraction::getIntersectionsFrom(const IObject *o, const Ray& ray) const
{
	Ray transformed;
	transformed.origin = InverseTransformPoint(ray.origin, o->transform);
	transformed.direction = InverseTransformVector(ray.direction, o->transform);

	if (!o->IsPrimitive())
		return o->findIntersections(transformed);

	auto dists = o->findDistances(transformed);
	SmallVector<Intersect> out;
	for (size_t index = 0; index < dists.size(); index++)
	{
		double d = dists[index];
		Intersect i = {d, o, o->transform, true};
		out.push_back(i);
	}
	return out;

}

bool Subtraction::isFacing(const Intersect& i, const Ray& ray) const
{
	glm::dvec3 hitPoint = ray.origin + ray.direction * i.distance;
	hitPoint = InverseTransformPoint(hitPoint, i.transform);

	glm::dvec3 normal = i.hitRef->findNormal(hitPoint);
	normal = TransformVector(normal, i.transform);

	if (!i.positive)
		normal = -normal;
	
	if (glm::dot(normal, ray.direction) < 0)
		return true;
	return false;	
}

inline std::vector<Edge> Subtraction::generateEdges(const SmallVector<Intersect>& p,
						    const SmallVector<Intersect>& n,
						    const Ray& ray) const
{
	std::vector<Edge> out;
	Edge edge;

	for (size_t index = 0; index < p.size(); index++)
	{
		const auto& i = p[index];
		edge.inter = i;
		edge.forwardFacing = isFacing(i, ray);
		edge.positive = true;
		out.push_back(edge);
	}
	for (size_t index = 0; index < n.size(); index++)
	{
		const auto& i = n[index];
		edge.inter = i;
		edge.forwardFacing = isFacing(i, ray);
		edge.inter.positive = !edge.inter.positive;
		edge.positive = false;
		out.push_back(edge);
	}

	std::sort(out.begin(),
		  out.end(),
		  [](Edge& a, Edge& b){return a.inter.distance < b.inter.distance;});

	return out;
}

SmallVector<Intersect>	Subtraction::findIntersections(const Ray& ray) const
{
	auto p = getIntersectionsFrom(_positive, ray);
	auto n = getIntersectionsFrom(_negative, ray);

	std::vector<Edge> edges = generateEdges(p, n, ray);
	
	bool insideP = false;
	bool insideN = false;

	// checking if ray starts off inside an object
	bool toggle1 = false;
	bool toggle2 = false;
	for (auto& edge : edges)
	{
		if (edge.positive && !toggle1)
		{
			insideP = !edge.forwardFacing;
			toggle1 = true;
		}
		if (!edge.positive && !toggle2)
		{
			insideN = !edge.forwardFacing;
			toggle2 = true;
		}
		if (toggle1 && toggle2)
			break;
	}
	SmallVector<Intersect> out;
	for (auto& edge : edges)
	{
		int event = insideP + 2 * insideN + 4 * edge.positive + 8 * edge.forwardFacing;

		switch(event)
		{
		case(0b1001):
			edge.inter.transform = CompoundTransform(edge.inter.transform, transform);
			out.push_back(edge.inter);
			insideN = true;
			break;
		case(0b1000):
			insideN = true;
			break;
		case(0b0011):
			edge.inter.transform = CompoundTransform(edge.inter.transform, transform);
			out.push_back(edge.inter);
			insideN = false;
			break;
		case(0b0010):
			insideN = false;
			break;
		case(0b1110):
			insideP = true;
			break;
		case(0b1100):
			edge.inter.transform = CompoundTransform(edge.inter.transform, transform);
			out.push_back(edge.inter);
			insideP = true;
			break;
		case(0b0111):
			insideP = false;
			break;
		case(0b0101):
			edge.inter.transform = CompoundTransform(edge.inter.transform, transform);
			out.push_back(edge.inter);
			insideP = false;
			break;
		default:
			;
//			for (auto& e : edges)
//			{
//				std::cout << e.inter.distance <<
//					" " << e.inter.hitRef << " " <<
//					e.inter.positive << " " <<
//					e.forwardFacing << " " << e.positive << std::endl;
//			}
//			std::cout << "bad edge order: " << insideP << " " << insideN << " " <<
//				edge.positive << " " << edge.forwardFacing << std::endl;
//			assert(!"badly defined shape");
		}
	}
	return out;	
}

Subtraction::Subtraction(IObject *positive, IObject *negative)
{
	_positive = positive;
	_negative = negative;
}

Subtraction::~Subtraction(void)
{
	delete _positive;
	delete _negative;
}

bool	Subtraction::IsPrimitive(void) const
{
	return false;	
}

Intersect	Subtraction::Intersection(const Ray& ray) const
{
	Ray transformed;
	transformed.origin = InverseTransformPoint(ray.origin, transform);
	transformed.direction = InverseTransformVector(ray.direction, transform);

	auto hits = findIntersections(transformed);
	Intersect best;
	best.distance = INFINITY;

	for (size_t index = 0; index < hits.size(); index++)
	{
		auto& hit = hits[index];
		if (hit.distance < best.distance && hit.distance > 0)
			best = hit;
	}
	return best;
}
