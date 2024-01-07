// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __IMG_PUB_H__
#define __IMG_PUB_H__

#include "libUtil/util.h"
#include "libMsg/MsgImg.h"
#include "libMsg/MsgImgPubSubTypes.h"
#include "libCfg/Cfg.h"
#include "CapImg.h"

#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>


#define IMG_MSG_FMT_YUV 1

class ImgPub
{
public:

    ImgPub( const app::CfgPubImgPtr cfg);

    virtual ~ImgPub();

    //!Initialize
    bool init(bool use_env);
    void run();


    //!Run for number samples
    //std::thread run();

private:
    bool   getNewFrm( const uint64_t fn );
    bool   prepareImgMsg( const uint64_t fn );
    bool   publish( bool waitForListener = true );
    void   runLoop();

private:

    app::CfgPubImgPtr    cfg_{nullptr};
    app::CapImgPtr       cap_{nullptr};   

#if IMG_MSG_FMT_YUV    
    app::HostYuvFrm yuv2Pub_;
#endif

    cv::Mat         img2Pub_;
    MsgImg          imgMsg_;

    app_time_ms_t   sndStartTime_ms_{0};
    double          sndAveragedFrms_{0.0};

    eprosima::fastdds::dds::DomainParticipant* participant_;

    eprosima::fastdds::dds::Publisher* publisher_;

    eprosima::fastdds::dds::Topic* topic_;

    eprosima::fastdds::dds::DataWriter* writer_;

    bool stop_;

    class PubListener : public eprosima::fastdds::dds::DataWriterListener
    {
    public:

        PubListener()
            : matched_(0)
            , firstConnected_(false)
        {
        }

        ~PubListener() override
        {
        }

        void on_publication_matched(
                eprosima::fastdds::dds::DataWriter* writer,
                const eprosima::fastdds::dds::PublicationMatchedStatus& info) override;

        int matched_;

        bool firstConnected_;
    }
    listener_;

    eprosima::fastdds::dds::TypeSupport type_;

};

#endif /* IMAGEPUBLISHER_H_ */
