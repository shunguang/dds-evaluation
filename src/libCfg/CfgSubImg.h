#ifndef __CFG_SUB_IMG_H__
#define __CFG_SUB_IMG_H__

#include "CfgBase.h"
namespace app {

	class  CFG_EXPORT CfgSubImg : public CfgBase {
	public:
		CfgSubImg();
		CfgSubImg( const CfgSubImg &x )=default;
		CfgSubImg& operator = (const CfgSubImg &x)=default;
		virtual ~CfgSubImg()=default;

		virtual boost::property_tree::ptree toPropertyTree();
		virtual void fromPropertyTree(const boost::property_tree::ptree &pt);
		virtual std::string toString();

	public:
		std::string topicName{"topic1"}; 	
		std::string typeName{"abc"}; 	
        std::string pqosName{"pqos-abc"};
		std::string dispWindowName{"win"}; 	
	};
	typedef std::shared_ptr<CfgSubImg>		CfgSubImgPtr;
}

#endif
