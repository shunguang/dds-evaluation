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

#ifndef __IMG_SUB_H__
#define __IMG_SUB_H__

#include "libMsg/MsgImgPubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastrtps/subscriber/SampleInfo.h>
#include <fastdds/dds/core/status/SubscriptionMatchedStatus.hpp>

#include "libUtil/util.h"
#include "libCfg/Cfg.h"

#define IMG_MSG_FMT_YUV 1

class SubListener : public eprosima::fastdds::dds::DataReaderListener
{
public:
    SubListener(const app::CfgSubImgPtr cfg)
        : cfg_(cfg)
    {
         pktStartTime_ms_ = APP_TIME_CURRENT_MS;
         dispRcvdImg_ = (cfg->dispWindowName == "NULL") ? false : true;
    }

    ~SubListener() override
    {
    }

    void on_data_available(
        eprosima::fastdds::dds::DataReader *reader) override;

    void on_subscription_matched(
        eprosima::fastdds::dds::DataReader *reader,
        const eprosima::fastdds::dds::SubscriptionMatchedStatus &info) override;

public:
    app::CfgSubImgPtr       cfg_{nullptr};    
    app::AppMeanStd<float>  ddsLatencyStatistics_;
    uint64_t                pktRecved_Byte_{0};
    uint64_t                pktStartFrmNum_{0};
    app_time_ms_t           pktStartTime_ms_{0};
    uint64_t                pktAveragedRate_bps_{0};
    float                   pktAveragedFrmsPerSec_{0.0f};
    uint32_t                totalLostFrms_{0};
    uint64_t                prevFrmNum_{0};

    bool                    dispRcvdImg_{false};
    MsgImg imgMsg_;
    int matched_{0};
    uint64_t nMsgRcvd_{0};
};

class ImgSub
{
public:

    ImgSub(const app::CfgSubImgPtr cfg );

    virtual ~ImgSub();

    //!Initialize the subscriber
    bool init(
            bool use_env);

    //!RUN the subscriber
    void run();

    //!Run the subscriber until number samples have been received.
    void run(uint32_t number);

private:
    app::CfgSubImgPtr    cfg_{nullptr};

    eprosima::fastdds::dds::DomainParticipant* participant_;

    eprosima::fastdds::dds::Subscriber* subscriber_;

    eprosima::fastdds::dds::Topic* topic_;

    eprosima::fastdds::dds::DataReader* reader_;

    eprosima::fastdds::dds::TypeSupport type_;

    SubListener  listener_;


};

void createImageSubscriber(bool use_environment_qos, const app::CfgSubImgPtr cfg);

#endif /* IMAGESUBSCRIBER_H_ */
