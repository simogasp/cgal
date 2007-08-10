// TODO: Add licence
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL:$
// $Id: $
// 
//
// Author(s)     : 
//
// ============================================================================

#ifndef CGAL_ALGEBRAIC_CURVE_KERNEL_CURVE_PAIR_ANALYSIS_2_H
#define CGAL_ALGEBRAIC_CURVE_KERNEL_CURVE_PAIR_ANALYSIS_2_H

#include <CGAL/basic.h>
#include <CGAL/Handle_with_policy.h>

#include <SoX/basic.h>
//#include <CGAL/algorithm.h>
#include <SoX/GAPS/types.h>

CGAL_BEGIN_NAMESPACE

template < class AlgebraicCurveKernel_2, class Rep_ > 
class Curve_pair_analysis_2;

namespace CGALi {

template < class AlgebraicCurveKernel_2 >
class Curve_pair_analysis_2_rep {

public:
	// this first template argument
	typedef AlgebraicCurveKernel_2 Algebraic_curve_kernel_2;

	// myself
    typedef Curve_pair_analysis_2_rep<Algebraic_curve_kernel_2> Self;

	// type of curve pair
	typedef typename Algebraic_curve_kernel_2::Curve_pair_2 Curve_pair_2;
	
	// type of 1-curve analysis
	typedef typename Algebraic_curve_kernel_2::Curve_analysis_2
		Curve_analysis_2;

	// constructors
public:
    // default constructor ()
	Curve_pair_analysis_2_rep() 
	{   }

     // temporary constructor: directly from curve pair
    Curve_pair_analysis_2_rep(const Curve_pair_2& curve_pair) : 
		ca1_(curve_pair.curve1()), ca2_(curve_pair.curve2()), 
			curve_pair_(curve_pair)
	{   }

	// constructs from two curve analysis
	Curve_pair_analysis_2_rep(const Curve_analysis_2& ca1,
		const Curve_analysis_2& ca2) : ca1_(ca1), ca2_(ca2),
			curve_pair_(ca1.get_polynomial_2(), ca2.get_polynomial_2())
	{  }

    // data
	mutable Curve_analysis_2 ca1_, ca2_;
	
	// temporarily this implementation uses underlying Curve_pair_2 from SweepX
    mutable Curve_pair_2 curve_pair_;
    
    // befriending the handle
    friend class Curve_pair_analysis_2<Algebraic_curve_kernel_2, Self>;
};
    
//! \brief The class is meant to provide tools to analyse a pair of curves. 
//!
//! Analysis describes the curve pair's interesting points and how they are 
//! connected. The analysis searches for events. Events only occur at a finite 
//! number of x-coordinate. Each such coordinate is covered by a 
//! \c CurvePairVerticalLine_1, originated by the events of a single curve and 
//! also the intersections of two curves. These coordinates also define
//! open intervals on the x-axis. \c CurvePairVerticalLine 1 at values in 
//! between one such interval differ only in the values of the 
//! \c Algebraic_real_2 entries. Topological information are equal.
template <class AlgebraicCurveKernel_2, 
	  class Rep_ = CGALi::Curve_pair_analysis_2_rep<AlgebraicCurveKernel_2> >
class Curve_pair_analysis_2 : public ::CGAL::Handle_with_policy< Rep_ > 
{
public:
	//!@{
	//! \name typedefs

    //! this instance's first template parameter
	typedef AlgebraicCurveKernel_2 Algebraic_curve_kernel_2;

    //! this instance's second template parameter
	typedef Rep_ Rep;

	//! x-coordinate type
	typedef typename Algebraic_curve_kernel_2::X_coordinate_1 X_coordinate_1;

	//! type of a curve point
	typedef typename Algebraic_curve_kernel_2::Xy_coordinate_2 Xy_coordinate_2;

	//! type of a curve pair
	typedef typename Algebraic_curve_kernel_2::Curve_pair_2 Curve_pair_2;

	//! type of a curve
	typedef typename Algebraic_curve_kernel_2::Curve_2 Curve_2;

	//! type of 1-curve analysis
	typedef typename Algebraic_curve_kernel_2::Curve_analysis_2 
			Curve_analysis_2;

	//! myself
	typedef Curve_pair_analysis_2<Algebraic_curve_kernel_2, Rep> Self;

	//! type of a vertical line
	typedef CGALi::Curve_pair_vertical_line_1<Self>
		Curve_pair_vertical_line_1;
		
	//! the handle superclass
    typedef ::CGAL::Handle_with_policy< Rep > Base;

	//!@}
	//!\name Constructors
	//!@{

	//! \brief default constructor
    Curve_pair_analysis_2() : Base(Rep()) 
    {  }

    /*!\brief
     * copy constructor
     */
    Curve_pair_analysis_2(const Self& p) : Base(static_cast<const Base&>(p)) 
	{  }

	//! constructs a curve pair analysis defined by analysis
	//! given by \c ca1_ and \c ca2_. The polynomials defining the analysis
	//! must be squarefree and coprime.
	Curve_pair_analysis_2(const Curve_analysis_2& ca1_,
		const Curve_analysis_2& ca2_) : Base(Rep(ca1_, ca2_))
	{  }

	// temporary constructor to construct directly from curve pair
	Curve_pair_analysis_2(const Curve_pair_2& curve_pair_) : 
			Base(Rep(curve_pair_))
	{  }
           
    /*!\brief
     * constructs a curve pair analysis from a given represenation
     */
    Curve_pair_analysis_2(Rep rep) : Base(rep) 
    {  }

	//!@}
	//!\name Access functions
	//!@{

	//! \brief returns curve analysis for c-"th" curve (0 or 1)
	Curve_analysis_2 get_curve_analysis(bool c) const
	{ 
		if(c == 0)
			return this->ptr()->ca1_;
		return this->ptr()->ca2_;
	}

	//! \brief returns number of vertical lines that encode an event
	int number_of_vertical_lines_with_event()
	{
		return this->ptr()->curve_pair_.num_events();
	}

	//! \brief given the i-th event of the curve pair this method returns the
	//! id of the event of the corresponding curve analysis \c c (0 or 1),
	//! or -1, if the curve has no event at this coordinate.
	//!
	//! \pre 0 <= i < number_of_vertical_lines_with_event()
	int event_of_curve_analysis(int i, bool c)
	{
		CGAL_precondition(i >= 0&&i < number_of_vertical_lines_with_event());
		SoX::Index_triple triple =  this->ptr()->curve_pair_.event_indices(i);
		return (c == 0 ? triple.ffy : triple.ggy);
	}

  	//! \brief returns an instance of \c CurvePairVerticalLine_1 at the i-th
	//! event
	//!
	//! \pre 0 <= i < number_of_vertical_lines_with_event()
	Curve_pair_vertical_line_1 vertical_line_at_event(int i)
	{
		CGAL_precondition(i >= 0&&i < number_of_vertical_lines_with_event());
		return  this->ptr()->curve_pair_.slice_at_event(i);
	}

	//! \brief returns an instance of CurvePairVerticalLine_1 of the i-th 
	//! interval between x-events
	//!
	//! \pre 0 <= i < number_of_vertical_lines_with_event() 
	Curve_pair_vertical_line_1 vertical_line_of_interval(int i)
	{
		CGAL_precondition(i >= 0&&i <= number_of_vertical_lines_with_event());
		return  this->ptr()->curve_pair_.slice_at_interval(i); 
	}

	//! \brief returns vertical_line_at_event(i), if x hits i-th event, 
	//! otherwise vertical_line_of_interval(i), where i is the id of the 
	//! interval \c x lies in.
	//!
	//! If \c pertub is \c CGAL::NEGATIVE (CGAL::POSITIVE) and x states an 
	//! event, then \c vertical_line_of_interval(i)
	//! (\c vertical_line_of_interval(i+1)) is returned.
	//! 
	//! \pre \c x is finite
	Curve_pair_vertical_line_1 vertical_line_for_x(X_coordinate_1 x, 
		CGAL::Sign perturb = CGAL::ZERO)
	{
		// CGAL_precondition(x is finite ??);
		int i;
		bool is_evt;
		this->ptr()->curve_pair_.x_to_index(x, i, is_evt);
		if(is_evt) {
			if(perturb == CGAL::ZERO)
				return vertical_line_at_event(i);
			if(perturb == CGAL::POSITIVE)
				i++;
		}
		return vertical_line_of_interval(i);
	}

	//! \brief returns an instance of CurvePairVerticalLine_1 at a given \c x
	//!
	//! \pre \c x is finite
	Curve_pair_vertical_line_1 vertical_line_at_exact_x(X_coordinate_1 x)
	{
		// CGAL_precondition(x is finite ??);
		return vertical_line_for_x(x);
	}

}; // class Curve_pair_analysis_2

} // namespace CGALi

CGAL_END_NAMESPACE

#endif // CGAL_ALGEBRAIC_CURVE_KERNEL_CURVE_PAIR_ANALYSIS_2_H
