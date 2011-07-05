#include <CGAL/Cartesian.h>

#include <CGAL/Convex_hull_traits_3.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/Object.h>
#include <CGAL/point_generators_3.h>
#include <vector>
#include <cassert>
#ifdef CGAL_USE_LEDA
#  include <CGAL/leda_rational.h>
typedef leda_rational                   Precise_rational;
#elif defined CGAL_USE_GMP
#  include <CGAL/Gmpz.h>
#  include <CGAL/Quotient.h>
typedef CGAL::Quotient<CGAL::Gmpz>      Precise_rational;
#else
#  include <CGAL/MP_Float.h>
#  include <CGAL/Quotient.h>
typedef CGAL::Quotient<CGAL::MP_Float>  Precise_rational;
#endif


typedef Precise_rational                              NT;
typedef CGAL::Cartesian<NT>                           K;
typedef CGAL::Convex_hull_traits_3<K>                 Traits;
typedef Traits::Polyhedron_3                          Polyhedron_3;

typedef K::Point_3                                        Point_3;
typedef K::Segment_3                                      Segment_3;

typedef CGAL::Creator_uniform_3<NT,Point_3>               Creator;
typedef CGAL::Random_points_in_sphere_3<Point_3,Creator>  Generator;

const unsigned int num = 40;

void test_tetrahedron_convexity()
{
    Polyhedron_3 P;

    P.make_tetrahedron( Point_3(1,0,0),
                        Point_3(-1,1,0),
                        Point_3(0,0,1),
                        Point_3(-1,-1,0) );

    for( Polyhedron_3::Facet_iterator f = P.facets_begin();
         f != P.facets_end(); ++f )
    {
        CGAL::compute_plane_equation(f);
    }

    assert( CGAL::is_strongly_convex_3(P) );
}

void test_triangle_convexity()
{
    Polyhedron_3 P;

    P.make_triangle( Point_3(1,0,0),
                     Point_3(-1,1,0),
                     Point_3(0,0,1));

    for( Polyhedron_3::Facet_iterator f = P.facets_begin();
         f != P.facets_end(); ++f )
    {
        CGAL::compute_plane_equation(f);
    }

    assert( CGAL::is_strongly_convex_3(P) );
}


void test_small_hull()
{
  std::vector<Point_3> points;
  points.push_back(Point_3(0,0,0));
  points.push_back(Point_3(10,0,0));
  points.push_back(Point_3(0,10,0));
  points.push_back(Point_3(0,0,10));
  points.push_back(Point_3(5,5,5));
  points.push_back(Point_3(2,5,3));
  points.push_back(Point_3(1,3,2));

  Polyhedron_3 polyhedron1;
  CGAL::convex_hull_3(points.begin(), points.end(), polyhedron1, Traits());
  assert( polyhedron1.size_of_vertices() == 5 && 
           polyhedron1.size_of_facets() == 6 );
  Polyhedron_3 polyhedron2;
  CGAL::convex_hull_3(points.begin(), points.end(), polyhedron2);
  assert(CGAL::is_strongly_convex_3(polyhedron2)); // test default traits class
  assert( polyhedron2.size_of_vertices() == 5 && 
          polyhedron2.size_of_facets() == 6 );
}


int main()
{
  std::cerr << "Testing triangle" << std::endl;
  test_triangle_convexity();
  std::cerr << "Testing tetrahedron" << std::endl;
  test_tetrahedron_convexity();
  std::cerr << "Testing small hull" << std::endl;
  test_small_hull();

  std::cerr << "Testing 500 random points" << std::endl;
  std::vector<Point_3> points;
  Generator g(500);
  CGAL::copy_n( g, num, std::back_inserter(points));

  assert(points.size() == num);

  CGAL::Object ch_object;
  CGAL::convex_hull_3(points.begin(), points.end(), ch_object, Traits());
  CGAL::convex_hull_3(points.begin(), points.end(), ch_object);

  Segment_3 segment;

  Polyhedron_3 polyhedron;

  assert( CGAL::assign(segment, ch_object) || 
          CGAL::assign(polyhedron, ch_object) );
  return 0;
}
