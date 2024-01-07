#ifndef __CFG_PUB_IMG_H__
#define __CFG_PUB_IMG_H__

#include "CfgBase.h"
namespace app {
   	enum ImgSrc_t {
		IMG_SRC_VIDEO_FILE=0,
		IMG_SRC_IMG_FOLDER,
		IMG_SRC_CAMERA1,
		IMG_SRC_CAMERA2,
		IMG_SRC_COUNT
	};

	class  CFG_EXPORT CfgPubImg : public CfgBase {
	public:
		CfgPubImg();
		CfgPubImg( const CfgPubImg &x )=default;
		CfgPubImg& operator = (const CfgPubImg &x)=default;
		virtual ~CfgPubImg()=default;

		virtual boost::property_tree::ptree toPropertyTree();
		virtual void fromPropertyTree(const boost::property_tree::ptree &pt);
		virtual std::string toString();

	public:
		std::string topicName; 	
		std::string typeName; 	
        std::string pqosName;

        ImgSrc_t    imgSrcType{IMG_SRC_VIDEO_FILE};
        std::string imgUrl {""};
		ImgSize		imgSz;	   //output image sz after H264 decoding regardless the original size from camera
		VideoFps    fps{30,1};
		uint64_t	nTotFrames{0};  //0, loop for ever, >0 # of frame to send out

	};
	typedef std::shared_ptr<CfgPubImg>		CfgPubImgPtr;
}

#endif
