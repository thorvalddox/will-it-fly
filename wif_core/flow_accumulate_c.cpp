#include "flow_accumulate_c.hpp"

#include <numeric>
#include "source_sheet_c.hpp"
#include "vortex_sheet_c.hpp"
#include "add_sheet_c.hpp"
#include <assert.h>
#include <iostream>

using std::vector;
using std::shared_ptr;


namespace wif_core
{

flow_accumulate_c::flow_accumulate_c()
{
	//
}


flow_accumulate_c::~flow_accumulate_c()
{
	//
}


flow_accumulate_c::flow_accumulate_c(const vector<shared_ptr<flow_c>> & flows):
	flows(flows)
{
	//
}


flow_accumulate_c::flow_accumulate_c(shared_ptr<flow_c> flow)
{
	flows.push_back(flow);
}


void flow_accumulate_c::add_source_sheets(const vector<double> & sigmas, const airfoil_c & foil)
{
	vector<line_2d_c> lines = foil.get_lines();
	int l = lines.size();

	for(int i = 0; i < l; i++)
	{
		this->add_source_sheet(lines[i], sigmas[i]);
	}
}


void flow_accumulate_c::add_vortex_sheets(double strength, const airfoil_c & foil)
{
	vector<line_2d_c> lines = foil.get_lines();
	int l = lines.size();

	for(int i = 0; i < l; i++)
	{
		this->add_vortex_sheet(lines[i], strength);
	}
}


void flow_accumulate_c::add_flow(shared_ptr<flow_c> flow)
{
	flows.push_back(flow);
}


double flow_accumulate_c::get_psi(const vector_2d_c & pos) const
{
	add_sheet_psi_c add = add_sheet_psi_c(pos);
	return std::accumulate(flows.begin(), flows.end(), 0.0, add);
}


double flow_accumulate_c::get_phi(const vector_2d_c & pos) const
{
	add_sheet_phi_c add = add_sheet_phi_c(pos);
	return std::accumulate(flows.begin(), flows.end(), 0.0, add);
}


vector_2d_c flow_accumulate_c::get_velocity(const vector_2d_c & pos) const
{
	add_sheet_v_c add = add_sheet_v_c(pos);
	return std::accumulate(flows.begin(), flows.end(), vector_2d_c(0, 0), add);
}


void flow_accumulate_c::add_source_sheet(line_2d_c line, double sigma)
{
	std::shared_ptr<source_sheet_c> point = std::make_shared<source_sheet_c>(line, sigma);
	flows.push_back(point);
}


void flow_accumulate_c::add_vortex_sheet(line_2d_c line, double sigma)
{
	std::shared_ptr<vortex_sheet_c> point = std::make_shared<vortex_sheet_c>(line, sigma);
	flows.push_back(point);
}


} // namespace wif_core
