/*
*------------------------------------------------------------------------
*Cfg.cpp
*
* This code was developed by Shunguang Wu in his spare time. No government
* or any client funds were used.
*
*
* THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
*
* IN NO EVENT SHALL THE AUTHOR OR DISTRIBUTOR BE LIABLE FOR
* ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
* OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
* WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
* LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
* OF THIS SOFTWARE.
*
* Permission to use, copy, modify, distribute, and sell this software and
* its documentation for any purpose is prohibited unless it is granted under
* the author's written notice.
*
* Copyright(c) 2020 by Shunguang Wu, All Right Reserved
*-------------------------------------------------------------------------
*/
#include "Cfg.h"
using namespace std;
using namespace app;

Cfg::Cfg()
	: m_camIdVec()
	, m_camMap()
	, m_pubImg(new CfgPubImg())
	, m_subImg(new CfgSubImg())
	, m_log(new CfgLog())
{
}

void Cfg::readFromFile(const char *fname)
{
	boost::property_tree::ptree pt;
	boost::property_tree::xml_parser::read_xml( fname, pt);
	this->fromPropertyTree(pt.get_child("cfg"));
	printf("xml read done!\n");
}

void Cfg::writeToFile(const char *fname)
{
	boost::property_tree::ptree pt = toPropertyTree();
	boost::property_tree::xml_parser::xml_writer_settings<std::string> settings(' ', 4);
	boost::property_tree::xml_parser::write_xml(fname, pt, std::locale(), settings);
}

std::string Cfg::toString()
{
	boost::property_tree::ptree pt = toPropertyTree();

	std::ostringstream oss;
	boost::property_tree::xml_parser::xml_writer_settings<std::string> settings(' ', 4);
	boost::property_tree::xml_parser::write_xml(oss, pt, settings);
	return oss.str();
}

void Cfg::fromPropertyTree(const boost::property_tree::ptree &pt0)
{
	const boost::property_tree::ptree pt = pt0.get_child("cams");
	m_camIdVec.clear();

#if 0
  	for(const boost::property_tree::ptree::value_type &g : pt) {
		CfgCamPtr curr( new CfgCam() );
		curr->fromPropertyTree( g.second );
		//cout << curr->toString() << endl;
		if( curr->valid_ ){
			int camId = curr->cameraId_;
			m_camMap[camId] = curr; 
			m_camIdVec.push_back( camId );
		}
	}
#endif

	m_pubImg->fromPropertyTree(pt0.get_child("pubImg"));
	m_subImg->fromPropertyTree(pt0.get_child("subImg"));
	m_log->fromPropertyTree(pt0.get_child("log"));
}


boost::property_tree::ptree Cfg::toPropertyTree()
{
	boost::property_tree::ptree ptLog = m_log->toPropertyTree();
	boost::property_tree::ptree ptPubImg = m_pubImg->toPropertyTree();
	boost::property_tree::ptree ptSubImg = m_subImg->toPropertyTree();

	boost::property_tree::ptree pt;

#if 0	
	for ( const auto camId : m_camIdVec){
		CfgCam cam = getCam( camId );
		boost::property_tree::ptree  ptCam = cam.toPropertyTree();
		pt.add_child( "cfg.cams.cam", ptCam );
	}	
#endif

	pt.add_child("cfg.pubImg", ptPubImg);
	pt.add_child("cfg.subImg", ptSubImg);
	pt.add_child("cfg.log", ptLog);

	return pt;
}



void Cfg::updateRecFlag(int camIdx, bool isRecording) {
	boost::mutex::scoped_lock lock(m_mutex);
	m_camMap[camIdx]->isRec_ = isRecording;
}

void Cfg::updateDispFlag(int camIdx, bool isDisp) {
	boost::mutex::scoped_lock lock(m_mutex);
	m_camMap[camIdx]->isDisp_ = isDisp;
}

void Cfg::updateCamName(int camIdx, std::string name)
{
	boost::mutex::scoped_lock lock(m_mutex);
	m_camMap[camIdx]->cameraName_ = name;
}

