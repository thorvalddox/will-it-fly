#include "vortex_sheet_c.hpp"

#include <cmath>


namespace wif_core
{


vortex_sheet_c::vortex_sheet_c() :
	line(line_2d_c(-1, 0, 1, 0)),
	lambda(1)
{
	//
}


vortex_sheet_c::vortex_sheet_c(const line_2d_c & line, double lambda):
	line(line),
	lambda(lambda)
{
	//
}


vortex_sheet_c::~vortex_sheet_c()
{
	//
}


double vortex_sheet_c::get_psi(const vector_2d_c & pos) const
{
	double a = line.begin.y - pos.y;
	double b = line.end.y - line.begin.y;
	double c = line.begin.x - pos.x;
	double d = line.end.x - line.begin.x;

	double temp1 = a * b + b * b + d * d + c * d;
	double temp2 = log(a * a + 2 * a * b + b * b + d * d + c * c + 2 * c * d);
	double temp3 = lambda / (4 * M_PI * (b * b + d * d));

	double at1 = atan2(b * c - a * d, temp1) ;
	double at2 = atan2(b * c - a * d, a * b + c * d);


	return ((temp1) * temp2
	        + (2 * a * d - 2 * c * b) * at1 - 2 * b * b - 2 * d * d
	        - (a * b + c * d) * log(a * a + c * c) - (2 * a * d - 2 * b * c) * at2)
	       * temp3;
}


double vortex_sheet_c::get_phi(const vector_2d_c & pos) const
{
	double ymin = 0;
	double ymax = 0;

	vector_2d_c transpos = line.get_transformed(pos, ymin, ymax);
	double x = transpos.x;
	double y = transpos.y;

	//std::cout<<ymin<<'\n'<<ymax<<'\n';

	double lo1 = log(ymax * ymax - 2 * ymax * y + x * x + y * y);
	double lo2 = log(ymin * ymin - 2 * ymin * y + x * x + y * y);
	double at1 = atan((ymax - y) / x);
	double at2 = atan((ymin - y) / x);

	return -lambda / (4 * M_PI) * (x * (lo1 - lo2) + 2 * (y - ymax) * at1 - 2 * (y - ymin) * at2);
}


vector_2d_c vortex_sheet_c::get_velocity(const vector_2d_c & pos) const
{
	double ymin = 0;
	double ymax = 0;

	vector_2d_c transpos = line.get_transformed(pos, ymin, ymax);
	double x = transpos.x;
	double y = transpos.y;

	double vy = (atan((y - ymin) / x) - atan((y - ymax) / x)) * lambda / (2 * M_PI);
	double vx = -log((x * x + (y - ymin) * (y - ymin)) / (x * x + (y - ymax) * (y - ymax))) * lambda / (4 * M_PI);
	return vector_2d_c(vx, vy);
}


} // namespace wif_core
